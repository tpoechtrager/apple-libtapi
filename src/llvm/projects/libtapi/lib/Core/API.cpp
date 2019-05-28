//===- lib/Core/API.cpp - TAPI API ------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "tapi/Core/API.h"
#include "tapi/Core/APIVisitor.h"

using namespace llvm;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

API::GlobalRecord *
API::GlobalRecord::create(BumpPtrAllocator &allocator, StringRef name,
                          APILoc loc, const AvailabilityInfo &availability,
                          APIAccess access, const Decl *decl,
                          bool isWeakDefined) {
  return new (allocator)
      GlobalRecord{name, loc, availability, access, decl, isWeakDefined};
}

API::FunctionRecord *
API::FunctionRecord::create(BumpPtrAllocator &allocator, StringRef name,
                            APILoc loc, const AvailabilityInfo &availability,
                            APIAccess access, const Decl *decl,
                            bool isWeakDefined, bool isInlined) {
  return new (allocator) FunctionRecord{
      name, loc, availability, access, decl, isWeakDefined, isInlined};
}

API::EnumConstantRecord *API::EnumConstantRecord::create(
    BumpPtrAllocator &allocator, StringRef name, APILoc loc,
    const AvailabilityInfo &availability, APIAccess access, const Decl *decl) {
  return new (allocator)
      EnumConstantRecord{name, loc, availability, access, decl};
}

API::ObjCMethodRecord *API::ObjCMethodRecord::create(
    BumpPtrAllocator &allocator, StringRef name, APILoc loc,
    const AvailabilityInfo &availability, APIAccess access,
    bool isInstanceMethod, bool isOptional, bool isDynamic, const Decl *decl) {
  return new (allocator) ObjCMethodRecord{
      name,       loc,       availability, access, isInstanceMethod,
      isOptional, isDynamic, decl};
}

API::ObjCPropertyRecord *API::ObjCPropertyRecord::create(
    BumpPtrAllocator &allocator, StringRef name, StringRef getterName,
    StringRef setterName, APILoc loc, const AvailabilityInfo &availability,
    APIAccess access, PropertyAttributeKind attributes, bool isOptional,
    const Decl *decl) {
  return new (allocator)
      ObjCPropertyRecord{name,   getterName, setterName, loc, availability,
                         access, attributes, isOptional, decl};
}

API::ObjCInstanceVariableRecord *API::ObjCInstanceVariableRecord::create(
    BumpPtrAllocator &allocator, StringRef name, APILoc loc,
    const AvailabilityInfo &availability, APIAccess access,
    AccessControl accessControl, bool isFragile, const Decl *decl) {
  return new (allocator) ObjCInstanceVariableRecord{
      name, loc, availability, access, accessControl, isFragile, decl};
}

API::ObjCInterfaceRecord *API::ObjCInterfaceRecord::create(
    BumpPtrAllocator &allocator, StringRef name, APILoc loc,
    const AvailabilityInfo &availability, APIAccess access, bool isExported,
    StringRef superClassName, const Decl *decl) {
  return new (allocator) ObjCInterfaceRecord{
      name, loc, availability, access, isExported, superClassName, decl};
}

API::ObjCCategoryRecord *API::ObjCCategoryRecord::create(
    BumpPtrAllocator &allocator, StringRef interfaceName, StringRef name,
    APILoc loc, const AvailabilityInfo &availability, APIAccess access,
    const Decl *decl) {
  return new (allocator)
      ObjCCategoryRecord{interfaceName, name, loc, availability, access, decl};
}

API::ObjCProtocolRecord *API::ObjCProtocolRecord::create(
    BumpPtrAllocator &allocator, StringRef name, APILoc loc,
    const AvailabilityInfo &availability, APIAccess access, const Decl *decl) {
  return new (allocator)
      ObjCProtocolRecord{name, loc, availability, access, decl};
}

API::GlobalRecord *API::addGlobalVariable(StringRef name, APILoc loc,
                                          const AvailabilityInfo &availability,
                                          APIAccess access, const Decl *decl,
                                          bool isWeakDefined) {
  name = copyString(name);
  auto result = globalVariables.insert({name, nullptr});
  if (result.second) {
    auto *record = GlobalRecord::create(allocator, name, loc, availability,
                                        access, decl, isWeakDefined);
    result.first->second = record;
  }
  return result.first->second;
}

API::FunctionRecord *API::addFunction(StringRef name, APILoc loc,
                                      const AvailabilityInfo &availability,
                                      APIAccess access, const Decl *decl,
                                      bool isWeakDefined, bool isInlined) {
  name = copyString(name);
  auto result = functions.insert({name, nullptr});
  if (result.second) {
    auto *record =
        FunctionRecord::create(allocator, name, loc, availability, access, decl,
                               isWeakDefined, isInlined);
    result.first->second = record;
  } else {
    if (!isInlined)
      result.first->second->isInlined = false;
  }
  return result.first->second;
}

API::EnumConstantRecord *
API::addEnumConstant(StringRef name, APILoc loc,
                     const AvailabilityInfo &availability, APIAccess access,
                     const Decl *decl) {
  name = copyString(name);
  auto result = enumConstants.insert({name, nullptr});
  if (result.second) {
    auto *record = EnumConstantRecord::create(allocator, name, loc,
                                              availability, access, decl);
    result.first->second = record;
  }
  return result.first->second;
}

API::ObjCInterfaceRecord *
API::addObjCInterface(StringRef name, APILoc loc,
                      const AvailabilityInfo &availability, APIAccess access,
                      bool isExported, StringRef superClassName,
                      const Decl *decl) {
  name = copyString(name);
  auto result = interfaces.insert({name, nullptr});
  if (result.second) {
    superClassName = copyString(superClassName);
    auto *record =
        ObjCInterfaceRecord::create(allocator, name, loc, availability, access,
                                    isExported, superClassName, decl);
    result.first->second = record;
  }
  return result.first->second;
}

API::ObjCCategoryRecord *
API::addObjCCategory(StringRef interfaceName, StringRef name, APILoc loc,
                     const AvailabilityInfo &availability, APIAccess access,
                     const Decl *decl) {
  interfaceName = copyString(interfaceName);
  name = copyString(name);
  auto result =
      categories.insert({std::make_pair(interfaceName, name), nullptr});
  if (result.second) {
    auto *record = ObjCCategoryRecord::create(allocator, interfaceName, name,
                                              loc, availability, access, decl);
    result.first->second = record;
  }

  auto it = interfaces.find(interfaceName);
  if (it != interfaces.end())
    it->second->categories.push_back(result.first->second);

  return result.first->second;
}

API::ObjCProtocolRecord *
API::addObjCProtocol(StringRef name, APILoc loc,
                     const AvailabilityInfo &availability, APIAccess access,
                     const Decl *decl) {
  name = copyString(name);
  auto result = protocols.insert({name, nullptr});
  if (result.second) {
    auto *record = ObjCProtocolRecord::create(allocator, name, loc,
                                              availability, access, decl);
    result.first->second = record;
  }

  return result.first->second;
}

API::ObjCMethodRecord *
API::addObjCMethod(API::ObjCContainerRecord *record, StringRef name, APILoc loc,
                   const AvailabilityInfo &availability, APIAccess access,
                   bool isInstanceMethod, bool isOptional, bool isDynamic,
                   const Decl *decl) {
  name = copyString(name);
  auto *method =
      ObjCMethodRecord::create(allocator, name, loc, availability, access,
                               isInstanceMethod, isOptional, isDynamic, decl);
  record->methods.push_back(method);
  return method;
}

API::ObjCPropertyRecord *
API::addObjCProperty(API::ObjCContainerRecord *record, StringRef name,
                     StringRef getterName, StringRef setterName, APILoc loc,
                     const AvailabilityInfo &availability, APIAccess access,
                     ObjCPropertyRecord::PropertyAttributeKind attributes,
                     bool isOptional, const Decl *decl) {
  name = copyString(name);
  getterName = copyString(getterName);
  setterName = copyString(setterName);
  auto *property = ObjCPropertyRecord::create(
      allocator, name, getterName, setterName, loc, availability, access,
      attributes, isOptional, decl);
  record->properties.push_back(property);
  return property;
}

API::ObjCInstanceVariableRecord *API::addObjCInstanceVariable(
    API::ObjCContainerRecord *record, StringRef name, APILoc loc,
    const AvailabilityInfo &availability, APIAccess access,
    ObjCInstanceVariableRecord::AccessControl accessControl, bool isFragile,
    const Decl *decl) {
  name = copyString(name);
  auto *ivar = ObjCInstanceVariableRecord::create(
      allocator, name, loc, availability, access, accessControl, isFragile,
      decl);
  record->ivars.push_back(ivar);
  return ivar;
}

API::GlobalRecord *API::addTypeDef(StringRef name, APILoc loc,
                                   const AvailabilityInfo &availability,
                                   APIAccess access, const Decl *decl) {
  name = copyString(name);
  auto result = typeDefs.insert({name, nullptr});
  if (result.second) {
    auto *record = GlobalRecord::create(allocator, name, loc, availability,
                                        access, decl, /*isWeakDefined=*/false);
    result.first->second = record;
  }
  return result.first->second;
}

const API::GlobalRecord *API::findGlobalVariable(StringRef name) const {
  auto it = globalVariables.find(name);
  if (it != globalVariables.end())
    return it->second;
  return nullptr;
}

const API::FunctionRecord *API::findFunction(StringRef name) const {
  auto it = functions.find(name);
  if (it != functions.end())
    return it->second;
  return nullptr;
}

const API::GlobalRecord *API::findTypeDef(StringRef name) const {
  auto it = typeDefs.find(name);
  if (it != typeDefs.end())
    return it->second;
  return nullptr;
}

const API::EnumConstantRecord *API::findEnumConstant(StringRef name) const {
  auto it = enumConstants.find(name);
  if (it != enumConstants.end())
    return it->second;
  return nullptr;
}

const API::ObjCInterfaceRecord *API::findObjCInterface(StringRef name) const {
  auto it = interfaces.find(name);
  if (it != interfaces.end())
    return it->second;
  return nullptr;
}

const API::ObjCProtocolRecord *API::findObjCProtocol(StringRef name) const {
  auto it = protocols.find(name);
  if (it != protocols.end())
    return it->second;
  return nullptr;
}

const API::ObjCCategoryRecord *API::findObjCCategory(StringRef interfaceName,
                                                     StringRef name) const {
  auto it = categories.find({interfaceName, name});
  if (it != categories.end())
    return it->second;
  return nullptr;
}

void API::visit(APIVisitor &visitor) const {
  for (auto &it : typeDefs)
    visitor.visitTypeDef(*it.second);
  for (auto &it : globalVariables)
    visitor.visitGlobalVariable(*it.second);
  for (auto &it : functions)
    visitor.visitFunction(*it.second);
  for (auto &it : enumConstants)
    visitor.visitEnumConstant(*it.second);
  for (auto &it : protocols)
    visitor.visitObjCProtocol(*it.second);
  for (auto &it : interfaces)
    visitor.visitObjCInterface(*it.second);
  for (auto &it : categories)
    visitor.visitObjCCategory(*it.second);
}

StringRef API::copyString(StringRef string) {
  if (string.empty())
    return {};

  void *ptr = allocator.Allocate(string.size(), 1);
  memcpy(ptr, string.data(), string.size());
  return StringRef(reinterpret_cast<const char *>(ptr), string.size());
}

TAPI_NAMESPACE_INTERNAL_END
