//===- lib/Core/Path.cpp - Path Operating System Concept --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Modified path support to handle frameworks.
///
//===----------------------------------------------------------------------===//

#include "llvm/Support/Path.h"
#include "tapi/Core/FileManager.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Core/Utils.h"
#include "tapi/Defines.h"
#include "clang/Basic/VirtualFileSystem.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/Twine.h"
#include "llvm/Support/Error.h"

using namespace llvm;

TAPI_NAMESPACE_INTERNAL_BEGIN

/// \brief Replace extension considering frameworks.
void replace_extension(SmallVectorImpl<char> &path, const Twine &extension) {
  StringRef p(path.begin(), path.size());
  auto parentPath = sys::path::parent_path(p);
  auto filename = sys::path::filename(p);

  if (!parentPath.endswith(filename.str() + ".framework")) {
    sys::path::replace_extension(path, extension);
    return;
  }

  SmallString<8> ext_storage;
  StringRef ext = extension.toStringRef(ext_storage);

  // Append '.' if needed.
  if (!ext.empty() && ext[0] != '.')
    path.push_back('.');

  // Append extension.
  path.append(ext.begin(), ext.end());
}

llvm::Expected<std::vector<std::string>>
enumerateFiles(FileManager &fm, StringRef path,
               const std::function<bool(StringRef)> &func) {
  std::vector<std::string> headers;
  std::error_code ec;
  auto &fs = *fm.getVirtualFileSystem();
  for (clang::vfs::recursive_directory_iterator i(fs, path, ec), ie; i != ie;
       i.increment(ec)) {

    // Skip files that not exist. This usually happens for broken symlinks.
    if (ec == std::errc::no_such_file_or_directory) {
      ec.clear();
      continue;
    }

    if (ec)
      return errorCodeToError(ec);

    auto path = i->getName();
    if (func(path))
      headers.emplace_back(path);
  }

  return headers;
}

Expected<std::vector<std::string>> enumerateHeaderFiles(FileManager &fm,
                                                        StringRef path) {
  return enumerateFiles(fm, path, isHeaderFile);
}

TAPI_NAMESPACE_INTERNAL_END
