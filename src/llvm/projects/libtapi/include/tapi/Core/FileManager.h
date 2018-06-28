//===- tapi/Core/FileManager.h - File Manager --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Extends the clang file manager.
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_FILE_MANAGER_H
#define TAPI_CORE_FILE_MANAGER_H

#include "tapi/Core/LLVM.h"
#include "tapi/Defines.h"
#include "clang/Basic/FileManager.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

/// \brief Basically the clang FileManager with additonal convenience methods
///        and a recording stat cache.
class FileManager final : public clang::FileManager {
public:
  FileManager(const clang::FileSystemOptions &fileSystemOpts,
              llvm::IntrusiveRefCntPtr<clang::vfs::FileSystem> fs = nullptr);

  /// \brief Check if a particular path exists.
  bool exists(StringRef path);

  /// \brief Check if a particular path is a directory.
  bool isDirectory(StringRef path, bool CacheFailure = true) {
    return getDirectory(path, CacheFailure) != nullptr;
  }

  /// \brief Check if a particular path is a symlink using directory_iterator.
  bool isSymlink(StringRef path);

  /// \brief Add a recording stat cache. This is used by the snapshot system.
  void installStatRecorder();

private:
  bool initWithVFS = false;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_FILE_MANAGER_H
