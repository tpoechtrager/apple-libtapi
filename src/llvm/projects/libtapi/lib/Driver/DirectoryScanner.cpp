//===- lib/Driver/DirectoryScanner.cpp - Directory Scanner ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the directory scanner.
///
/// Scans the directory for frameworks.
///
//===----------------------------------------------------------------------===//

#include "tapi/Driver/DirectoryScanner.h"
#include "tapi/Core/FileManager.h"
#include "tapi/Core/Framework.h"
#include "tapi/Core/HeaderFile.h"
#include "tapi/Core/Utils.h"
#include "tapi/Diagnostics/Diagnostics.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/VirtualFileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

static bool isFramework(StringRef path) {
  while (path.back() == '/')
    path = path.slice(0, path.size() - 1);

  return StringSwitch<bool>(sys::path::extension(path))
      .Case(".framework", true)
      .Default(false);
}

DirectoryScanner::DirectoryScanner(FileManager &fm, DiagnosticsEngine &diag)
    : _fm(fm), diag(diag) {
  _registry.addBinaryReaders();
}

bool DirectoryScanner::scanDylibDirectory(
    StringRef directory, std::vector<Framework> &frameworks) const {

  // Check some known sub-directory locations.
  auto getDirectory = [&](const char *subDirectory) {
    SmallString<PATH_MAX> path(directory);
    sys::path::append(path, subDirectory);
    return _fm.getDirectory(path, /*CacheFailure=*/false);
  };
  const DirectoryEntry *directoryEntryPublic = getDirectory("usr/include");
  const DirectoryEntry *directoryEntryPrivate =
      getDirectory("usr/local/include");

  if (!directoryEntryPublic && !directoryEntryPrivate) {
    errs() << "error: Cannot find public or private sub directories: "
           << directory << "\n";
  }

  frameworks.emplace_back(directory);
  auto &dylib = frameworks.back();
  dylib.isDynamicLibrary = true;

  if (directoryEntryPublic) {
    if (!scanHeaders(dylib, directoryEntryPublic->getName(),
                     HeaderType::Public))
      return false;
  }
  if (directoryEntryPrivate) {
    if (!scanHeaders(dylib, directoryEntryPrivate->getName(),
                     HeaderType::Private))
      return false;
  }

  return true;
}

bool DirectoryScanner::scanDirectory(StringRef directory,
                                     std::vector<Framework> &frameworks) const {
  if (scanDylibLocations)
    return scanDylibDirectory(directory, frameworks);

  // We expect a certain directory structure and naming convention to find the
  // frameworks.
  static const char *subDirectories[] = {"System/Library/Frameworks/",
                                         "System/Library/PrivateFrameworks/"};

  // Check if the directory is already a framework.
  if (isFramework(directory)) {
    frameworks.emplace_back(directory);
    if (!scanFrameworkDirectory(frameworks.back()))
      return false;
    return true;
  }

  // Check some known sub-directory locations.
  for (const auto *subDirectory : subDirectories) {
    SmallString<PATH_MAX> path(directory);
    sys::path::append(path, subDirectory);

    if (!scanFrameworksDirectory(frameworks, path))
      return false;
  }

  return true;
}

/// \brief Scan the directory for frameworks.
bool DirectoryScanner::scanFrameworksDirectory(
    std::vector<Framework> &frameworks, StringRef directory) const {
  std::error_code ec;
  auto &fs = *_fm.getVirtualFileSystem();
  for (vfs::directory_iterator i = fs.dir_begin(directory, ec), ie; i != ie;
       i.increment(ec)) {
    auto path = i->getName();

    // Skip files that not exist. This usually happens for broken symlinks.
    if (ec == std::errc::no_such_file_or_directory) {
      ec.clear();
      continue;
    }

    if (ec) {
      diag.report(diag::err) << path << ec.message();
      return false;
    }

    if (isFramework(path)) {
      if (!_fm.isDirectory(path, /*CacheFailure=*/false))
        continue;

      frameworks.emplace_back(path);

      if (!scanFrameworkDirectory(frameworks.back()))
        return false;
    }
  }

  return true;
}

bool DirectoryScanner::scanSubFrameworksDirectory(
    std::vector<Framework> &frameworks, StringRef path) const {
  if (_fm.isDirectory(path, /*CacheFailure=*/false))
    return scanFrameworksDirectory(frameworks, path);

  diag.report(diag::err_no_directory) << path;
  return false;
}

bool DirectoryScanner::scanFrameworkDirectory(Framework &framework) const {
  // Unfortunately we cannot identify symlinks in the VFS. We assume that if
  // there is a Versions directory, then we have symlinks and directly proceed
  // to the Versiosn folder.
  std::error_code ec;
  auto &fs = *_fm.getVirtualFileSystem();
  for (vfs::directory_iterator i = fs.dir_begin(framework.getPath(), ec), ie;
       i != ie; i.increment(ec)) {
    auto path = i->getName();

    // Skip files that not exist. This usually happens for broken symlinks.
    if (ec == std::errc::no_such_file_or_directory) {
      ec.clear();
      continue;
    }

    if (ec) {
      diag.report(diag::err) << path << ec.message();
      return false;
    }

    if (_fm.isSymlink(path))
      continue;

    StringRef fileName = sys::path::filename(path);
    // Scan all "public" headers.
    if (fileName.compare("Headers") == 0) {
      if (!scanHeaders(framework, path, HeaderType::Public))
        return false;
      continue;
    }
    // Scan all "private" headers.
    else if (fileName.compare("PrivateHeaders") == 0) {
      if (!scanHeaders(framework, path, HeaderType::Private))
        return false;
      continue;
    }
    // Scan for module maps.
    else if (fileName.compare("Modules") == 0) {
      if (!scanModules(framework, path))
        return false;
    }
    // Check for sub frameworks.
    else if (fileName.compare("Frameworks") == 0) {
      if (!scanSubFrameworksDirectory(framework._subFrameworks, path))
        return false;
      continue;
    }
    // Check for versioned frameworks.
    else if (fileName.compare("Versions") == 0) {
      if (!scanFrameworkVersionsDirectory(framework, path))
        return false;
      continue;
    }

    // If it is a directory, scan the directory to check for dynamic libs.
    if (_fm.isDirectory(path, /*CacheFailure=*/false)) {
      if (!scanLibraryDirectory(framework, path))
        return false;
      continue;
    }

    // Check for dynamic libs.
    auto result = isDynamicLibrary(path);
    if (!result) {
      diag.report(diag::err) << path << toString(result.takeError());
      return false;
    }

    if (result.get())
      framework.addDynamicLibraryFile(path);
  }

  return true;
}

bool DirectoryScanner::scanHeaders(Framework &framework, StringRef path,
                                   HeaderType type) const {
  std::error_code ec;
  auto &fs = *_fm.getVirtualFileSystem();
  for (vfs::recursive_directory_iterator i(fs, path, ec), ie; i != ie;
       i.increment(ec)) {
    auto headerPath = i->getName();

    // Skip files that not exist. This usually happens for broken symlinks.
    if (ec == std::errc::no_such_file_or_directory) {
      ec.clear();
      continue;
    }

    if (ec) {
      diag.report(diag::err) << headerPath << ec.message();
      return false;
    }

    // Ignore tmp files from unifdef.
    auto filename = sys::path::filename(headerPath);
    if (filename.startswith("."))
      continue;

    if (!isHeaderFile(headerPath))
      continue;

    framework.addHeaderFile(headerPath, type,
                            headerPath.drop_front(path.size()));
  }

  return true;
}

bool DirectoryScanner::scanModules(Framework &framework, StringRef path) const {
  std::error_code ec;
  auto &fs = *_fm.getVirtualFileSystem();
  for (vfs::recursive_directory_iterator i(fs, path, ec), ie; i != ie;
       i.increment(ec)) {
    auto path = i->getName();

    // Skip files that not exist. This usually happens for broken symlinks.
    if (ec == std::errc::no_such_file_or_directory) {
      ec.clear();
      continue;
    } else if (ec) {
      diag.report(diag::err) << path << ec.message();
      return false;
    }

    if (!path.endswith(".modulemap"))
      continue;

    framework.addModuleMap(path);
  }

  return true;
}

/// FIXME: How to handle versions? For now scan them separately as independent
/// frameworks.
bool DirectoryScanner::scanFrameworkVersionsDirectory(Framework &framework,
                                                      StringRef path) const {
  std::error_code ec;
  auto &fs = *_fm.getVirtualFileSystem();
  for (vfs::directory_iterator i = fs.dir_begin(path, ec), ie; i != ie;
       i.increment(ec)) {
    auto path = i->getName();

    // Skip files that not exist. This usually happens for broken symlinks.
    if (ec == std::errc::no_such_file_or_directory) {
      ec.clear();
      continue;
    }

    if (ec) {
      diag.report(diag::err) << path << ec.message();
      return false;
    }

    if (_fm.isSymlink(path))
      continue;

    // Each version is just a framework directory.
    if (!_fm.isDirectory(path, /*CacheFailure=*/false))
      continue;

    framework._versions.emplace_back(path);
    if (!scanFrameworkDirectory(framework._versions.back()))
      return false;
  }

  return true;
}

bool DirectoryScanner::scanLibraryDirectory(Framework &framework,
                                            StringRef path) const {
  std::error_code ec;
  auto &fs = *_fm.getVirtualFileSystem();
  for (vfs::recursive_directory_iterator i(fs, path, ec), ie; i != ie;
       i.increment(ec)) {
    auto path = i->getName();

    // Skip files that not exist. This usually happens for broken symlinks.
    if (ec == std::errc::no_such_file_or_directory) {
      ec.clear();
      continue;
    }

    if (ec) {
      diag.report(diag::err) << path << ec.message();
      return false;
    }

    if (_fm.isSymlink(path))
      continue;

    if (_fm.isDirectory(path, /*CacheFailure=*/false))
      continue;

    // Check for dynamic libs.
    auto result = isDynamicLibrary(path);
    if (!result) {
      diag.report(diag::err) << path << toString(result.takeError());
      return false;
    }

    if (result.get())
      framework.addDynamicLibraryFile(path);
  }

  return true;
}

Expected<bool> DirectoryScanner::isDynamicLibrary(StringRef path) const {
  auto bufferOrErr = _fm.getBufferForFile(path);
  if (auto ec = bufferOrErr.getError())
    return errorCodeToError(ec);

  auto fileType = _registry.getFileType(*bufferOrErr.get());
  if (!fileType)
    return fileType;

  if (fileType.get() == FileType::MachO_DynamicLibrary ||
      fileType.get() == FileType::MachO_DynamicLibrary_Stub)
    return true;

  if (allowBundles && (fileType.get() == FileType::MachO_Bundle))
    return true;

  return false;
}

bool DirectoryScanner::addDylibsAsFramework(
    StringRef name, StringRef path, const PathSeq &dylibs,
    std::vector<Framework> &frameworks) const {
  assert(_fm.isDirectory(path, false) && "Directory must exists");
  frameworks.emplace_back(path);
  auto &framework = frameworks.back();

  for (const auto &path : dylibs) {
    // Check for dynamic libs.
    auto result = isDynamicLibrary(path);
    if (!result) {
      diag.report(diag::err) << path << toString(result.takeError());
      return false;
    }

    if (result.get())
      framework.addDynamicLibraryFile(path);
    else
      return false;
  }

  return true;
}

bool DirectoryScanner::scanSDKContent(StringRef directory,
                                      std::vector<Framework> &frameworks,
                                      Configuration *config) const {
  // Build a Unix framework for information in /usr/include and /usr/lib
  frameworks.emplace_back(directory);
  auto &SDKFramework = frameworks.back();
  auto getDirectory = [&](StringRef subDirectory) {
    SmallString<PATH_MAX> path(directory);
    sys::path::append(path, subDirectory);
    return path;
  };

  // Scan headers.
  if (!scanHeaders(SDKFramework, getDirectory("usr/include"),
                   HeaderType::Public))
    return false;
  if (!scanHeaders(SDKFramework, getDirectory("usr/local/include"),
                   HeaderType::Private))
    return false;
  // Scan dylibs.
  if (!scanLibraryDirectory(SDKFramework, getDirectory("usr/lib")))
    return false;

  if (!scanLibraryDirectory(SDKFramework, getDirectory("usr/local/lib")))
    return false;

  // Adding all the frameworks in /System.
  if (!scanFrameworksDirectory(SDKFramework._subFrameworks,
                               getDirectory("System/Library/Frameworks")))
    return false;

  if (!scanFrameworksDirectory(
          SDKFramework._subFrameworks,
          getDirectory("System/Library/PrivateFrameworks")))
    return false;

  // Add dylib configurations.
  if (config) {
    for (const auto &dylibs : config->dylibConfigs) {
      if (!addDylibsAsFramework(dylibs->name, directory, dylibs->binaries,
                                SDKFramework._subFrameworks))
        return false;
    }
  }

  if (!scanAllLocations)
    return true;

  // Scan the bundles and extensions in /System/Library.
  std::error_code ec;
  auto &fs = *_fm.getVirtualFileSystem();
  for (auto i = fs.dir_begin(getDirectory("System/Library"), ec);
       i != vfs::directory_iterator(); i.increment(ec)) {
    auto path = i->getName();

    // Skip files that not exist. This usually happens for broken symlinks.
    if (ec == std::errc::no_such_file_or_directory) {
      ec.clear();
      continue;
    } else if (ec) {
      diag.report(diag::err) << path << ec.message();
      return false;
    }

    // Skip framework directories that is handled above.
    if (path.endswith("Frameworks") || path.endswith("PrivateFrameworks"))
      continue;

    // Skip all that is not a directory.
    if (_fm.isDirectory(path, /*CacheFailure=*/false)) {
      SDKFramework._subFrameworks.emplace_back(path);
      if (!scanLibraryDirectory(SDKFramework._subFrameworks.back(), path))
        return false;
    }
  }

  return true;
}

bool DirectoryScanner::scan(StringRef directory,
                            std::vector<Framework> &frameworks,
                            Configuration *config) const {
  if (!scanSDK)
    return scanDirectory(directory, frameworks);

  return scanSDKContent(directory, frameworks, config);
}

TAPI_NAMESPACE_INTERNAL_END
