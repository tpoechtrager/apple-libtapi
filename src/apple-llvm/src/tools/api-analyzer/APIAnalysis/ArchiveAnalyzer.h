//===- ArchiveAnalyzer.h - API Analysis -----------------------------------===//
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

#ifndef LLVM_ARCHIVEANALYZER_H
#define LLVM_ARCHIVEANALYZER_H

#include "APIAnalysisImpl.h"

int AnalyzeArchive(llvm::StringRef filePath,
                   APIAnalysisIntermediateResult &result,
                   const APIAnalysisOptions &options);

#endif
