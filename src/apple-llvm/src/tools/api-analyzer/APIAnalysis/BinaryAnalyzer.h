//===- BinaryAnalyzer.h - API Analysis ------------------------------------===//
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

#ifndef LLVM_BINARYANALYZER_H
#define LLVM_BINARYANALYZER_H

#include "APIAnalysisImpl.h"
namespace llvm {
namespace object {
class Binary;
}
}

int AnalyzeBinary(llvm::StringRef filePath,
                  APIAnalysisIntermediateResult &result,
                  const APIAnalysisOptions &options);

int AnalyzeBinary(llvm::MemoryBufferRef fileData,
                  APIAnalysisIntermediateResult &result,
                  const APIAnalysisOptions &options);

int AnalyzeBinaryImpl(llvm::object::Binary &Binary,
                      APIAnalysisIntermediateResult &result,
                      const APIAnalysisOptions &options);

#endif
