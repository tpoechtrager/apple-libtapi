//===- lib/Driver/ScanDriver.cpp - TAPI Scan Driver -------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the scan driver for the tapi tool.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/Configuration.h"
#include "tapi/Core/Framework.h"
#include "tapi/Core/HeaderFile.h"
#include "tapi/Core/Path.h"
#include "tapi/Core/Registry.h"
#include "tapi/Core/TextAPI_v1.h"
#include "tapi/Core/Utils.h"
#include "tapi/Defines.h"
#include "tapi/Driver/Diagnostics.h"
#include "tapi/Driver/DirectoryScanner.h"
#include "tapi/Driver/Driver.h"
#include "tapi/Driver/DriverUtils.h"
#include "tapi/Driver/Options.h"
#include "tapi/Driver/Snapshot.h"
#include "tapi/Driver/SnapshotFileSystem.h"
#include "tapi/Scanner/Scanner.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/VirtualFileSystem.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Regex.h"

using namespace llvm;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

// Scan Driver Context.
namespace {
struct Context {
  // Make the context not copyable.
  Context(const Context &) = delete;
  Context &operator=(const Context &) = delete;

  using DirectorySeq = std::vector<std::string>;
  using FrameworkSeq = std::vector<Framework>;

  DirectorySeq scanDirectories;
  FrameworkSeq frameworks;
  Configuration config;
  Registry registry;
  FileManager &fm;
  DiagnosticsEngine &diag;

  Context(FileManager &fm, DiagnosticsEngine &diag) : fm(fm), diag(diag) {
    registry.addBinaryReaders();
    registry.addYAMLReaders();
    registry.addYAMLWriters();
  }

  /// \brief Add a directory to the scan worklist.
  bool addScanDirectory(StringRef path) {
    SmallString<PATH_MAX> absolutePath(path);
    fm.makeAbsolutePath(absolutePath);
    if (!fm.isDirectory(absolutePath, /*CacheFailure=*/false))
      return false;

    scanDirectories.emplace_back(absolutePath.c_str());
    return true;
  }

  using const_directory_range =
      llvm::iterator_range<DirectorySeq::const_iterator>;
  const_directory_range directories() const {
    return {scanDirectories.begin(), scanDirectories.end()};
  }
};
} // namespace

static std::unique_ptr<ParsingJob> createJob(const Context &context,
                                             const Framework &framework) {
  auto &fm = context.fm;
  auto job = make_unique<ParsingJob>();
  job->fileManager = &fm;

  // Get the list of all architectures we have to build for.
  for (const auto &file : framework._interfaceFiles)
    job->architectures |= file->getArchitectures();

  auto basePath = framework.getPath();

  job->language = context.config.getLanguage(basePath);
  job->language_std = context.config.commandLine.std;
  job->useRTTI = context.config.commandLine.useRTTI;
  job->visibility = context.config.commandLine.visibility;
  job->isysroot = context.config.getSysRoot();
  job->macros = context.config.getMacros(basePath);
  job->frameworkPaths = context.config.getFrameworkPaths(basePath);
  job->includePaths = context.config.getIncludePaths(basePath);
  job->clangExtraArgs = context.config.commandLine.clangExtraArgs;
  job->scanPublicHeaders = context.config.commandLine.scanPublicHeaders;
  job->scanPrivateHeaders = context.config.commandLine.scanPrivateHeaders;
  job->enableModules = context.config.commandLine.enableModules;
  job->moduleCachePath = context.config.commandLine.moduleCachePath;
  job->validateSystemHeaders = context.config.commandLine.validateSystemHeaders;
  job->clangResourcePath = context.config.commandLine.clangResourcePath;

  for (auto &header :
       context.config.getPreIncludedHeaders(basePath, HeaderType::Public)) {
    if (auto *file = fm.getFile(header))
      job->publicPreIncludeFiles.emplace_back(file);
    else {
      context.diag.report(diag::err_no_such_public_pre_header_file) << header;
      return nullptr;
    }
  }

  for (auto &header :
       context.config.getPreIncludedHeaders(basePath, HeaderType::Private)) {
    if (auto *file = fm.getFile(header))
      job->privatePreIncludeFiles.emplace_back(file);
    else {
      context.diag.report(diag::err_no_such_private_pre_header_file) << header;
      return nullptr;
    }
  }

  // Create a sorted list of framework headers.
  std::vector<const FileEntry *> publicHeaderFiles;
  std::vector<const FileEntry *> privateHeaderFiles;
  for (const auto &header : framework._headerFiles) {
    auto *file = fm.getFile(header.first);
    if (file == nullptr) {
      if (header.second == HeaderType::Public)
        context.diag.report(diag::err_no_such_public_header_file)
            << header.first;
      else
        context.diag.report(diag::err_no_such_private_header_file)
            << header.first;
      return nullptr;
    }
    if (header.second == HeaderType::Private)
      privateHeaderFiles.emplace_back(file);
    else
      publicHeaderFiles.emplace_back(file);
  }
  sort(publicHeaderFiles, [](const FileEntry *lhs, const FileEntry *rhs) {
    return StringRef(lhs->getName()) < StringRef(rhs->getName());
  });
  sort(privateHeaderFiles, [](const FileEntry *lhs, const FileEntry *rhs) {
    return StringRef(lhs->getName()) < StringRef(rhs->getName());
  });

  // Create the excluded headers list.
  std::vector<const FileEntry *> publicExcludedHeaderFiles;
  std::vector<const FileEntry *> privateExcludedHeaderFiles;

  for (auto &exclude :
       context.config.getExcludedHeaders(basePath, HeaderType::Public)) {
    SmallString<PATH_MAX> path(basePath);
    sys::path::append(path, "Headers", exclude);
    if (const auto *file = context.fm.getFile(exclude))
      publicExcludedHeaderFiles.emplace_back(file);
    else if (const auto *file = context.fm.getFile(path))
      publicExcludedHeaderFiles.emplace_back(file);
    else {
      context.diag.report(diag::err_no_such_excluded_public_header_file)
          << exclude;
      return nullptr;
    }
  }

  for (auto &exclude :
       context.config.getExcludedHeaders(basePath, HeaderType::Private)) {
    SmallString<PATH_MAX> path(basePath);
    sys::path::append(path, "PrivateHeaders", exclude);
    if (const auto *file = context.fm.getFile(exclude))
      privateExcludedHeaderFiles.emplace_back(file);
    else if (const auto *file = context.fm.getFile(path))
      privateExcludedHeaderFiles.emplace_back(file);
    else {
      context.diag.report(diag::err_no_such_excluded_private_header_file)
          << exclude;
      return nullptr;
    }
  }

  // Remove exclude headers from header list.
  publicHeaderFiles.erase(
      remove_if(publicHeaderFiles, [&](const FileEntry *file) {
        return find(publicExcludedHeaderFiles, file) !=
               publicExcludedHeaderFiles.end();
      }), publicHeaderFiles.end());

  privateExcludedHeaderFiles.erase(
      remove_if(privateHeaderFiles, [&](const FileEntry *file) {
        return find(privateExcludedHeaderFiles, file) !=
               privateExcludedHeaderFiles.end();
      }), privateHeaderFiles.end());

  // Check if the framework has an umbrella header and move that to the
  // beginning.
  auto matchFirstAndMoveToTop = [](std::vector<const FileEntry *> &array,
                                   Regex &regex) -> bool {
    auto it = find_if(array, [&regex](const FileEntry *file) {
      return regex.match(file->getName());
    });

    if (it == array.end())
      return false;

    std::rotate(array.begin(), it, std::next(it));
    return true;
  };
  auto &publicUmbrellaHeaderPath =
      context.config.commandLine.publicUmbrellaHeaderPath;
  if (!publicUmbrellaHeaderPath.empty()) {
    auto escapedString = Regex::escape(publicUmbrellaHeaderPath);
    Regex umbrellaRegex(escapedString);

    if (!matchFirstAndMoveToTop(publicHeaderFiles, umbrellaRegex)) {
      context.diag.report(diag::err_no_such_public_umbrella_header_file)
          << publicUmbrellaHeaderPath;
      return nullptr;
    }
  } else {
    auto frameworkName = sys::path::stem(framework.getName());
    auto umbrellaName = Regex::escape(frameworkName) + "\\.h";
    Regex umbrellaRegex(umbrellaName);

    matchFirstAndMoveToTop(publicHeaderFiles, umbrellaRegex);
  }

  auto &privateUmbrellaHeaderPath =
      context.config.commandLine.privateUmbrellaHeaderPath;
  if (!privateUmbrellaHeaderPath.empty()) {
    auto escapedString = Regex::escape(privateUmbrellaHeaderPath);
    Regex umbrellaRegex(escapedString);

    if (!matchFirstAndMoveToTop(privateHeaderFiles, umbrellaRegex)) {
      context.diag.report(diag::err_no_such_private_umbrella_header_file)
          << privateUmbrellaHeaderPath;
      return nullptr;
    }
  } else {
    auto frameworkName = sys::path::stem(framework.getName());
    auto umbrellaName = Regex::escape(frameworkName) + "[_]?Private\\.h";
    Regex umbrellaRegex(umbrellaName);

    matchFirstAndMoveToTop(privateHeaderFiles, umbrellaRegex);
  }

  auto sysroot = context.config.getSysRoot();
  findAndAddHeaderFiles(
      publicHeaderFiles, fm, context.diag,
      context.config.getExtraHeaders(basePath, HeaderType::Public), sysroot,
      basePath, diag::err_no_such_public_header_file);

  if (context.diag.hasErrorOccurred())
    return nullptr;

  findAndAddHeaderFiles(
      privateHeaderFiles, fm, context.diag,
      context.config.getExtraHeaders(basePath, HeaderType::Private), sysroot,
      basePath, diag::err_no_such_private_header_file);

  if (context.diag.hasErrorOccurred())
    return nullptr;

  job->publicHeaderFiles = std::move(publicHeaderFiles);
  job->privateHeaderFiles = std::move(privateHeaderFiles);

  // Add the current framework directory as a system framework directory. This
  // will prevent it from being droped from the top of the list if there is
  // a matching system framework include path.
  job->frameworkPaths.insert(job->frameworkPaths.begin(),
                             sys::path::parent_path(basePath).str());

  return job;
}

static bool scanFramework(Context &context, Framework &framework) {
  //
  // First scan all sub-frameworks, because we most likely will depend on them.
  //
  for (auto &F : framework._subFrameworks)
    if (!scanFramework(context, F))
      return false;

  //
  // Second scan all versions ...
  //
  for (auto &F : framework._versions)
    if (!scanFramework(context, F))
      return false;

  //
  // Now scan the actuall framework.
  //
  // First read the dylib, because they tell us the supported architectures.
  for (const auto &path : framework._dynamicLibraryFiles) {
    auto bufferOrErr = context.fm.getBufferForFile(path);
    if (auto ec = bufferOrErr.getError()) {
      context.diag.report(diag::err_cannot_read_file) << path << ec.message();
      return false;
    }

    auto file = context.registry.readFile(std::move(bufferOrErr.get()));
    if (!file) {
      context.diag.report(diag::err_cannot_read_file)
          << path << toString(file.takeError());
      return false;
    }

    auto *interface = cast<ExtendedInterfaceFile>(file.get().release());
    framework._interfaceFiles.emplace_back(interface);
  }

  // Now scan the header files.
  auto job = createJob(context, framework);
  if (job == nullptr)
    return false;

  if (auto symbols = Scanner::run(std::move(job))) {
    framework._headerSymbols = std::move(symbols);
    return true;
  }

  return false;
}

static bool generateAPI(Context &context, Options &opts, Framework &framework,
                        const Framework *parent = nullptr) {
  for (auto &F : framework._subFrameworks)
    if (!generateAPI(context, opts, F))
      return false;

  for (auto &F : framework._versions)
    if (!generateAPI(context, opts, F, &framework))
      return false;

  for (auto &interface : framework._interfaceFiles) {
    auto file = api::v1::createTextAPIFile(framework, interface.get());
    if (!file) {
      context.diag.report(diag::err_cannot_generate_stub);
      return false;
    }

    SmallString<PATH_MAX> path(interface->getPath());
    if (!opts.driverOptions.outputPath.empty()) {
      auto base = framework.getPath();
      assert(path.startswith(base) && "Unexpected base path for library");
      path = opts.driverOptions.outputPath;
      if (parent)
        path.append(StringRef(interface->getPath())
                        .drop_front(parent->getPath().size()));
      else
        path.append(StringRef(interface->getPath()).drop_front(base.size()));
    }

    // Create the directory in case it doesn't exist.
    SmallString<PATH_MAX> outputDir(path);
    sys::path::remove_filename(outputDir);
    if (auto ec = sys::fs::create_directories(outputDir)) {
      context.diag.report(diag::err_cannot_create_directory)
          << outputDir << ec.message();
      return false;
    }

    TAPI_INTERNAL::replace_extension(path, "api");
    if (!file->convertTo(FileType::API_V1, path)) {
      context.diag.report(diag::err_cannot_convert_dylib) << file->getPath();
      return false;
    }

    auto result = context.registry.writeFile(file.get());
    if (result) {
      context.diag.report(diag::err_cannot_write_file)
          << file->getPath() << toString(std::move(result));
      return false;
    }
    globalSnapshot->recordFile(file->getPath());

    TAPI_INTERNAL::replace_extension(path, "spi");
    if (!file->convertTo(FileType::SPI_V1, path)) {
      context.diag.report(diag::err_cannot_convert_dylib) << file->getPath();
      return false;
    }

    result = context.registry.writeFile(file.get());
    if (result) {
      context.diag.report(diag::err_cannot_write_file)
          << file->getPath() << toString(std::move(result));
      return false;
    }
    globalSnapshot->recordFile(file->getPath());
  }
  return true;
}

/// \brief Scan the directory for header and dynamic libraries and generate
///        API/SPI files.
bool Driver::Scan::run(DiagnosticsEngine &diag, Options &opts) {
  Context context(opts.getFileManager(), diag);
  auto &config = context.config.commandLine;

  // FIXME: Copy the options for now to reduce the amount of change.
  config.language = opts.frontendOptions.language;
  config.std = opts.frontendOptions.language_std;
  config.isysroot = opts.frontendOptions.isysroot;
  config.frameworkPaths = opts.frontendOptions.frameworkPaths;
  config.includePaths = opts.frontendOptions.includePaths;
  config.macros = opts.frontendOptions.macros;
  config.useRTTI = opts.frontendOptions.useRTTI;
  config.visibility = opts.frontendOptions.visibility;
  config.enableModules = opts.frontendOptions.enableModules;
  config.moduleCachePath = opts.frontendOptions.moduleCachePath;
  config.validateSystemHeaders = opts.frontendOptions.validateSystemHeaders;
  config.clangExtraArgs = opts.frontendOptions.clangExtraArgs;
  config.clangResourcePath = opts.frontendOptions.clangResourcePath;

  if (!opts.tapiOptions.configurationFile.empty()) {
    const auto *file = context.fm.getFile(opts.tapiOptions.configurationFile);
    if (!file) {
      diag.report(clang::diag::err_drv_no_such_file)
          << opts.tapiOptions.configurationFile;
      return false;
    }

    auto bufferOrErr = context.fm.getBufferForFile(file);
    if (auto ec = bufferOrErr.getError()) {
      diag.report(diag::err_cannot_read_file) << file->getName()
                                              << ec.message();
      return false;
    }

    auto configurationFile =
        context.registry.readFile(std::move(bufferOrErr.get()));
    if (!configurationFile) {
      diag.report(diag::err_cannot_read_file)
          << file->getName() << toString(configurationFile.takeError());
      return false;
    }

    auto *configuration =
        cast<ConfigurationFile>(configurationFile.get().get());
    context.config.setConfiguration(std::move(*configuration),
                                    opts.getFileManager(),
                                    opts.driverOptions.inputs.front(), diag);
  }

  // Handle extra header directories/files.
  config.extraPublicHeaders = opts.tapiOptions.extraPublicHeaders;
  config.extraPrivateHeaders = opts.tapiOptions.extraPrivateHeaders;

  // Handle excluded header files.
  config.excludePublicHeaders = opts.tapiOptions.excludePublicHeaders;
  config.excludePrivateHeaders = opts.tapiOptions.excludePrivateHeaders;

  // Handle disabling of header scanning.
  config.scanPublicHeaders = opts.tapiOptions.scanPublicHeaders;
  config.scanPrivateHeaders = opts.tapiOptions.scanPrivateHeaders;

  if (opts.driverOptions.inputs.empty()) {
    diag.report(clang::diag::err_drv_no_input_files);
    return false;
  }

  if (opts.driverOptions.inputs.size() != 1) {
    diag.report(diag::err_expected_one_input_file);
  }

  for (const auto &path : opts.driverOptions.inputs) {
    if (!context.addScanDirectory(path)) {
      diag.report(clang::diag::err_drv_no_such_file) << path;
      return false;
    }
  }

  // Handle public/private umbrella header.
  if (!opts.tapiOptions.publicUmbrellaHeaderPath.empty()) {
    if (!context.fm.exists(opts.tapiOptions.publicUmbrellaHeaderPath)) {
      diag.report(clang::diag::err_drv_no_such_file)
          << opts.tapiOptions.publicUmbrellaHeaderPath;
      return false;
    }
    config.publicUmbrellaHeaderPath = opts.tapiOptions.publicUmbrellaHeaderPath;
  }

  if (!opts.tapiOptions.privateUmbrellaHeaderPath.empty()) {
    if (!context.fm.exists(opts.tapiOptions.privateUmbrellaHeaderPath)) {
      diag.report(clang::diag::err_drv_no_such_file)
          << opts.tapiOptions.privateUmbrellaHeaderPath;
      return false;
    }
    config.privateUmbrellaHeaderPath =
        opts.tapiOptions.privateUmbrellaHeaderPath;
  }

  //
  // Scan through the directories and create a list of all found frameworks.
  //

  DirectoryScanner scanner(context.fm, diag);
  for (const auto &dir : context.directories())
    if (!scanner.scanDirectory(dir, context.frameworks))
      return false;

  //
  // Now scan the content of each framework.
  //

  for (auto &F : context.frameworks)
    if (!scanFramework(context, F))
      return false;

  bool warnAll = context.config.commandLine.scanPublicHeaders &&
                 context.config.commandLine.scanPrivateHeaders;
  for (auto &framework : context.frameworks)
    framework.verify(warnAll);

  // Only generate the API files when requested.
  if (!opts.tapiOptions.generateAPI)
    return true;

  for (auto &framework : context.frameworks)
    if (!generateAPI(context, opts, framework))
      return false;

  return true;
}

TAPI_NAMESPACE_INTERNAL_END
