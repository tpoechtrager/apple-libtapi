//===- tapi/Core/Utils.h - TAPI Utility Methods -----------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Misc utility methods.
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_UTILS_H
#define TAPI_CORE_UTILS_H

#include "tapi/Core/LLVM.h"
#include "tapi/Defines.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/TextAPI/MachO/ArchitectureSet.h"
#include "llvm/TextAPI/MachO/Platform.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

class FileManager;

/// Return true of the path to the dylib is considered a public location.
bool isPublicDylib(StringRef path);
/// Return true of the path is within a location of public framework.
bool isWithinPublicLocation(StringRef path);
/// Return true of the path has extension of a header.
bool isHeaderFile(StringRef path);
/// Search the path to find the library specificed by installName.
std::string findLibrary(StringRef installName, FileManager &fm,
                        ArrayRef<std::string> frameworkSearchPaths,
                        ArrayRef<std::string> librarySearchPaths,
                        ArrayRef<std::string> searchPaths);

ArchitectureSet mapToArchitectureSet(ArrayRef<llvm::Triple> targets);
std::string getOSAndEnvironmentName(llvm::MachO::PlatformKind Platform,
                                    std::string Version);
llvm::MachO::PlatformKind getPlatformFromName(StringRef name);

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_UTILS_H
