//===- APIAnalysisImpl.h - API Analysis -----------------------------------===//
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

#ifndef LLVM_APIANALYSISIMPL_H
#define LLVM_APIANALYSISIMPL_H

#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/APIAnalysis.h"

struct APIAnalysisIntermediateResult {
  std::string installName;
  llvm::StringSet<> messageNames;
  llvm::StringMap<bool> classNames;
  llvm::StringMap<llvm::StringMap<bool>> instanceMethods;
  llvm::StringMap<llvm::StringMap<bool>> classMethods;
  llvm::StringMap<llvm::StringMap<bool>> protocolInstanceMethods;
  llvm::StringMap<llvm::StringMap<bool>> protocolClassMethods;
  llvm::StringMap<llvm::StringMap<llvm::StringMap<bool>>>
      categoryInstanceMethods;
  llvm::StringMap<llvm::StringMap<llvm::StringMap<bool>>> categoryClassMethods;
  llvm::StringMap<bool> functionNames;
  llvm::StringMap<bool> globals;
  llvm::StringSet<> linkedLibraries;
  llvm::SmallVector<std::string, 16> orderedLibraries;
  llvm::StringSet<> asmSymbols;
  llvm::StringSet<> localSymbols;
  llvm::StringMap<std::string> superClasses;
  llvm::StringSet<> potentiallyDefinedSelectors;

  llvm::StringMap<std::string> classLinkMap;
};

int AnalyzeFileImpl(llvm::StringRef &filePath,
                    APIAnalysisIntermediateResult &result,
                    const APIAnalysisOptions &options);

int AnalyzeFileImpl(llvm::MemoryBufferRef &fileData,
                    APIAnalysisIntermediateResult &result,
                    const APIAnalysisOptions &options);

#endif
