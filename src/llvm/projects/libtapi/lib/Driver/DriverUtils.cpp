//===- lib/Driver/DriverUtils.cpp - TAPI Driver Utilities -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the driver utility methods.
///
//===----------------------------------------------------------------------===//
#include "tapi/Driver/DriverUtils.h"
#include "tapi/Core/FileManager.h"
#include "tapi/Core/Utils.h"
#include "tapi/Driver/Diagnostics.h"
#include "clang/Basic/VirtualFileSystem.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Path.h"

using namespace llvm;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

static void findAndAddHeaderFiles(std::vector<const FileEntry *> &headersOut,
                                  FileManager &fm, DiagnosticsEngine &diag,
                                  const DirectoryEntry *dir) {
  std::error_code ec;
  auto &fs = *fm.getVirtualFileSystem();
  for (vfs::directory_iterator i = fs.dir_begin(dir->getName(), ec), ie;
       i != ie; i.increment(ec)) {
    auto path = i->getName();

    // Skip files that not exist. This usually happens for broken symlinks.
    if (ec == std::errc::no_such_file_or_directory) {
      ec.clear();
      continue;
    }

    if (ec) {
      diag.report(diag::err) << path << ec.message();
      return;
    }

    const auto *file = fm.getFile(path);
    if (!file) {
      diag.report(diag::err_cannot_open_file) << path;
      return;
    }

    if (isHeaderFile(file->getName()))
      headersOut.emplace_back(file);
  }
}

static bool findAndAddHeaderFiles(std::vector<const FileEntry *> &headersOut,
                                  FileManager &fm, DiagnosticsEngine &diag,
                                  StringRef path) {
  if (const auto *file = fm.getFile(path))
    headersOut.emplace_back(file);
  else if (const auto *dir = fm.getDirectory(path))
    findAndAddHeaderFiles(headersOut, fm, diag, dir);
  else
    return false;

  return true;
}

bool findAndAddHeaderFiles(std::vector<const FileEntry *> &headersOut,
                           FileManager &fm, DiagnosticsEngine &diag,
                           std::vector<std::string> headersIn,
                           StringRef sysroot, StringRef basePath,
                           unsigned diagID) {
  for (auto &path : headersIn) {
    if (findAndAddHeaderFiles(headersOut, fm, diag, path))
      continue;

    SmallString<PATH_MAX> fullPath;
    sys::path::append(fullPath, sysroot, path);
    if (findAndAddHeaderFiles(headersOut, fm, diag, fullPath))
      continue;

    SmallString<PATH_MAX> frameworkPath;
    sys::path::append(frameworkPath, basePath, "Headers", path);
    if (findAndAddHeaderFiles(headersOut, fm, diag, frameworkPath))
      continue;

    diag.report(diagID) << path;
    return false;
  }
  return true;
}

TAPI_NAMESPACE_INTERNAL_END
