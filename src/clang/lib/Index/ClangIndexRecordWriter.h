//===--- ClangIndexRecordWriter.h - Index record serialization ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_INDEX_CLANGINDEXRECORDWRITER_H
#define LLVM_CLANG_LIB_INDEX_CLANGINDEXRECORDWRITER_H

#include "IndexRecordHasher.h"
#include "clang/AST/Mangle.h"
#include "clang/Index/IndexRecordWriter.h"
#include "clang/Index/IndexingAction.h"
#include "llvm/ADT/SmallString.h"

namespace clang {
  class ASTContext;
  class Decl;
  class IdentifierInfo;
  class MacroInfo;

namespace index {
  class FileIndexRecord;

class ClangIndexRecordWriter {
  IndexRecordWriter Impl;

  ASTContext &Ctx;
  RecordingOptions RecordOpts;

  std::unique_ptr<ASTNameGenerator> ASTNameGen;
  llvm::BumpPtrAllocator Allocator;
  llvm::DenseMap<const void *, StringRef> USRByDecl;
  IndexRecordHasher Hasher;

public:
  ClangIndexRecordWriter(ASTContext &Ctx, RecordingOptions Opts);
  ~ClangIndexRecordWriter();

  ASTContext &getASTContext() { return Ctx; }
  ASTNameGenerator *getASTNameGen() { return ASTNameGen.get(); }

  bool writeRecord(StringRef Filename, const FileIndexRecord &Record,
                   std::string &Error, std::string *RecordFile = nullptr);
  StringRef getUSR(const Decl *D);
  StringRef getUSR(const IdentifierInfo *Name, const MacroInfo *MI);

private:
  StringRef getUSRNonCached(const Decl *D);
  StringRef getUSRNonCached(const IdentifierInfo *Name, const MacroInfo *MI);
};

} // end namespace index
} // end namespace clang

#endif
