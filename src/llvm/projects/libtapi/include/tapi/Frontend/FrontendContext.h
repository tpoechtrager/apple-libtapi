//===- Frontend/FrontendContext.h - TAPI Frontend Context -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the Frontend Context
///
//===----------------------------------------------------------------------===//
#ifndef TAPI_FRONTEND_FRONTENDCONTEXT_H
#define TAPI_FRONTEND_FRONTENDCONTEXT_H

#include "tapi/Core/API.h"
#include "tapi/Core/FileManager.h"
#include "tapi/Core/HeaderFile.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Defines.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include <map>

TAPI_NAMESPACE_INTERNAL_BEGIN

struct FrontendContext {
  llvm::Triple target;
  API api;
  std::unique_ptr<clang::CompilerInstance> compiler;
  llvm::IntrusiveRefCntPtr<clang::ASTContext> ast;
  llvm::IntrusiveRefCntPtr<clang::SourceManager> sourceMgr;
  std::shared_ptr<clang::Preprocessor> pp;
  llvm::IntrusiveRefCntPtr<FileManager> fileManager;
  std::map<const FileEntry *, HeaderType> files;

  FrontendContext(
      StringRef workingDirectory = StringRef(),
      IntrusiveRefCntPtr<FileSystemStatCacheFactory> cacheFactory = nullptr,
      IntrusiveRefCntPtr<clang::vfs::FileSystem> vfs = nullptr);

  void visit(APIVisitor &visitor) { api.visit(visitor); }
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_FRONTEND_FRONTENDCONTEXT_H
