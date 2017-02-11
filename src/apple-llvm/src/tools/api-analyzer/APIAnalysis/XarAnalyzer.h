//=- llvm/Analysis/XarAnalyzer.h - API Usage Analysis -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_XARANALYZER_H
#define LLVM_XARANALYZER_H

#include "APIAnalysisImpl.h"

namespace llvm {
namespace object {
class SectionRef;
}
}

int AnalyzeXar(llvm::StringRef filePath, APIAnalysisIntermediateResult &result,
               const APIAnalysisOptions &options);

int AnalyzeXar(llvm::MemoryBufferRef fileData,
               APIAnalysisIntermediateResult &result,
               const APIAnalysisOptions &options);

#endif
