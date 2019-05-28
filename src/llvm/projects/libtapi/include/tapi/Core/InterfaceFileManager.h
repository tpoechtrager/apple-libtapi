//===- IntefaceFileManager.h - TAPI Interface File Manager ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Interface File Manager
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_INTERFACE_FILE_MANAGER_H
#define TAPI_CORE_INTERFACE_FILE_MANAGER_H

#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/Registry.h"
#include "tapi/Defines.h"
#include <map>

TAPI_NAMESPACE_INTERNAL_BEGIN

class FileManager;

class InterfaceFileManager {
public:
  InterfaceFileManager(FileManager &fm);
  Expected<InterfaceFileBase *> readFile(const std::string &path);
  Error writeFile(const InterfaceFileBase *file, const std::string &path) const;

private:
  FileManager &_fm;
  Registry _registry;
  std::map<std::string, std::unique_ptr<InterfaceFileBase>> _libraries;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_INTERFACE_FILE_MANAGER_H
