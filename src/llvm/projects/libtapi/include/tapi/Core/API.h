//===- tapi/Core/API.h - TAPI API -------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the API.
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_API_H
#define TAPI_CORE_API_H

#include "tapi/Core/AvailabilityInfo.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Defines.h"
#include "clang/AST/DeclObjC.h"
#include "clang/Basic/SourceLocation.h"
#include "llvm/ADT/MapVector.h"
#include "llvm/ADT/StringRef.h"

using clang::Decl;

TAPI_NAMESPACE_INTERNAL_BEGIN

class APIVisitor;

class API {
public:
  using APILoc = clang::PresumedLoc;

  enum class APIAccess {
    Public,
    Private,
    Project,
  };

  struct APIRecord {
    StringRef name;
    APILoc loc;
    AvailabilityInfo availability;
    APIAccess access;
    const Decl *decl;
  };

  struct EnumConstantRecord : APIRecord {
    EnumConstantRecord(StringRef name, APILoc loc,
                       const AvailabilityInfo &availability, APIAccess access,
                       const Decl *decl)
        : APIRecord({name, loc, availability, access, decl}) {}
    static EnumConstantRecord *create(llvm::BumpPtrAllocator &allocator,
                                      StringRef name, APILoc loc,
                                      const AvailabilityInfo &availability,
                                      APIAccess access, const Decl *decl);
  };

  struct GlobalRecord : APIRecord {
    bool isWeakDefined;

    GlobalRecord(StringRef name, APILoc loc,
                 const AvailabilityInfo &availability, APIAccess access,
                 const Decl *decl, bool isWeakDefined)
        : APIRecord({name, loc, availability, access, decl}),
          isWeakDefined(isWeakDefined) {}
    static GlobalRecord *create(llvm::BumpPtrAllocator &allocator,
                                StringRef name, APILoc loc,
                                const AvailabilityInfo &availability,
                                APIAccess access, const Decl *decl,
                                bool isWeakDefined);
  };

  struct FunctionRecord : GlobalRecord {
    bool isInlined;

    FunctionRecord(StringRef name, APILoc loc,
                   const AvailabilityInfo &availability, APIAccess access,
                   const Decl *decl, bool isWeakDefined, bool isInlined)
        : GlobalRecord(name, loc, availability, access, decl, isWeakDefined),
          isInlined(isInlined) {}
    static FunctionRecord *create(llvm::BumpPtrAllocator &allocator,
                                  StringRef name, APILoc loc,
                                  const AvailabilityInfo &availability,
                                  APIAccess access, const Decl *decl,
                                  bool isWeakDefined, bool isInlined);
  };

  struct ObjCPropertyRecord : APIRecord {
    using PropertyAttributeKind =
        clang::ObjCPropertyDecl::PropertyAttributeKind;
    PropertyAttributeKind attributes;
    StringRef getterName;
    StringRef setterName;
    bool isOptional;

    ObjCPropertyRecord(StringRef name, StringRef getterName,
                       StringRef setterName, APILoc loc,
                       const AvailabilityInfo &availability, APIAccess access,
                       PropertyAttributeKind attributes, bool isOptional,
                       const Decl *decl)
        : APIRecord({name, loc, availability, access, decl}),
          attributes(attributes), getterName(getterName),
          setterName(setterName), isOptional(isOptional) {}

    static ObjCPropertyRecord *
    create(llvm::BumpPtrAllocator &allocator, StringRef name,
           StringRef getterName, StringRef setterName, APILoc loc,
           const AvailabilityInfo &availability, APIAccess access,
           PropertyAttributeKind attributes, bool isOptional, const Decl *decl);
  };

  struct ObjCInstanceVariableRecord : APIRecord {
    using AccessControl = clang::ObjCIvarDecl::AccessControl;
    AccessControl accessControl;
    bool isFragile;

    ObjCInstanceVariableRecord(StringRef name, APILoc loc,
                               const AvailabilityInfo &availability,
                               APIAccess access, AccessControl accessControl,
                               bool isFragile, const Decl *decl)
        : APIRecord({name, loc, availability, access, decl}),
          accessControl(accessControl), isFragile(isFragile) {}

    static ObjCInstanceVariableRecord *
    create(llvm::BumpPtrAllocator &allocator, StringRef name, APILoc loc,
           const AvailabilityInfo &availability, APIAccess access,
           AccessControl accessControl, bool isFragile, const Decl *decl);
  };

  struct ObjCMethodRecord : APIRecord {
    bool isInstanceMethod;
    bool isOptional;
    bool isDynamic;

    ObjCMethodRecord(StringRef name, APILoc loc,
                     const AvailabilityInfo &availability, APIAccess access,
                     bool isInstanceMethod, bool isOptional, bool isDynamic,
                     const Decl *decl)
        : APIRecord({name, loc, availability, access, decl}),
          isInstanceMethod(isInstanceMethod), isOptional(isOptional),
          isDynamic(isDynamic) {}

    static ObjCMethodRecord *create(llvm::BumpPtrAllocator &allocator,
                                    StringRef name, APILoc loc,
                                    const AvailabilityInfo &availability,
                                    APIAccess access, bool isInstanceMethod,
                                    bool isOptional, bool isDynamic,
                                    const Decl *decl);
  };

  struct ObjCProtocolRecord;
  struct ObjCContainerRecord : APIRecord {
    std::vector<const ObjCMethodRecord *> methods;
    std::vector<const ObjCPropertyRecord *> properties;
    std::vector<const ObjCInstanceVariableRecord *> ivars;
    std::vector<StringRef> protocols;

    ObjCContainerRecord(StringRef name, APILoc loc,
                        const AvailabilityInfo &availability, APIAccess access,
                        const Decl *decl)
        : APIRecord({name, loc, availability, access, decl}) {}
  };

  struct ObjCCategoryRecord : ObjCContainerRecord {
    StringRef interfaceName;

    ObjCCategoryRecord(StringRef interfaceName, StringRef name, APILoc loc,
                       const AvailabilityInfo &availability, APIAccess access,
                       const Decl *decl)
        : ObjCContainerRecord(name, loc, availability, access, decl),
          interfaceName(interfaceName) {}

    static ObjCCategoryRecord *create(llvm::BumpPtrAllocator &allocator,
                                      StringRef interfaceName, StringRef name,
                                      APILoc loc,
                                      const AvailabilityInfo &availability,
                                      APIAccess access, const Decl *decl);
  };

  struct ObjCProtocolRecord : ObjCContainerRecord {
    ObjCProtocolRecord(StringRef name, APILoc loc,
                       const AvailabilityInfo &availability, APIAccess access,
                       const Decl *decl)
        : ObjCContainerRecord(name, loc, availability, access, decl) {}

    static ObjCProtocolRecord *create(llvm::BumpPtrAllocator &allocator,
                                      StringRef name, APILoc loc,
                                      const AvailabilityInfo &availability,
                                      APIAccess access, const Decl *decl);
  };

  struct ObjCInterfaceRecord : ObjCContainerRecord {
    std::vector<const ObjCCategoryRecord *> categories;
    StringRef superClassName;
    bool isExported;
    bool hasExceptionAttribute = false;

    ObjCInterfaceRecord(StringRef name, APILoc loc,
                        const AvailabilityInfo &availability, APIAccess access,
                        bool isExported, StringRef superClassName,
                        const Decl *decl)
        : ObjCContainerRecord(name, loc, availability, access, decl),
          superClassName(superClassName), isExported(isExported) {}

    static ObjCInterfaceRecord *
    create(llvm::BumpPtrAllocator &allocator, StringRef name, APILoc loc,
           const AvailabilityInfo &availability, APIAccess access,
           bool isExported, StringRef superClassName, const Decl *decl);
  };

  EnumConstantRecord *addEnumConstant(StringRef name, APILoc loc,
                                      const AvailabilityInfo &availability,
                                      APIAccess access, const Decl *decl);
  GlobalRecord *addGlobalVariable(StringRef name, APILoc loc,
                                  const AvailabilityInfo &availability,
                                  APIAccess access, const Decl *decl,
                                  bool isWeakDefined = false);
  FunctionRecord *addFunction(StringRef name, APILoc loc,
                              const AvailabilityInfo &availability,
                              APIAccess access, const Decl *decl,
                              bool isWeakDefined = false,
                              bool isInlined = false);
  ObjCInterfaceRecord *addObjCInterface(StringRef name, APILoc loc,
                                        const AvailabilityInfo &availability,
                                        APIAccess access, bool isExported,
                                        StringRef superClassName,
                                        const Decl *decl);
  ObjCCategoryRecord *addObjCCategory(StringRef interfaceName, StringRef name,
                                      APILoc loc,
                                      const AvailabilityInfo &availability,
                                      APIAccess access, const Decl *decl);
  ObjCProtocolRecord *addObjCProtocol(StringRef name, APILoc loc,
                                      const AvailabilityInfo &availability,
                                      APIAccess access, const Decl *decl);
  ObjCMethodRecord *addObjCMethod(ObjCContainerRecord *record, StringRef name,
                                  APILoc loc,
                                  const AvailabilityInfo &availability,
                                  APIAccess access, bool isInstanceMethod,
                                  bool isOptional, bool isDynamic,
                                  const Decl *decl);
  ObjCPropertyRecord *
  addObjCProperty(ObjCContainerRecord *record, StringRef name,
                  StringRef getterName, StringRef setterName, APILoc loc,
                  const AvailabilityInfo &availability, APIAccess access,
                  ObjCPropertyRecord::PropertyAttributeKind attributes,
                  bool isOptional, const Decl *decl);
  ObjCInstanceVariableRecord *addObjCInstanceVariable(
      ObjCContainerRecord *record, StringRef name, APILoc loc,
      const AvailabilityInfo &availability, APIAccess access,
      ObjCInstanceVariableRecord::AccessControl accessControl, bool isFragile,
      const Decl *decl);

  GlobalRecord *addTypeDef(StringRef name, APILoc loc,
                           const AvailabilityInfo &availability,
                           APIAccess access, const Decl *decl);

  void visit(APIVisitor &visitor) const;

  const GlobalRecord *findGlobalVariable(StringRef) const;
  const FunctionRecord *findFunction(StringRef name) const;
  const GlobalRecord *findTypeDef(StringRef) const;
  const EnumConstantRecord *findEnumConstant(StringRef name) const;
  const ObjCInterfaceRecord *findObjCInterface(StringRef) const;
  const ObjCProtocolRecord *findObjCProtocol(StringRef) const;
  const ObjCCategoryRecord *findObjCCategory(StringRef, StringRef) const;

private:
  StringRef copyString(StringRef string);

  using GlobalRecordMap = llvm::MapVector<StringRef, GlobalRecord *>;
  using FunctionRecordMap = llvm::MapVector<StringRef, FunctionRecord *>;
  using EnumConstantRecordMap =
      llvm::MapVector<StringRef, EnumConstantRecord *>;
  using ObjCInterfaceRecordMap =
      llvm::MapVector<StringRef, ObjCInterfaceRecord *>;
  using ObjCCategoryRecordMap =
      llvm::MapVector<std::pair<StringRef, StringRef>, ObjCCategoryRecord *>;
  using ObjCProtocolRecordMap =
      llvm::MapVector<StringRef, ObjCProtocolRecord *>;

  llvm::BumpPtrAllocator allocator;
  GlobalRecordMap globalVariables;
  FunctionRecordMap functions;
  EnumConstantRecordMap enumConstants;
  ObjCInterfaceRecordMap interfaces;
  ObjCCategoryRecordMap categories;
  ObjCProtocolRecordMap protocols;
  GlobalRecordMap typeDefs;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_API_H
