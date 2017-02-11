//=- llvm/Analysis/APIUsageAnalysis.h - API Usage Analysis ------*- C++ -*-===//
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

#ifndef LLVM_MODULEANALYZER_H
#define LLVM_MODULEANALYZER_H

#include "APIAnalysisImpl.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"

#include <unordered_map>

namespace llvm {
class Module;
}

typedef std::unordered_map<std::string, bool> APIInfo;

struct ObjCClass {
  std::string className;
  ObjCClass* superClass;
  
  APIInfo instanceMethods;
  APIInfo classMethods;

  bool isDefined;

  // constuctor
  ObjCClass(llvm::StringRef name) {
    className = name.str();
    superClass = nullptr;
    isDefined = false;
  }
};

typedef llvm::StringMap<ObjCClass> ObjCClassCollection;

int AnalyzeModule(llvm::StringRef filePath,
                  APIAnalysisIntermediateResult &result,
                  const APIAnalysisOptions &options);

int AnalyzeModule(llvm::MemoryBufferRef data,
                  APIAnalysisIntermediateResult &result,
                  const APIAnalysisOptions &options);

#endif
