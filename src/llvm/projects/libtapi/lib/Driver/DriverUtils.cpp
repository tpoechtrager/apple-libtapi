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
#include "tapi/Diagnostics/Diagnostics.h"
#include "clang/Basic/VirtualFileSystem.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Path.h"

using namespace llvm;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

static void findAndAddHeaderFilesImpl(HeaderSeq &headersOut, FileManager &fm,
                                      DiagnosticsEngine &diag, StringRef path,
                                      HeaderType type) {
  std::error_code ec;
  auto &fs = *fm.getVirtualFileSystem();
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
      return;
    }

    if (isHeaderFile(path))
      headersOut.emplace_back(path, type);
  }
}

static bool findAndAddHeaderFiles(HeaderSeq &headersOut, FileManager &fm,
                                  DiagnosticsEngine &diag, StringRef path,
                                  HeaderType type) {
  if (fm.isDirectory(path))
    findAndAddHeaderFilesImpl(headersOut, fm, diag, path, type);
  else if (fm.exists(path))
    headersOut.emplace_back(path, type);
  else
    return false;

  return true;
}

bool findAndAddHeaderFiles(HeaderSeq &headersOut, FileManager &fm,
                           DiagnosticsEngine &diag, PathSeq headersIn,
                           HeaderType type, StringRef sysroot,
                           StringRef basePath, unsigned diagID) {
  for (auto &path : headersIn) {
    if (findAndAddHeaderFiles(headersOut, fm, diag, path, type))
      continue;

    SmallString<PATH_MAX> fullPath;
    sys::path::append(fullPath, sysroot, path);
    if (findAndAddHeaderFiles(headersOut, fm, diag, fullPath, type))
      continue;

    SmallString<PATH_MAX> frameworkPath;
    sys::path::append(frameworkPath, basePath, "Headers", path);
    if (findAndAddHeaderFiles(headersOut, fm, diag, frameworkPath, type))
      continue;

    diag.report(diagID) << path;
    return false;
  }
  return true;
}

TAPI_NAMESPACE_INTERNAL_END
