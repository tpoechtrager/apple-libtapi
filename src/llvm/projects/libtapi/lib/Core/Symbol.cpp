//===- lib/Core/Symbol.cpp - Symbol -----------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the Symbol
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/Symbol.h"
#include "llvm/Demangle/Demangle.h"
#include "llvm/Support/Compiler.h"
#include <string>

using namespace llvm;

TAPI_NAMESPACE_INTERNAL_BEGIN

std::string Symbol::getPrettyName(bool demangle) const {
  if (!demangle)
    return name;

  if (demangle && name.startswith("__Z")) {
    int status = 0;
    char *demangledName = llvm::itaniumDemangle(name.substr(1).str().c_str(),
                                                nullptr, nullptr, &status);
    if (status == 0) {
      std::string result = demangledName;
      free(demangledName);
      return result;
    }
  }

  if (name[0] == '_')
    return name.substr(1);

  return name;
}

std::string Symbol::getAnnotatedName(bool demangle) const {
  std::string result;
  if (isWeakDefined())
    result += "(weak-def) ";
  if (isWeakReferenced())
    result += "(weak-ref) ";
  if (isThreadLocalValue())
    result += "(tlv) ";
  switch (kind) {
  case SymbolKind::GlobalSymbol:
    return result + getPrettyName(demangle);
  case SymbolKind::ObjectiveCClass:
    return result + "(ObjC Class) " + name.str();
  case SymbolKind::ObjectiveCClassEHType:
    return result + "(ObjC Class EH) " + name.str();
  case SymbolKind::ObjectiveCInstanceVariable:
    return result + "(ObjC IVar) " + name.str();
  }
}

void Symbol::print(raw_ostream &os) const { os << getAnnotatedName(); }

#if !defined(NDEBUG) || defined(LLVM_ENABLE_DUMP)
/// \brief Print APISymbol in human readable format.
LLVM_DUMP_METHOD void Symbol::dump(raw_ostream &os) const {
  os << getAnnotatedName(true);
}
#endif

TAPI_NAMESPACE_INTERNAL_END
