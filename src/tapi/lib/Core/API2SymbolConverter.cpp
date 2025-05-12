//===- lib/Core/API2SymbolConverter.cpp - API2Symbol Converter ---------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements API2Symbol Converter
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/API2SymbolConverter.h"

#include "clang/AST/DeclObjC.h"

using namespace llvm;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

namespace {
SymbolFlags getFlagsFromRecord(const APIRecord &record) {
  auto flag = SymbolFlags::None;
  if (record.isWeakDefined())
    flag |= SymbolFlags::WeakDefined;
  if (record.isThreadLocalValue())
    flag |= SymbolFlags::ThreadLocalValue;
  if (record.isReexported())
    flag |= SymbolFlags::Rexported;

  if (record.isExternal())
    flag |= SymbolFlags::Undefined;
  if (record.isWeakReferenced())
    flag |= SymbolFlags::WeakReferenced;

  if (record.isText())
    flag |= SymbolFlags::Text;
  else
    flag |= SymbolFlags::Data;

  return flag;
}
} // namespace

void API2SymbolConverter::visitGlobal(GlobalRecord &record) {

  // Skip non exported symbols unless for flat namespace symbols.
  if (!record.isExported()) {
    if (recordUndefs && record.isExternal()) {
      record.flags = getFlagsFromRecord(record);
      symbolSet->addGlobal(SymbolKind::GlobalSymbol, record.name, record.flags,
                           target);
    }
    return;
  }

  auto sym = parseSymbol(record.name);
  record.name = sym.name;
  record.flags = getFlagsFromRecord(record);
  symbolSet->addGlobal(sym.kind, record.name, record.flags, target);
}

void API2SymbolConverter::visitObjCInterface(ObjCInterfaceRecord &record) {
  if (record.isExported()) {
    record.flags = getFlagsFromRecord(record);
    record.linkage = APILinkage::Exported;
    symbolSet->addGlobal(SymbolKind::ObjectiveCClass, record.name, record.flags,
                         target);
    if (record.hasExceptionAttribute)
      symbolSet->addGlobal(SymbolKind::ObjectiveCClassEHType, record.name,
                           record.flags, target);
  }

  auto addIvars = [&](ArrayRef<ObjCInstanceVariableRecord *> ivars) {
    for (auto *ivar : ivars) {
      if (!ivar->isExported())
        continue;
      // ObjC has an additional mechanism to specify if an ivar is exported or
      // not.
      if (ivar->accessControl == ObjCIvarDecl::Private ||
          ivar->accessControl == ObjCIvarDecl::Package)
        continue;
      std::string name =
          ObjCInstanceVariableRecord::createName(record.name, ivar->name);
      ivar->flags = getFlagsFromRecord(*ivar);
      symbolSet->addGlobal(SymbolKind::ObjectiveCInstanceVariable, name,
                           ivar->flags, target);
    }
  };
  addIvars(record.ivars);

  for (auto *category : record.categories) {
    addIvars(category->ivars);
  }
}

void API2SymbolConverter::visitObjCCategory(ObjCCategoryRecord &record) {
  auto addIvars = [&](ArrayRef<ObjCInstanceVariableRecord *> ivars) {
    for (auto *ivar : ivars) {
      if (!ivar->isExported())
        continue;
      // ObjC has an additional mechanism to specify if an ivar is exported or
      // not.
      if (ivar->accessControl == ObjCIvarDecl::Private ||
          ivar->accessControl == ObjCIvarDecl::Package)
        continue;
      std::string name =
          ObjCInstanceVariableRecord::createName(record.name, ivar->name);
      ivar->flags = getFlagsFromRecord(*ivar);
      symbolSet->addGlobal(SymbolKind::ObjectiveCInstanceVariable, name,
                           ivar->flags, target);
    }
  };

  addIvars(record.ivars);
}

TAPI_NAMESPACE_INTERNAL_END
