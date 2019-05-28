//===- lib/Driver/StubDriver.cpp - TAPI Stub Driver -------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the stub driver for the tapi tool.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/FileSystem.h"
#include "tapi/Core/Path.h"
#include "tapi/Core/Registry.h"
#include "tapi/Core/Utils.h"
#include "tapi/Defines.h"
#include "tapi/Diagnostics/Diagnostics.h"
#include "tapi/Driver/Driver.h"
#include "tapi/Driver/Options.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/FileSystem.h"
#include <string>

using namespace llvm;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

// Stub Driver Context.
namespace {
struct Context {
  Context(FileManager &fm, DiagnosticsEngine &diag) : fm(fm), diag(diag) {
    registry.addBinaryReaders();
    registry.addYAMLReaders();
    registry.addYAMLWriters();
  }

  Context(const Context &) = delete;

  bool deleteInputFile = false;
  bool inlinePrivateFrameworks = false;
  bool deletePrivateFrameworks = false;
  bool recordUUIDs = true;
  bool setInstallAPIFlag = false;

  std::string sysroot;
  std::string inputPath;
  std::string outputPath;
  PathSeq searchPaths;
  PathSeq librarySearchPaths;
  PathSeq frameworkSearchPaths;
  Registry registry;
  FileManager &fm;
  DiagnosticsEngine &diag;
  FileType fileType;

  std::map<std::string, std::string> normalizedPathToVarName;
};
} // namespace

namespace detail {
struct SymlinkInfo {
  std::string srcPath;
  std::string symlinkContent;

  SymlinkInfo(std::string path, std::string link)
      : srcPath(std::move(path)), symlinkContent(std::move(link)) {}
};
} // end namespace detail.

static bool isPrivatePath(StringRef path, bool isSymlink = false) {
  if (path.startswith("/System/Library/PrivateFrameworks"))
    return true;

  if (path.startswith("/usr/lib") && sys::path::parent_path(path) != "/usr/lib")
    return true;

  // /System/Library/Frameworks/ is a public location
  if (path.startswith("/System/Library/Frameworks/")) {
    StringRef name, rest;
    std::tie(name, rest) =
        path.drop_front(sizeof("/System/Library/Frameworks")).split('.');

    // but only top level framework
    // /System/Library/Frameworks/Foo.framework/Foo ==> true
    // /System/Library/Frameworks/Foo.framework/Versions/A/Foo ==> true
    // /System/Library/Frameworks/Foo.framework/Resources/libBar.dylib ==> false
    // /System/Library/Frameworks/Foo.framework/Frameworks/Bar.framework/Bar
    // ==> false
    // /System/Library/Frameworks/Foo.framework/Frameworks/Xfoo.framework/XFoo
    // ==> false
    if (rest.startswith("framework/") &&
        (rest.endswith(name) || rest.endswith((name + ".tbd").str()) ||
         (isSymlink && rest.endswith("Current"))))
      return false;

    return true;
  }

  return false;
}

static bool inlineFrameworks(Context &ctx, InterfaceFile *dylib) {
  PathSeq toDelete;
  std::vector<std::pair<std::string, ArchitectureSet>> toAdd;
  auto &reexports = dylib->reexportedLibraries();
  for (auto &lib : reexports) {
    if (isPublicLocation(lib.getInstallName()))
      continue;

    if (lib.getInstallName().startswith("@"))
      continue;

    auto path =
        findLibrary(lib.getInstallName(), ctx.fm, ctx.frameworkSearchPaths,
                    ctx.librarySearchPaths, ctx.searchPaths);
    if (path.empty()) {
      ctx.diag.report(diag::err_cannot_find_reexport) << lib.getInstallName();
      return false;
    }

    auto bufferOrError = ctx.fm.getBufferForFile(path);
    if (auto ec = bufferOrError.getError()) {
      ctx.diag.report(diag::err_cannot_read_file) << path << ec.message();
      return false;
    }

    auto file = ctx.registry.readFile(std::move(bufferOrError.get()),
                                      ReadFlags::Symbols);
    if (!file) {
      ctx.diag.report(diag::err_cannot_read_file)
          << path << toString(file.takeError());
      return false;
    }

    std::shared_ptr<InterfaceFile> reexportedDylib;
    auto *file2 = file.get().release();
    if (auto *extended = dyn_cast<ExtendedInterfaceFile>(file2))
      reexportedDylib = make_unique<InterfaceFile>(std::move(*extended));
    else
      reexportedDylib =
          std::unique_ptr<InterfaceFile>(cast<InterfaceFile>(file2));

    if (!inlineFrameworks(ctx, reexportedDylib.get()))
      return false;

    if ((dylib->getPlatform() != reexportedDylib->getPlatform())) {
      ctx.diag.report(diag::err_property_mismatch)
          << "platform" << dylib->getPath() << reexportedDylib->getPath();
      return false;
    }

    if (ctx.fileType != FileType::TBD_V3) {
      if (dylib->getSwiftABIVersion() !=
          reexportedDylib->getSwiftABIVersion()) {
        ctx.diag.report(diag::err_property_mismatch)
            << "Swift ABI version" << dylib->getPath()
            << reexportedDylib->getPath();
        return false;
      }

      if (!reexportedDylib->isTwoLevelNamespace()) {
        ctx.diag.report(diag::err_property_mismatch)
            << "twolevel namespace" << dylib->getPath()
            << reexportedDylib->getPath();
        return false;
      }

      std::map<Architecture, Architecture> overrideMap;
      for (auto arch : lib.getArchitectures()) {
        if (reexportedDylib->getArchitectures().has(arch))
          continue;

        if (reexportedDylib->getArchitectures().hasABICompatibleSlice(arch)) {
          auto compArch =
              reexportedDylib->getArchitectures().getABICompatibleSlice(arch);
          overrideMap[compArch] = arch;

          continue;
        }

        ctx.diag.report(diag::err_not_all_architectures)
            << reexportedDylib->getPath();
        return false;
      }

      for (const auto &lib : reexportedDylib->reexportedLibraries()) {
        auto archs = lib.getArchitectures();
        for (auto arch : lib.getArchitectures())
          if (overrideMap.count(arch))
            archs.set(overrideMap[arch]);
        toAdd.emplace_back(lib.getInstallName(), archs);
      }

      for (const auto *symbol : reexportedDylib->exports()) {
        ArchitectureSet newArchs;
        for (auto arch : symbol->getArchitectures()) {
          newArchs.set(arch);
          if (overrideMap.count(arch))
            newArchs.set(overrideMap[arch]);
        }
        dylib->addSymbol(symbol->getKind(), symbol->getName(), newArchs,
                         symbol->getFlags());
      }

      toDelete.emplace_back(lib.getInstallName());
    } else {
      if (!reexportedDylib->convertTo(ctx.fileType)) {
        ctx.diag.report(diag::err_cannot_convert_dylib)
            << reexportedDylib->getPath();
        return false;
      }
      // Clear InstallAPI flag.
      reexportedDylib->setInstallAPI(false);
      dylib->inlineFramework(reexportedDylib);
    }
  }

  for (auto &name : toDelete)
    dylib->removeReexportedLibrary(name);

  for (const auto &it : toAdd)
    dylib->addReexportedLibrary(it.first, it.second);

  return true;
}

static bool stubifyDynamicLibrary(Context &ctx) {
  const auto *inputFile = ctx.fm.getFile(ctx.inputPath);
  if (!inputFile) {
    ctx.diag.report(clang::diag::err_drv_no_such_file) << ctx.inputPath;
    return false;
  }
  auto bufferOrErr = ctx.fm.getBufferForFile(inputFile);
  if (auto ec = bufferOrErr.getError()) {
    ctx.diag.report(diag::err_cannot_read_file)
        << inputFile->getName() << ec.message();
    return false;
  }

  // Is the input file a dynamic library?
  if (!ctx.registry.canRead(
          bufferOrErr.get()->getMemBufferRef(),
          FileType::MachO_DynamicLibrary | FileType::MachO_DynamicLibrary_Stub |
              FileType::TBD_V1 | FileType::TBD_V2 | FileType::TBD_V3)) {
    ctx.diag.report(diag::err_not_a_dylib) << inputFile->getName();
    return false;
  }

  auto file =
      ctx.registry.readFile(std::move(bufferOrErr.get()), ReadFlags::Symbols);
  if (!file) {
    ctx.diag.report(diag::err_cannot_read_file)
        << ctx.inputPath << toString(file.takeError());
    return false;
  }

  std::unique_ptr<InterfaceFile> interface;
  auto *file2 = file.get().release();
  if (auto *extended = dyn_cast<ExtendedInterfaceFile>(file2))
    interface = make_unique<InterfaceFile>(std::move(*extended));
  else
    interface = std::unique_ptr<InterfaceFile>(cast<InterfaceFile>(file2));

  auto *dylib = interface.get();
  if (!dylib->convertTo(ctx.fileType)) {
    ctx.diag.report(diag::err_cannot_convert_dylib) << dylib->getPath();
    return false;
  }

  if (ctx.inlinePrivateFrameworks) {
    if (!inlineFrameworks(ctx, dylib))
      return false;
  }

  if (!ctx.recordUUIDs)
    dylib->clearUUIDs();

  dylib->setInstallAPI(ctx.setInstallAPIFlag);

  if (auto result = ctx.registry.writeFile(dylib, ctx.outputPath)) {
    ctx.diag.report(diag::err_cannot_write_file)
        << ctx.outputPath << toString(std::move(result));
    return false;
  }

  if (ctx.deleteInputFile) {
    inputFile->closeFile();
    ctx.fm.invalidateCache(inputFile);
    if (auto ec = sys::fs::remove(ctx.inputPath)) {
      ctx.diag.report(diag::err) << ctx.inputPath << ec.message();
      return false;
    }
  }

  return true;
}

/// \brief Converts all dynamic libraries/frameworks to text-based stubs if
/// possible. Also create the same symlinks as the ones that pointed to the
/// orignal library. If requested the source library will be deleted.
///
/// inputPath is the canonical path - no symlinks and no path relative elements.
static bool stubifyDirectory(Context &ctx) {
  assert(ctx.inputPath.back() != '/' && "Unexpected / at end of input path.");

  std::map<std::string, std::vector<detail::SymlinkInfo>> symlinks;
  std::map<std::string, std::unique_ptr<InterfaceFile>> dylibs;
  std::map<std::string, std::string> originalNames;
  std::set<std::pair<std::string, bool>> toDelete;
  std::error_code ec;
  for (sys::fs::recursive_directory_iterator i(ctx.inputPath, ec), ie; i != ie;
       i.increment(ec)) {

    if (ec == std::errc::no_such_file_or_directory) {
      ctx.diag.report(diag::err) << i->path() << ec.message();
      continue;
    }

    if (ec) {
      ctx.diag.report(diag::err) << i->path() << ec.message();
      return false;
    }

    // Skip header directories (include/Headers/PrivateHeaders) and module
    // files.
    StringRef path = i->path();
    if (path.endswith("/include") || path.endswith("/Headers") ||
        path.endswith("/PrivateHeaders") || path.endswith("/Modules") ||
        path.endswith(".map") || path.endswith(".modulemap")) {
      i.no_push();
      continue;
    }

    // Check if the entry is a symlink. We don't follow symlinks, but we record
    // their content.
    bool isSymlink;
    if (auto ec = sys::fs::is_symlink_file(path, isSymlink)) {
      ctx.diag.report(diag::err) << path << ec.message();
      return false;
    }

    if (isSymlink) {
      // Don't follow symlink.
      i.no_push();

      bool shouldSkip;
      auto ec2 = shouldSkipSymlink(path, shouldSkip);
      if (ec2 == std::errc::no_such_file_or_directory) {
        ctx.diag.report(diag::warn_broken_symlink) << path;
        continue;
      }

      if (ec2) {
        ctx.diag.report(diag::err) << path << ec2.message();
        return false;
      }

      if (shouldSkip)
        continue;

      if (ctx.deletePrivateFrameworks &&
          isPrivatePath(path.drop_front(ctx.inputPath.size()), true)) {
        toDelete.emplace(path, false);
        continue;
      }

      SmallString<PATH_MAX> symlinkPath;
      if (auto ec = read_link(path, symlinkPath)) {
        ctx.diag.report(diag::err_cannot_read_file) << path << ec.message();
        return false;
      }

      // Some projects use broken symlinks that are absulte paths, which are
      // invalid during build time, but would be correct during runtime. In the
      // case of an absolute path we should check first if the path exist with
      // the SDKContentRoot as prefix.
      SmallString<PATH_MAX> linkSrc = path;
      SmallString<PATH_MAX> linkTarget;
      if (sys::path::is_absolute(symlinkPath)) {
        linkTarget = ctx.inputPath;
        sys::path::append(linkTarget, symlinkPath);

        if (ctx.fm.exists(linkTarget)) {
          // Convert the aboslute path to an relative path.
          if (auto ec = make_relative(linkSrc, linkTarget, symlinkPath)) {
            ctx.diag.report(diag::err) << linkTarget << ec.message();
            return false;
          }
        } else if (!ctx.fm.exists(symlinkPath)) {
          ctx.diag.report(diag::warn_broken_symlink) << path;
          continue;
        } else {
          linkTarget = symlinkPath;
        }
      } else {
        linkTarget = linkSrc;
        sys::path::remove_filename(linkTarget);
        sys::path::append(linkTarget, symlinkPath);
      }

      // The symlink src is garantueed to be a canonical path, because we don't
      // follow symlinks when scanning the SDK. The symlink target is
      // constructed from the symlink path and need to be canonicalized.
      if (auto ec = realpath(linkTarget)) {
        ctx.diag.report(diag::err) << linkTarget << ec.message();
        return false;
      }

      auto itr = symlinks.emplace(
          std::piecewise_construct, std::forward_as_tuple(linkTarget.c_str()),
          std::forward_as_tuple(std::vector<detail::SymlinkInfo>()));
      itr.first->second.emplace_back(linkSrc.str(), symlinkPath.c_str());

      continue;
    }

    // We only have to look at files.
    auto *file = ctx.fm.getFile(path);
    if (!file)
      continue;

    if (ctx.deletePrivateFrameworks &&
        isPrivatePath(path.drop_front(ctx.inputPath.size()))) {
      i.no_push();
      toDelete.emplace(path, false);
      continue;
    }

    auto bufferOrErr = ctx.fm.getBufferForFile(file);
    if (auto ec = bufferOrErr.getError()) {
      ctx.diag.report(diag::err_cannot_read_file) << path << ec.message();
      return false;
    }

    // Check for dynamic libs and text-based stub files.
    if (!ctx.registry.canRead(bufferOrErr.get()->getMemBufferRef(),
                              FileType::MachO_DynamicLibrary |
                                  FileType::MachO_DynamicLibrary_Stub |
                                  FileType::TBD_V1 | FileType::TBD_V2 |
                                  FileType::TBD_V3))
      continue;

    auto file2 =
        ctx.registry.readFile(std::move(bufferOrErr.get()), ReadFlags::Symbols);
    if (!file2) {
      ctx.diag.report(diag::err_cannot_read_file)
          << path << toString(file2.takeError());
      return false;
    }

    std::unique_ptr<InterfaceFile> interface;
    auto *file3 = file2.get().release();
    if (auto *extended = dyn_cast<ExtendedInterfaceFile>(file3))
      interface = make_unique<InterfaceFile>(std::move(*extended));
    else
      interface = std::unique_ptr<InterfaceFile>(cast<InterfaceFile>(file3));

    // Normalize path for map lookup by removing the extension.
    SmallString<PATH_MAX> normalizedPath(path);
    TAPI_INTERNAL::replace_extension(normalizedPath, "");

    if ((interface->getFileType() == FileType::MachO_DynamicLibrary) ||
        (interface->getFileType() == FileType::MachO_DynamicLibrary_Stub)) {
      originalNames[normalizedPath.c_str()] = interface->getPath();

      // Don't add this MachO dynamic library, because we already have a
      // text-based stub recorded for this path.
      if (dylibs.count(normalizedPath.c_str()))
        continue;
    }

    // FIXME: Once we use C++17, this can be simplified.
    auto it = dylibs.find(normalizedPath.c_str());
    if (it != dylibs.end())
      it->second = std::move(interface);
    else
      dylibs.emplace(std::piecewise_construct,
                     std::forward_as_tuple(normalizedPath.c_str()),
                     std::forward_as_tuple(std::move(interface)));
  }

  for (auto &it : dylibs) {
    auto &dylib = it.second;
    SmallString<PATH_MAX> input(dylib->getPath());
    SmallString<PATH_MAX> output = input;
    TAPI_INTERNAL::replace_extension(output, ".tbd");

    if (!dylib->convertTo(ctx.fileType)) {
      ctx.diag.report(diag::err_cannot_convert_dylib) << dylib->getPath();
      return false;
    }

    if (ctx.inlinePrivateFrameworks) {
      if (!inlineFrameworks(ctx, dylib.get()))
        return false;
    }

    if (!ctx.recordUUIDs)
      dylib->clearUUIDs();

    dylib->setInstallAPI(ctx.setInstallAPIFlag);

    auto result = ctx.registry.writeFile(dylib.get(), output.str());
    if (result) {
      ctx.diag.report(diag::err_cannot_write_file)
          << output << toString(std::move(result));
      return false;
    }

    // Get the original file name.
    SmallString<PATH_MAX> normalizedPath(dylib->getPath());
    TAPI_INTERNAL::replace_extension(normalizedPath, "");
    auto it2 = originalNames.find(normalizedPath.c_str());
    if (it2 == originalNames.end())
      continue;
    auto originalName = it2->second;

    if (ctx.deleteInputFile)
      toDelete.emplace(originalName, true);

    // Don't allow for more than 20 levels of symlinks.
    StringRef toCheck = originalName;
    for (int i = 0; i < 20; ++i) {
      auto itr = symlinks.find(toCheck);
      if (itr != symlinks.end()) {
        for (auto &symInfo : itr->second) {
          SmallString<PATH_MAX> linkSrc(symInfo.srcPath);
          SmallString<PATH_MAX> linkTarget(symInfo.symlinkContent);
          TAPI_INTERNAL::replace_extension(linkSrc, "tbd");
          TAPI_INTERNAL::replace_extension(linkTarget, "tbd");

          if (auto ec = sys::fs::remove(linkSrc)) {
            ctx.diag.report(diag::err) << linkSrc << ec.message();
            return false;
          }

          if (auto ec = sys::fs::create_link(linkTarget, linkSrc)) {
            ctx.diag.report(diag::err) << linkTarget << ec.message();
            return false;
          }

          if (ctx.deleteInputFile)
            toDelete.emplace(symInfo.srcPath, true);

          toCheck = symInfo.srcPath;
        }
      } else
        break;
    }
  }

  // Recursively delete the directories (this will abort when they are not empty
  // or we reach the root of the SDK.
  for (auto &it : toDelete) {
    auto &path = it.first;
    auto isInput = it.second;
    if (!isInput && symlinks.count(path))
      continue;

    if (auto ec = sys::fs::remove(path)) {
      ctx.diag.report(diag::err) << path << ec.message();
      return false;
    }

    std::error_code ec;
    auto dir = sys::path::parent_path(path);
    do {
      ec = sys::fs::remove(dir);
      dir = sys::path::parent_path(dir);
      if (!dir.startswith(ctx.inputPath))
        break;
    } while (!ec);
  }

  return true;
}

/// \brief Generate text-based stub files from dynamic libraries.
bool Driver::Stub::run(DiagnosticsEngine &diag, Options &opts) {
  if (opts.driverOptions.inputs.empty()) {
    diag.report(clang::diag::err_drv_no_input_files);
    return false;
  }

  // FIME: Copy everything for now.
  Context ctx(opts.getFileManager(), diag);
  ctx.deleteInputFile = opts.tapiOptions.deleteInputFile;
  ctx.inlinePrivateFrameworks = opts.tapiOptions.inlinePrivateFrameworks;
  ctx.deletePrivateFrameworks = opts.tapiOptions.deletePrivateFrameworks;
  ctx.recordUUIDs = opts.tapiOptions.recordUUIDs;
  ctx.setInstallAPIFlag = opts.tapiOptions.setInstallAPIFlag;

  // Handle isysroot.
  ctx.sysroot = opts.frontendOptions.isysroot;
  ctx.frameworkSearchPaths = opts.frontendOptions.systemFrameworkPaths;
  ctx.frameworkSearchPaths.insert(ctx.frameworkSearchPaths.end(),
                                  opts.frontendOptions.frameworkPaths.begin(),
                                  opts.frontendOptions.frameworkPaths.end());
  ctx.librarySearchPaths = opts.frontendOptions.libraryPaths;
  ctx.fileType = opts.tapiOptions.fileType;

  // Only expect one input.
  SmallString<PATH_MAX> input;
  for (const auto &path : opts.driverOptions.inputs) {
    if (input.empty())
      input = path;
    else {
      diag.report(clang::diag::err_drv_unknown_argument) << path;
      return false;
    }
  }

  if (auto ec = realpath(input)) {
    diag.report(diag::err) << input << ec.message();
    return false;
  }
  ctx.inputPath = input.str();

  bool isDirectory = false;
  bool isFile = false;
  if (auto ec = sys::fs::is_directory(ctx.inputPath, isDirectory)) {
    diag.report(diag::err) << ctx.inputPath << ec.message();
    return false;
  }
  if (!isDirectory) {
    if (auto ec = sys::fs::is_regular_file(ctx.inputPath, isFile)) {
      diag.report(diag::err) << ctx.inputPath << ec.message();
      return false;
    }
  }

  // Expect a directory or a file.
  if (!isDirectory && !isFile) {
    diag.report(diag::err_invalid_input_file) << ctx.inputPath;
    return false;
  }

  // Handle -o.
  if (!opts.driverOptions.outputPath.empty())
    ctx.outputPath = opts.driverOptions.outputPath;
  else if (isFile) {
    SmallString<PATH_MAX> outputPath(ctx.inputPath);
    TAPI_INTERNAL::replace_extension(outputPath, ".tbd");
    ctx.outputPath = outputPath.str();
  } else {
    assert(isDirectory && "Expected a directory.");
    ctx.outputPath = ctx.inputPath;
  }

  if (isDirectory)
    ctx.searchPaths.emplace_back(ctx.inputPath);

  if (!ctx.sysroot.empty())
    ctx.searchPaths.emplace_back(ctx.sysroot);

  if (isFile)
    return stubifyDynamicLibrary(ctx);

  return stubifyDirectory(ctx);
}

TAPI_NAMESPACE_INTERNAL_END
