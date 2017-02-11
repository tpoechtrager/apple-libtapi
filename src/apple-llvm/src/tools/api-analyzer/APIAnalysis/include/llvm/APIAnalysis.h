//===- APIAnalysis.h - API Analysis ---------------------------------------===//
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

#ifndef LLVM_APIANALYSIS_H
#define LLVM_APIANALYSIS_H

#include <map>
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/Support/MemoryBuffer.h"

struct ObjCClassInfo {
  llvm::SmallVector<std::string, 10> internalInstanceMethods;
  llvm::SmallVector<std::string, 10> internalClassMethods;
  llvm::SmallVector<std::string, 10> externalInstanceMethods;
  llvm::SmallVector<std::string, 10> externalClassMethods;
  std::string parentClass;
};

struct APIAnalysisResult {
  std::string installName;
  llvm::StringSet<> messageNames;
  llvm::StringMap<ObjCClassInfo> objCClasses;
  llvm::StringMap<ObjCClassInfo> objCProtocols;
  std::map<std::pair<std::string, std::string>, ObjCClassInfo> objCCategories;
  llvm::StringSet<> internalClassNames;
  llvm::StringSet<> externalClassNames;
  llvm::StringSet<> externalFunctions;
  llvm::StringSet<> internalFunctions;
  llvm::StringSet<> externalGlobals;
  llvm::StringSet<> internalGlobals;
  llvm::StringSet<> linkedLibraries;
  llvm::StringSet<> potentiallyDefinedSelectors;
  llvm::SmallVector<std::string, 16> orderedLibraries;
  llvm::SmallVector<std::string, 16> asmSymbols;
};

struct APIAnalysisOptions {
  bool scanSelFromString;
  bool scanDLSym;
  bool demangle;
  bool bitcodeOnly;
};

int InitializeAnalyzer();

int AnalyzeFile(llvm::StringRef &filePath, APIAnalysisResult &result,
                const APIAnalysisOptions &options);

int AnalyzeFile(llvm::MemoryBufferRef &fileData, APIAnalysisResult &result,
                const APIAnalysisOptions &options);

#endif
