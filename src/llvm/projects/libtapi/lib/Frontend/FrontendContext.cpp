//===- lib/Frontend/FrontendContext.cpp - TAPI Frontend Context -*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the TAPI Frontend Context
///
//===----------------------------------------------------------------------===//

#include "tapi/Frontend/FrontendContext.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

FrontendContext::FrontendContext(
    StringRef workingDirectory,
    IntrusiveRefCntPtr<FileSystemStatCacheFactory> cacheFactory,
    IntrusiveRefCntPtr<clang::vfs::FileSystem> vfs) {
  fileManager = new FileManager(clang::FileSystemOptions{workingDirectory},
                                cacheFactory, vfs);
}

TAPI_NAMESPACE_INTERNAL_END
