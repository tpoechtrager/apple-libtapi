//===- tapi/Scanner/Scanner.h - TAPI Scanner --------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the TAPI Scanner.
//===----------------------------------------------------------------------===//

#ifndef TAPI_SCANNER_SCANNER_H
#define TAPI_SCANNER_SCANNER_H

#include "tapi/Core/ArchitectureSupport.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Core/XPI.h"
#include "clang/Frontend/FrontendOptions.h"
#include <map>


TAPI_NAMESPACE_INTERNAL_BEGIN

class FileManager;

struct ParsingJob {
  FileManager *fileManager = nullptr;
  ArchitectureSet architectures;
  Platform platform = Platform::Unknown;
  clang::InputKind::Language language = clang::InputKind::Unknown;
  bool useRTTI = true;
  bool scanPublicHeaders = true;
  bool scanPrivateHeaders = true;
  bool enableModules = false;
  bool validateSystemHeaders = false;
  bool useObjectiveCARC = false;
  bool useObjectiveCWeakARC = false;
  std::string osVersion;
  std::string language_std;
  std::string visibility;
  std::string isysroot;
  std::string serializeDiagnosticsFile;
  std::string moduleCachePath;
  std::string clangResourcePath;
  std::vector<std::pair<std::string, bool /*isUndef*/>> macros;
  std::vector<const clang::FileEntry *> publicPreIncludeFiles;
  std::vector<const clang::FileEntry *> privatePreIncludeFiles;
  std::vector<const clang::FileEntry *> publicHeaderFiles;
  std::vector<const clang::FileEntry *> privateHeaderFiles;
  std::vector<std::string> systemFrameworkPaths;
  std::vector<std::string> systemIncludePaths;
  std::vector<std::string> frameworkPaths;
  std::vector<std::string> includePaths;
  std::vector<std::string> clangExtraArgs;
};

class Scanner {
public:
  Scanner() = delete;
  static std::unique_ptr<XPISet> run(std::unique_ptr<ParsingJob> job);
};

TAPI_NAMESPACE_INTERNAL_END 

#endif // TAPI_SCANNER_SCANNER_H
