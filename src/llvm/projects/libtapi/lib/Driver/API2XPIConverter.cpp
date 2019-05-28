//===- lib/Driver/API2XPIConverter.cpp - API2XPI Converter ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements API2XPI Converter
///
//===----------------------------------------------------------------------===//

#include "API2XPIConverter.h"
#include "clang/AST/DeclObjC.h"

using namespace llvm;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

static XPIAccess convertAccess(API::APIAccess access) {
  switch (access) {
  case API::APIAccess::Public:
    return XPIAccess::Public;
  case API::APIAccess::Private:
    return XPIAccess::Private;
  case API::APIAccess::Project:
    return XPIAccess::Project;
  }
}

void API2XPIConverter::visitGlobalVariable(const API::GlobalRecord &record) {
  auto nameRef = record.name;
  if (nameRef.consume_front(".objc_class_name_") ||
      nameRef.consume_front("_OBJC_CLASS_$_") ||
      nameRef.consume_front("_OBJC_METACLASS_$_"))
    xpiSet->addObjCClass(nameRef, record.loc, convertAccess(record.access),
                         architecture, record.availability);
  else if (nameRef.consume_front("_OBJC_EHTYPE_$_"))
    xpiSet->addObjCClassEHType(nameRef, record.loc,
                               convertAccess(record.access), architecture,
                               record.availability);
  else if (nameRef.consume_front("_OBJC_IVAR_$_"))
    xpiSet->addObjCInstanceVariable(nameRef, record.loc,
                                    convertAccess(record.access), architecture,
                                    record.availability);
  else
    xpiSet->addGlobalSymbol(record.name, record.loc,
                            convertAccess(record.access), architecture,
                            record.availability, record.isWeakDefined);
}

void API2XPIConverter::visitFunction(const API::FunctionRecord &record) {
  if (record.isInlined)
    return;

  xpiSet->addGlobalSymbol(record.name, record.loc, convertAccess(record.access),
                          architecture, record.availability,
                          record.isWeakDefined);
}

static void addSelectorsFromObjCRecord(XPISet *xpi, Architecture architecture,
                                       const API::ObjCContainerRecord *record,
                                       ObjCContainer *container) {
  for (const auto *method : record->methods) {
    auto access = convertAccess(method->access);
    xpi->addObjCSelector(container, method->name, method->isInstanceMethod,
                         method->isDynamic, method->loc, access, architecture,
                         method->availability);
  }
  for (const auto *property : record->properties) {
    auto access = convertAccess(property->access);
    xpi->addObjCSelector(container, property->getterName,
                         /*isInstanceMethod*/ true,
                         /* isDynamic */ false, property->loc, access,
                         architecture, property->availability);
    if (!(property->attributes & ObjCPropertyDecl::OBJC_PR_readonly))
      xpi->addObjCSelector(container, property->setterName,
                           /*isInstanceMethod*/ true,
                           /* isDynamic */ false, property->loc, access,
                           architecture, property->availability);
  }
}

void API2XPIConverter::visitObjCInterface(
    const API::ObjCInterfaceRecord &record) {
  auto access = convertAccess(record.access);
  if (!record.isExported)
    access = XPIAccess::Internal;

  auto cls = xpiSet->addObjCClass(record.name, record.loc, access, architecture,
                                  record.availability);
  if (record.hasExceptionAttribute)
    xpiSet->addObjCClassEHType(record.name, record.loc, access, architecture,
                               record.availability);

  auto addIvars = [&](ArrayRef<const API::ObjCInstanceVariableRecord *> ivars) {
    for (const auto *ivar : ivars) {
      if (ivar->isFragile)
        continue;
      // ObjC has an additional mechanism to specify if an ivar is exported or
      // not.
      if (ivar->accessControl == ObjCIvarDecl::Private ||
          ivar->accessControl == ObjCIvarDecl::Package)
        continue;
      std::string name = (record.name + "." + ivar->name).str();
      xpiSet->addObjCInstanceVariable(name, ivar->loc,
                                      convertAccess(ivar->access), architecture,
                                      ivar->availability);
    }
  };
  addIvars(record.ivars);

  for (const auto *category : record.categories)
    addIvars(category->ivars);

  if (!visitAllObjC)
    return;

  addSelectorsFromObjCRecord(xpiSet, architecture, &record, cls);
}

void API2XPIConverter::visitObjCCategory(
    const API::ObjCCategoryRecord &record) {
  auto addIvars = [&](ArrayRef<const API::ObjCInstanceVariableRecord *> ivars) {
    for (const auto *ivar : ivars) {
      if (ivar->isFragile)
        continue;
      // ObjC has an additional mechanism to specify if an ivar is exported or
      // not.
      if (ivar->accessControl == ObjCIvarDecl::Private ||
          ivar->accessControl == ObjCIvarDecl::Package)
        continue;
      std::string name = (record.interfaceName + "." + ivar->name).str();
      xpiSet->addObjCInstanceVariable(name, ivar->loc,
                                      convertAccess(ivar->access), architecture,
                                      ivar->availability);
    }
  };
  addIvars(record.ivars);

  if (!visitAllObjC)
    return;

  auto access = convertAccess(record.access);
  auto baseCls =
      xpiSet->findSymbol(XPIKind::ObjectiveCClass, record.interfaceName);
  // If the class is only a forward decl or it is in system header, create
  // a stub on it.
  if (!baseCls)
    baseCls = xpiSet->addObjCClass(record.interfaceName, record.loc,
                                   XPIAccess::Unknown, architecture,
                                   AvailabilityInfo());

  // FIXME: const_cast is temporary until we remove XPISet.
  auto clsPtr = const_cast<ObjCClass *>(cast<ObjCClass>(baseCls));
  auto category =
      xpiSet->addObjCCategory(clsPtr, record.name, record.loc, access,
                              architecture, record.availability);
  addSelectorsFromObjCRecord(xpiSet, architecture, &record, category);
}

void API2XPIConverter::visitObjCProtocol(
    const API::ObjCProtocolRecord &record) {
  if (!visitAllObjC)
    return;
  auto access = convertAccess(record.access);

  auto protocol = xpiSet->addObjCProtocol(record.name, record.loc, access,
                                          architecture, record.availability);
  addSelectorsFromObjCRecord(xpiSet, architecture, &record, protocol);
}

TAPI_NAMESPACE_INTERNAL_END
