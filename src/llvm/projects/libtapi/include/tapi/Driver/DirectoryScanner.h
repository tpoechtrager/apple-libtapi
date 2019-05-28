//===- tapi/Driver/DirectoryScanner.h - DirectoryScanner --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the directory scanner.
//===----------------------------------------------------------------------===//

#ifndef TAPI_DRIVER_DIRECTORYSCANNER_H
#define TAPI_DRIVER_DIRECTORYSCANNER_H

#include "tapi/Core/Framework.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Core/Registry.h"
#include "tapi/Defines.h"
#include "tapi/Driver/Configuration.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Error.h"
#include <vector>

TAPI_NAMESPACE_INTERNAL_BEGIN

class DiagnosticsEngine;
class FileManager;
struct Framework;
enum class HeaderType;

class DirectoryScanner {
public:
  DirectoryScanner(FileManager &fm, DiagnosticsEngine &diag);
  bool scan(StringRef directory, std::vector<Framework> &frameworks,
            Configuration *config = nullptr) const;

  void setScanSDKContent(bool isSDK) { scanSDK = isSDK; }
  void setScanAllLocations(bool all) { scanAllLocations = all; }
  void setScanDylibLocations(bool dylib) { scanDylibLocations = dylib; }
  void setAllowBundle(bool bundle) { allowBundles = bundle; }

private:
  bool scanDylibDirectory(StringRef directory,
                          std::vector<Framework> &frameworks) const;
  bool scanFrameworksDirectory(std::vector<Framework> &frameworks,
                               StringRef directory) const;
  bool scanSubFrameworksDirectory(std::vector<Framework> &frameworks,
                                  StringRef path) const;
  bool scanFrameworkDirectory(Framework &framework) const;
  bool scanHeaders(Framework &framework, StringRef path, HeaderType type) const;
  bool scanModules(Framework &framework, StringRef path) const;
  bool scanFrameworkVersionsDirectory(Framework &framework,
                                      StringRef path) const;
  bool scanLibraryDirectory(Framework &framework, StringRef path) const;
  Expected<bool> isDynamicLibrary(StringRef path) const;

  bool addDylibsAsFramework(StringRef name, StringRef path,
                            const PathSeq &dylibs,
                            std::vector<Framework> &frameworks) const;

  bool scanDirectory(StringRef directory,
                     std::vector<Framework> &frameworks) const;

  bool scanSDKContent(StringRef directory, std::vector<Framework> &frameworks,
                      Configuration *config) const;

private:
  Registry _registry;
  FileManager &_fm;
  DiagnosticsEngine &diag;

  // Scanner options.
  bool scanSDK = false;
  bool scanAllLocations = false;
  bool scanDylibLocations = false;
  bool allowBundles = false;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_DRIVER_DIRECTORYSCANNER_H
