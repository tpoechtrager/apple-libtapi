//===- tapi/Core/Framework.h - TAPI Framework -------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the content of a framework, such as public and private
/// header files, and dynamic libraries.
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_FRAMEWORK_H
#define TAPI_CORE_FRAMEWORK_H

#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/InterfaceFile.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Core/XPI.h"
#include "tapi/Defines.h"
#include "llvm/ADT/StringRef.h"
#include <map>
#include <string>
#include <vector>

TAPI_NAMESPACE_INTERNAL_BEGIN

enum class HeaderType;

struct Framework {
  std::string _baseDirectory;
  std::map<std::string, HeaderType> _headerFiles;
  std::vector<std::string> _dynamicLibraryFiles;
  std::vector<Framework> _subFrameworks;
  std::vector<Framework> _versions;
  std::vector<std::unique_ptr<ExtendedInterfaceFile>> _interfaceFiles;
  std::unique_ptr<XPISet> _headerSymbols;

  Framework(StringRef directory) : _baseDirectory(directory) {}

  StringRef getName() const;

  StringRef getPath() const { return _baseDirectory; }

  void addHeaderFile(StringRef path, HeaderType type) {
    _headerFiles.emplace(path, type);
  }

  void addDynamicLibraryFile(StringRef path) {
    _dynamicLibraryFiles.emplace_back(path);
  }

  bool verify(bool warnAll = true) const;

  void print(raw_ostream &os) const;
};

inline raw_ostream &operator<<(raw_ostream &os, const Framework &framework) {
  framework.print(os);
  return os;
}

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_FRAMEWORK_H
