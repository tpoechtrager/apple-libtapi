//===-- ModuleObfuscator.h - Functions to obfuscate Modules -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This family of functions that are used to obfuscate strings in the Modules.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TRANSFORMS_UTILS_MODULEOBFUSCATOR_H
#define LLVM_TRANSFORMS_UTILS_MODULEOBFUSCATOR_H

#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include <utility> // for std::pair

namespace llvm {

class Module;
class Obfuscator;
class TargetMachine;

/// \brief Given a Module, obfuscate the strings in the modules. Symbols that
/// requires to be preserved for the system will be preserved as well.
bool obfuscateModule(Module &M, const TargetMachine *TM);

/// \brief Given a Module and a set of symbols must be preserved, obfuscate
/// the strings in the modules. Symbols that requires to be preserved for the
/// system will be preserved as well.
bool obfuscateModule(Module &M, const TargetMachine *TM, Obfuscator &obfs,
                     StringSet<> &PreserveSymbols);

} // End llvm namespace

#endif //  LLVM_TRANSFORMS_UTILS_MODULEOBFUSCATOR_H
