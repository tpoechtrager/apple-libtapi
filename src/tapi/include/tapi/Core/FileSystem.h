//===- tapi/Core/FileSystem.h - File System ---------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Additional file system support that is missing in LLVM.
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_FILE_SYSTEM_H
#define TAPI_CORE_FILE_SYSTEM_H

#include "tapi/Core/LLVM.h"
#include "tapi/Defines.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/Twine.h"
#include "llvm/Support/VirtualFileSystem.h"
#include <system_error>

TAPI_NAMESPACE_INTERNAL_BEGIN

std::error_code realpath(SmallVectorImpl<char> &path);

std::error_code read_link(const Twine &path, SmallVectorImpl<char> &linkPath);

std::error_code shouldSkipSymlink(const Twine &path, bool &result);

std::error_code make_relative(StringRef from, StringRef to,
                              SmallVectorImpl<char> &relativePath);


// MaskingOverlayFileSystem.
// OverlayFileSystem that is capable of masking some directories so that
// it appears to be not existing. Only works as the top layer of overlay
// file system.
class MaskingOverlayFileSystem : public llvm::vfs::OverlayFileSystem {
public:
  MaskingOverlayFileSystem(IntrusiveRefCntPtr<FileSystem> base,
                           StringRef sysroot);

  llvm::ErrorOr<llvm::vfs::Status> status(const Twine &path) override;

  void addExtraMaskingDirectory(StringRef path) {
    extraMaskingPath.emplace_back(path.str());
  }

private:
  std::string sysroot;
  std::vector<std::string> extraMaskingPath;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_FILE_SYSTEM_H
