//===- lib/Driver/InstallAPIDriver.cpp - TAPI Driver ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the driver for the tapi tool.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/HeaderFile.h"
#include "tapi/Core/InterfaceFileManager.h"
#include "tapi/Core/JSONFile.h"
#include "tapi/Core/Path.h"
#include "tapi/Core/Registry.h"
#include "tapi/Core/XPI.h"
#include "tapi/Defines.h"
#include "tapi/Driver/Diagnostics.h"
#include "tapi/Driver/DirectoryScanner.h"
#include "tapi/Driver/Driver.h"
#include "tapi/Driver/Options.h"
#include "tapi/Driver/Snapshot.h"
#include "tapi/LinkerInterfaceFile.h"
#include "tapi/Scanner/Scanner.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FileUtilities.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/Regex.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

using namespace llvm;
using namespace llvm::opt;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

static bool verifySymbols(const ExtendedInterfaceFile *apiFile,
                          const ExtendedInterfaceFile *dylibFile,
                          DiagnosticsEngine &diag,
                          VerificationMode verificationMode, bool demangle) {
  diag.setWarningsAsErrors(verificationMode == VerificationMode::Pedantic);

  auto xpiCmp = [](const XPI *lhs, const XPI *rhs) {
    if (lhs->getKind() < rhs->getKind())
      return true;
    if (rhs->getKind() < lhs->getKind())
      return false;

    return lhs->getName() < rhs->getName();
  };

  std::vector<const XPI *> symbols;
  for (const auto *symbol : apiFile->symbols())
    symbols.emplace_back(symbol);
  sort(symbols, xpiCmp);

  for (const auto *hsymbol : symbols) {
    const XPI *dsymbol = nullptr;
    bool haveSymbol =
        dylibFile->contains(hsymbol->getKind(), hsymbol->getName(), &dsymbol);

    if (verificationMode != VerificationMode::ErrorsOnly) {
      if (hsymbol->isUnavailable() && (haveSymbol))
        diag.report(diag::warn_symbol_unavailable)
            << hsymbol->getAnnotatedName(demangle);
    }

    if (!hsymbol->isExportedSymbol())
      continue;

    if (!haveSymbol) {
      diag.report(diag::err_library_missing_symbol)
          << hsymbol->getAnnotatedName(demangle);
      continue;
    }

    if (hsymbol->isThreadLocalValue() != dsymbol->isThreadLocalValue()) {
      if (hsymbol->isThreadLocalValue())
        diag.report(diag::err_header_symbol_tlv_mismatch)
            << hsymbol->getAnnotatedName(demangle)
            << dsymbol->getAnnotatedName(demangle);
      else
        diag.report(diag::err_dylib_symbol_tlv_mismatch)
            << dsymbol->getAnnotatedName(demangle)
            << hsymbol->getAnnotatedName(demangle);
    }

    if (hsymbol->isWeakDefined() != dsymbol->isWeakDefined()) {
      if (hsymbol->isWeakDefined())
        diag.report(diag::err_header_symbol_weak_mismatch)
            << hsymbol->getAnnotatedName(demangle)
            << dsymbol->getAnnotatedName(demangle);
      else
        diag.report(diag::err_dylib_symbol_weak_mismatch)
            << dsymbol->getAnnotatedName(demangle)
            << hsymbol->getAnnotatedName(demangle);
    }

    if (hsymbol->getArchitectures() == dsymbol->getArchitectures())
      continue;

    diag.report(diag::err_availability_mismatch)
        << hsymbol->getAnnotatedName(demangle) << hsymbol->getArchitectures()
        << dsymbol->getArchitectures();
  }

  // Check for all special linker symbols. They can affect the runtime behavior
  // and are always required to match even for ErrorsOnly mode.
  symbols.clear();
  for (const auto *symbol : dylibFile->exports())
    symbols.emplace_back(symbol);
  sort(symbols, xpiCmp);
  for (const auto *dsymbol : symbols) {
    // Skip normal symbols. We only care about special linker symbols here.
    if (!dsymbol->getName().startswith("$ld$"))
      continue;

    bool hasSymbol = apiFile->contains(dsymbol->getKind(), dsymbol->getName());
    if (hasSymbol)
      continue;

    diag.report(diag::err_header_symbol_missing)
        << dsymbol->getAnnotatedName(demangle);
  }

  if (verificationMode == VerificationMode::ErrorsOnly)
    return !diag.hasErrorOccurred();

  for (const auto *dsymbol : symbols) {
    // Skip special linker symbols. We already checked them.
    if (dsymbol->getName().startswith("$ld$"))
      continue;

    bool hasSymbol = apiFile->contains(dsymbol->getKind(), dsymbol->getName());
    if (hasSymbol)
      continue;

    // The existence of weak-defined RTTI can not always be inferred from the
    // header files, because they can be generated as part of an implementation
    // file.
    // We do not warn about weak-defined RTTI, because this doesn't affect
    // linking and can be ignored.
    if (dsymbol->isWeakDefined() && (dsymbol->getName().startswith("__ZTI") ||
                                     dsymbol->getName().startswith("__ZTS")))
      continue;

    // Do not warn about fragile ObjC classes. Even hidden classes are exported
    // and are required to be exported. They are not needed for linking, so we
    // can ignore them.
    if (dsymbol->getKind() == XPIKind::ObjectiveCClass &&
        dsymbol->getArchitectures() == Architecture::i386)
      continue;

    diag.report(diag::warn_header_symbol_missing)
        << dsymbol->getAnnotatedName(demangle);
  }

  return !diag.hasErrorOccurred();
}

static bool verifyFramework(const ExtendedInterfaceFile *apiFile,
                            const ExtendedInterfaceFile *dylibFile,
                            DiagnosticsEngine &diag,
                            VerificationMode verificationMode, bool demangle) {
  if (apiFile->getPlatform() != dylibFile->getPlatform()) {
    diag.report(diag::err_platform_mismatch) << apiFile->getPlatform()
                                             << dylibFile->getPlatform();
    return false;
  }

  if (apiFile->getArchitectures() != dylibFile->getArchitectures()) {
    diag.report(diag::err_architecture_mismatch)
        << apiFile->getArchitectures() << dylibFile->getArchitectures();
    return false;
  }

  if (apiFile->getInstallName() != dylibFile->getInstallName()) {
    diag.report(diag::err_install_name_mismatch) << apiFile->getInstallName()
                                                 << dylibFile->getInstallName();
    return false;
  }

  if (apiFile->getCurrentVersion() != dylibFile->getCurrentVersion()) {
    diag.report(diag::err_current_version_mismatch)
        << apiFile->getCurrentVersion() << dylibFile->getCurrentVersion();
    return false;
  }

  if (apiFile->getCompatibilityVersion() !=
      dylibFile->getCompatibilityVersion()) {
    diag.report(diag::err_compatibility_version_mismatch)
        << apiFile->getCompatibilityVersion()
        << dylibFile->getCompatibilityVersion();
    return false;
  }

  if (apiFile->isApplicationExtensionSafe() !=
      dylibFile->isApplicationExtensionSafe()) {
    diag.report(diag::err_appextension_safe_mismatch)
        << (apiFile->isApplicationExtensionSafe() ? "true" : "false")
        << (dylibFile->isApplicationExtensionSafe() ? "true" : "false");
    return false;
  }

  auto compareInterfaceFileRef = [&](const std::vector<InterfaceFileRef> &lhs,
                                     const std::vector<InterfaceFileRef> &rhs,
                                     unsigned diagID) {
    if (lhs.size() != rhs.size() || !equal(lhs, rhs)) {
      auto it1 = lhs.begin(), e1 = lhs.end();
      auto it2 = rhs.begin(), e2 = rhs.end();
      while (it1 != e1 || it2 != e2) {
        diag.report(diagID) << (it1 != e1 ? it1->getInstallName() : "<empty>")
                            << (it2 != e2 ? it2->getInstallName() : "<empty>");
        if (it1 != e1)
          ++it1;

        if (it2 != e2)
          ++it2;
      }
      return false;
    }
    return true;
  };

  if (!compareInterfaceFileRef(apiFile->reexportedLibraries(),
                               dylibFile->reexportedLibraries(),
                               diag::err_reexported_libraries_mismatch))
    return false;

  if (!compareInterfaceFileRef(apiFile->allowableClients(),
                               dylibFile->allowableClients(),
                               diag::err_allowable_clients_mismatch))
    return false;

  if (dylibFile->isTwoLevelNamespace() == false) {
    diag.report(diag::err_no_twolevel_namespace);
    return false;
  }

  if (dylibFile->getSwiftABIVersion() != 0) {
    diag.report(diag::err_swift_not_supported);
    return false;
  }

  return verifySymbols(apiFile, dylibFile, diag, verificationMode, demangle);
}

static Expected<std::unique_ptr<ExtendedInterfaceFile>>
getCodeCoverageSymbols(DiagnosticsEngine &diag, ArchitectureSet architectures,
                       Platform platform, std::string &osVersion,
                       std::string &isysroot) {
  static int staticSymbol;
  // Try to find clang first in the toolchain. If that fails, then fall-back to
  // the default search PATH.
  auto mainExecutable = sys::fs::getMainExecutable("tapi", &staticSymbol);
  StringRef toolchainBinDir = sys::path::parent_path(mainExecutable);
  auto clangBinary =
      sys::findProgramByName("clang", makeArrayRef(toolchainBinDir));
  if (clangBinary.getError()) {
    diag.report(diag::warn) << "cannot find 'clang' in toolchain directory. "
                               "Looking for 'clang' in PATH instead.";
    clangBinary = sys::findProgramByName("clang");
    if (auto ec = clangBinary.getError())
      return make_error<StringError>("unable to find 'clang' in PATH", ec);
  }

  // Create temporary input and output files.
  SmallString<PATH_MAX> inputFile;
  if (auto ec = sys::fs::createTemporaryFile("code_coverage", "c", inputFile))
    return make_error<StringError>("unable to create temporary input file", ec);
  FileRemover removeInputFile(inputFile);

  SmallString<PATH_MAX> outputFile;
  if (auto ec =
          sys::fs::createTemporaryFile("libcodecoverage", "dylib", outputFile))
    return make_error<StringError>("unable to create temporary output file",
                                   ec);
  FileRemover removeOutputFile(outputFile);

  std::error_code ec;
  raw_fd_ostream input(inputFile, ec, sys::fs::F_None);
  if (ec)
    return make_error<StringError>("cannot open input file", ec);
  input << "static int foo() { return 0; }\n";
  input.close();

  std::string deploymentTarget;
  switch (platform) {
  default:
    llvm_unreachable("Unexpected platform");
  case Platform::OSX:
    deploymentTarget = "-mmacosx-version-min=";
    break;
  case Platform::iOS:
    if (architectures.hasX86())
      deploymentTarget = "-mios-simulator-version-min=";
    else
      deploymentTarget = "-miphoneos-version-min=";
    break;
  case Platform::watchOS:
    if (architectures.hasX86())
      deploymentTarget = "-mwatchos-simulator-version-min=";
    else
      deploymentTarget = "-mwatchos-version-min=";
    break;
  case Platform::tvOS:
    if (architectures.hasX86())
      deploymentTarget = "-mtvos-simulator-version-min=";
    else
      deploymentTarget = "-mtvos-version-min=";
    break;
  case Platform::bridgeOS:
    deploymentTarget = "-mbridgeos-version-min=";
    break;
  }
  deploymentTarget += osVersion;

  Registry registry;
  registry.addBinaryReaders();

  std::string installDir = toolchainBinDir;
  std::vector<std::unique_ptr<ExtendedInterfaceFile>> files;
  for (auto arch : architectures) {
    std::string archName = getArchName(arch);
    const char *clangArgs[] = {"clang",
                               "-dynamiclib",
                               "-fprofile-instr-generate",
                               "-fcoverage-mapping",
                               "-ccc-install-dir",
                               installDir.c_str(),
                               "-isysroot",
                               isysroot.c_str(),
                               deploymentTarget.c_str(),
                               "-arch",
                               archName.c_str(),
                               "-o",
                               outputFile.c_str(),
                               inputFile.c_str(),
                               "-v",
                               nullptr};

    SmallString<PATH_MAX> stderrFile;
    if (auto ec = sys::fs::createTemporaryFile("stderr", "txt", stderrFile))
      return make_error<StringError>("unable to create temporary stderr file",
                                     ec);
    FileRemover removeStderrFile(stderrFile);

    StringRef stderrFileStr(stderrFile);
    const StringRef *redirects[] = {nullptr, nullptr, &stderrFileStr};
    bool failed = sys::ExecuteAndWait(clangBinary.get(), clangArgs,
                                      /*env=*/nullptr, redirects);

    if (failed) {
      auto bufferOr = MemoryBuffer::getFile(stderrFile);
      if (auto ec = bufferOr.getError())
        return make_error<StringError>("unable to read file", ec);

      std::string message = "'clang' invocation failed:\n";
      for (auto *arg : clangArgs) {
        if (arg == nullptr)
          continue;
        message.append(arg).append(1, ' ');
      }
      message.append(1, '\n');
      message.append(bufferOr.get()->getBuffer());

      return make_error<StringError>(
          message, std::make_error_code(std::errc::not_supported));
    }

    auto bufferOr = MemoryBuffer::getFile(outputFile.str());
    if (auto ec = bufferOr.getError())
      return make_error<StringError>("unable to read file", ec);
    auto file = registry.readFile(std::move(bufferOr.get()));
    if (!file)
      return file.takeError();
    files.emplace_back(cast<ExtendedInterfaceFile>(file.get().release()));
  }

  // Merge all the interface files into one.
  std::unique_ptr<ExtendedInterfaceFile> output;
  for (auto &file : files) {
    if (!output) {
      output = std::move(file);
      continue;
    }

    auto result = output->merge(file.get());
    if (!result)
      return result.takeError();
    output = std::move(result.get());
  }

  return std::move(output);
}

/// \brief Parses the headers and generate a text-based stub file.
bool Driver::InstallAPI::run(DiagnosticsEngine &diag, Options &opts) {
  auto &fm = opts.getFileManager();

  // Handle architectures.
  if (opts.linkerOptions.architectures.empty()) {
    diag.report(diag::err_no_architecture);
    return false;
  }

  // Handle install name.
  if (opts.linkerOptions.installName.empty()) {
    diag.report(diag::err_no_install_name);
    return false;
  }

  SmallString<PATH_MAX> name =
      sys::path::filename(opts.linkerOptions.installName);
  sys::path::replace_extension(name, "");
  globalSnapshot->setName(name);

  // Handle platform.
  if (opts.frontendOptions.platform == Platform::Unknown) {
    diag.report(diag::err_no_deployment_target);
    return false;
  }

  diag.setErrorLimit(opts.diagnosticsOptions.errorLimit);

  auto job = make_unique<ParsingJob>();
  job->fileManager = &fm;
  job->architectures = opts.linkerOptions.architectures;
  job->platform = opts.frontendOptions.platform;
  job->osVersion = opts.frontendOptions.osVersion;
  job->language = opts.frontendOptions.language;
  job->language_std = opts.frontendOptions.language_std;
  job->useRTTI = opts.frontendOptions.useRTTI;
  job->visibility = opts.frontendOptions.visibility;
  job->isysroot = opts.frontendOptions.isysroot;
  job->serializeDiagnosticsFile =
      opts.diagnosticsOptions.serializeDiagnosticsFile;
  job->macros = opts.frontendOptions.macros;
  job->systemFrameworkPaths = opts.frontendOptions.systemFrameworkPaths;
  job->systemIncludePaths = opts.frontendOptions.systemIncludePaths;
  job->frameworkPaths = opts.frontendOptions.frameworkPaths;
  job->includePaths = opts.frontendOptions.includePaths;
  job->clangExtraArgs = opts.frontendOptions.clangExtraArgs;
  job->enableModules = opts.frontendOptions.enableModules;
  job->moduleCachePath = opts.frontendOptions.moduleCachePath;
  job->validateSystemHeaders = opts.frontendOptions.validateSystemHeaders;
  job->clangResourcePath = opts.frontendOptions.clangResourcePath;
  job->useObjectiveCARC = opts.frontendOptions.useObjectiveCARC;
  job->useObjectiveCWeakARC = opts.frontendOptions.useObjectiveCWeakARC;

  if (opts.driverOptions.inputs.empty()) {
    diag.report(clang::diag::err_drv_no_input_files);
    return false;
  }

  const FileEntry *jsonFile = nullptr;
  for (const auto &path : opts.driverOptions.inputs) {
    if (sys::path::extension(path) == ".json") {
      if (auto *file = fm.getFile(path)) {
        jsonFile = file;
        break;
      }
    }
  }

  if (jsonFile) {
    if (!opts.tapiOptions.publicUmbrellaHeaderPath.empty())
      diag.report(diag::warn_ignore_option_json) << "--public-umbrella-header";

    if (!opts.tapiOptions.privateUmbrellaHeaderPath.empty())
      diag.report(diag::warn_ignore_option_json) << "--private-umbrella-header";
  }

  if (jsonFile == nullptr) {
    //
    // Scan through the directories and create a list of all found frameworks.
    //
    DirectoryScanner scanner(fm, diag);
    std::vector<Framework> frameworks;
    for (const auto &path : opts.driverOptions.inputs) {
      if (fm.isDirectory(path, /*CacheFailure=*/false)) {
        if (!scanner.scanDirectory(path, frameworks,
                                   opts.linkerOptions.isDynamicLibrary))
          return false;
      } else {
        diag.report(diag::err_no_directory) << path;
        return false;
      }
    }

    if (frameworks.empty()) {
      diag.report(diag::err_no_framework);
      return false;
    }

    if (frameworks.size() > 1) {
      diag.report(diag::err_more_than_one_framework);
      return false;
    }

    auto *framework = &frameworks.back();
    if (!framework->_versions.empty())
      framework = &framework->_versions.back();

    for (const auto &header : framework->_headerFiles) {
      auto *file = fm.getFile(header.first);
      if (!file) {
        if (header.second == HeaderType::Private)
          diag.report(diag::err_no_such_private_header_file) << header.first;
        else
          diag.report(diag::err_no_such_public_header_file) << header.first;
        return false;
      }
      if (header.second == HeaderType::Private)
        job->privateHeaderFiles.emplace_back(file);
      else
        job->publicHeaderFiles.emplace_back(file);
    }

    sort(job->publicHeaderFiles,
         [](const FileEntry *lhs, const FileEntry *rhs) {
           return StringRef(lhs->getName()) < StringRef(rhs->getName());
         });
    sort(job->privateHeaderFiles,
         [](const FileEntry *lhs, const FileEntry *rhs) {
           return StringRef(lhs->getName()) < StringRef(rhs->getName());
         });

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

    const auto &publicUmbrellaHeaderPath =
        opts.tapiOptions.publicUmbrellaHeaderPath;
    if (!publicUmbrellaHeaderPath.empty()) {
      auto escapedString = Regex::escape(publicUmbrellaHeaderPath);
      Regex umbrellaRegex(escapedString);

      if (!matchFirstAndMoveToTop(job->publicHeaderFiles, umbrellaRegex)) {
        diag.report(diag::err_no_such_public_umbrella_header_file)
            << publicUmbrellaHeaderPath;
        return false;
      }
    } else {
      auto frameworkName = sys::path::stem(framework->getName());
      auto umbrellaName = Regex::escape(frameworkName) + "\\.h";
      Regex umbrellaRegex(umbrellaName);

      matchFirstAndMoveToTop(job->publicHeaderFiles, umbrellaRegex);
    }

    const auto &privateUmbrellaHeaderPath =
        opts.tapiOptions.privateUmbrellaHeaderPath;
    if (!privateUmbrellaHeaderPath.empty()) {
      auto escapedString = Regex::escape(privateUmbrellaHeaderPath);
      Regex umbrellaRegex(escapedString);

      if (!matchFirstAndMoveToTop(job->privateHeaderFiles, umbrellaRegex)) {
        diag.report(diag::err_no_such_private_umbrella_header_file)
            << privateUmbrellaHeaderPath;
        return false;
      }
    } else {
      auto frameworkName = sys::path::stem(framework->getName());
      auto umbrellaName = Regex::escape(frameworkName) + "[_]?Private\\.h";
      Regex umbrellaRegex(umbrellaName);

      matchFirstAndMoveToTop(job->privateHeaderFiles, umbrellaRegex);
    }
  } else {
    Registry registry;
    auto reader = make_unique<YAMLReader>();
    reader->add(std::unique_ptr<DocumentHandler>(
        new json_file::v1::YAMLDocumentHandler));
    registry.add(std::move(reader));

    auto bufferOr = fm.getBufferForFile(jsonFile);
    if (auto ec = bufferOr.getError()) {
      diag.report(diag::err_cannot_read_file)
          << jsonFile->getName() << ec.message();
      return false;
    }

    auto file = registry.readFile(std::move(bufferOr.get()));
    if (!file) {
      diag.report(diag::err_cannot_read_file)
          << jsonFile->getName() << toString(file.takeError());
      return false;
    }
    auto *json = cast<JSONFile>(file.get().get());

    for (const auto &header : json->headers) {
      auto *file = fm.getFile(header.path);
      if (file == nullptr) {
        if (header.type == HeaderType::Public)
          diag.report(diag::err_no_such_public_header_file) << header.path;
        else
          diag.report(diag::err_no_such_private_header_file) << header.path;
        return false;
      }
      if (header.type == HeaderType::Public)
        job->publicHeaderFiles.emplace_back(file);
      else
        job->privateHeaderFiles.emplace_back(file);
    }
  }

  std::vector<const FileEntry *> extraPublicHeaders;
  std::vector<const FileEntry *> extraPrivateHeaders;
  std::set<const FileEntry *> excludePublicHeaders;
  std::set<const FileEntry *> excludePrivateHeaders;

  for (const auto &path : opts.tapiOptions.extraPublicHeaders) {
    if (const auto *file = fm.getFile(path)) {
      extraPublicHeaders.emplace_back(file);
    } else {
      diag.report(diag::err_no_such_public_header_file) << path;
      return false;
    }
  }

  for (const auto &path : opts.tapiOptions.extraPrivateHeaders) {
    if (const auto *file = fm.getFile(path)) {
      extraPrivateHeaders.emplace_back(file);
    } else {
      diag.report(diag::err_no_such_private_header_file) << path;
      return false;
    }
  }

  for (const auto &path : opts.tapiOptions.excludePublicHeaders) {
    if (const auto *file = fm.getFile(path)) {
      excludePublicHeaders.insert(file);
    } else {
      diag.report(diag::err_no_such_excluded_public_header_file) << path;
      return false;
    }
  }

  for (const auto &path : opts.tapiOptions.excludePrivateHeaders) {
    if (const auto *file = fm.getFile(path)) {
      excludePrivateHeaders.insert(file);
    } else {
      diag.report(diag::err_no_such_excluded_private_header_file) << path;
      return false;
    }
  }

  // Add extra header files
  job->publicHeaderFiles.insert(job->publicHeaderFiles.end(),
                                extraPublicHeaders.begin(),
                                extraPublicHeaders.end());
  job->privateHeaderFiles.insert(job->privateHeaderFiles.end(),
                                 extraPrivateHeaders.begin(),
                                 extraPrivateHeaders.end());

  {
    auto it = remove_if(job->publicHeaderFiles,
                        [&excludePublicHeaders](const FileEntry *file) {
                          return excludePublicHeaders.count(file);
                        });
    job->publicHeaderFiles.erase(it, job->publicHeaderFiles.end());
  }

  {
    auto it = remove_if(job->privateHeaderFiles,
                        [&excludePrivateHeaders](const FileEntry *file) {
                          return excludePrivateHeaders.count(file);
                        });
    job->privateHeaderFiles.erase(it, job->privateHeaderFiles.end());
  }

  // Infer additional include paths.
  std::set<std::string> inferredIncludePaths;
  for (const auto *file : job->publicHeaderFiles)
    inferredIncludePaths.insert(file->getDir()->getName());

  for (const auto *file : job->privateHeaderFiles)
    inferredIncludePaths.insert(file->getDir()->getName());

  job->includePaths.insert(job->includePaths.end(),
                           inferredIncludePaths.begin(),
                           inferredIncludePaths.end());

  auto headerSymbols = Scanner::run(std::move(job));
  if (!headerSymbols)
    return false;

  auto scanFile = make_unique<ExtendedInterfaceFile>(std::move(headerSymbols));
  scanFile->setFileType(FileType::TBD_V2);
  scanFile->setPlatform(opts.frontendOptions.platform);
  scanFile->setArchitectures(opts.linkerOptions.architectures);
  scanFile->setInstallName(opts.linkerOptions.installName);
  scanFile->setCurrentVersion(opts.linkerOptions.currentVersion);
  scanFile->setCompatibilityVersion(opts.linkerOptions.compatibilityVersion);
  scanFile->setTwoLevelNamespace();
  scanFile->setApplicationExtensionSafe(
      opts.linkerOptions.isApplicationExtensionSafe);
  scanFile->setInstallAPI();
  for (const auto &lib : opts.linkerOptions.allowableClients)
    scanFile->addAllowableClient(lib.getInstallName(), lib.getArchitectures());
  for (const auto &lib : opts.linkerOptions.reexportedLibraries)
    scanFile->addReexportedLibrary(lib.getInstallName(),
                                   lib.getArchitectures());

  // When code coverage is enabled we need to generate extra symbols manually.
  // These symbols are defined in libclang_rt.profile_*.a and are pulled in by
  // clang when -fprofile-instr-generate is specified on the command line.
  if (opts.tapiOptions.generateCodeCoverageSymbols) {
    auto file = getCodeCoverageSymbols(
        diag, opts.linkerOptions.architectures, opts.frontendOptions.platform,
        opts.frontendOptions.osVersion, opts.frontendOptions.isysroot);
    if (!file) {
      diag.report(diag::err) << "could not generate coverage symbols"
                             << toString(file.takeError());
      return false;
    }
    for (const auto *symbol : file.get()->exports())
      scanFile->addSymbol(symbol->getKind(), symbol->getName(),
                          symbol->getArchitectures(), symbol->getSymbolFlags(),
                          symbol->getAccess());
  }

  InterfaceFileManager manager(fm);
  if (!opts.tapiOptions.verifyAgainst.empty()) {
    auto file = manager.readFile(opts.tapiOptions.verifyAgainst);
    if (!file) {
      diag.report(diag::err_cannot_read_file)
          << opts.tapiOptions.verifyAgainst << toString(file.takeError());
      return false;
    }

    auto *dylib = cast<ExtendedInterfaceFile>(file.get());
    if (!verifyFramework(scanFile.get(), dylib, diag,
                         opts.tapiOptions.verificationMode,
                         opts.tapiOptions.demangle))
      return false;

    // Clear the installapi flag.
    scanFile->setInstallAPI(false);

    // Record the UUIDs from the dynamic library.
    if (opts.tapiOptions.recordUUIDs)
      for (auto &uuid : dylib->uuids())
        scanFile->addUUID(uuid.first, uuid.second);
  }

  if (opts.driverOptions.outputPath.empty()) {
    SmallString<PATH_MAX> path;
    if (auto ec = sys::fs::current_path(path)) {
      diag.report(diag::err) << path << ec.message();
      return false;
    }
    auto targetName = sys::path::stem(opts.linkerOptions.installName);
    sys::path::append(path, targetName);
    TAPI_INTERNAL::replace_extension(path, ".tbd");
    opts.driverOptions.outputPath = path.str();
  }

  SmallString<PATH_MAX> outputDir(opts.driverOptions.outputPath);
  sys::path::remove_filename(outputDir);
  auto ec = sys::fs::create_directories(outputDir);
  if (ec) {
    diag.report(diag::err_cannot_create_directory) << outputDir << ec.message();
    return false;
  }

  scanFile->setPath(opts.driverOptions.outputPath);
  auto interface = make_unique<InterfaceFile>(std::move(*scanFile.get()));
  auto result = manager.writeFile(interface.get());
  if (result) {
    diag.report(diag::err_cannot_write_file)
        << interface->getPath() << toString(std::move(result));
    return false;
  }
  globalSnapshot->recordFile(interface->getPath());

  return true;
}

TAPI_NAMESPACE_INTERNAL_END
