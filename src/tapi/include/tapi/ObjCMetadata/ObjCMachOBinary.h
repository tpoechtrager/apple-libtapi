//===- ObjCMachOBinary.h - ObjC MachO Metadata Interface ---------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares ObjCMachOBinary interface.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_OBJCMETADATA_OBJCMACHOBINARY_H
#define LLVM_OBJCMETADATA_OBJCMACHOBINARY_H

#include <llvm/Object/MachO.h>
#include <tapi/ObjCMetadata/ObjCMetadata.h>

#include <unordered_map>

namespace llvm {

class MachOMetadata : public ObjCMetaDataReader {
public:
  MachOMetadata(object::MachOObjectFile *Binary, Error& Err);

  object::MachOObjectFile *getMachOObject() {
    return cast<object::MachOObjectFile>(OwningBinary);
  }

  bool isObjC1() const override { return ObjCVersion == ObjC1; }
  bool isObjC2() const override { return ObjCVersion == ObjC2; }
  Expected<StringRef> getSwiftVersion() const override;

  const object::MachOObjectFile *getOwningBinary() const {
    return OwningBinary;
  }

  // Overrides ObjC Class Ref Implementation.
  Expected<ObjCClassList> classes() const override;
  Expected<ObjCClass> getObjCClassFromRef(ObjCClassRef Ref) const override;
  bool isObjCClassExternal(ObjCClassRef Ref) const override;

  // Overrides ObjC Category Ref Implementation.
  Expected<ObjCCategoryList> categories() const override;
  Expected<ObjCCategory>
  getObjCCategoryFromRef(ObjCCategoryRef Ref) const override;

  // Overrides ObjC Protocol Ref Implementation.
  Expected<ObjCProtocolList> protocols() const override;
  Expected<ObjCProtocol>
  getObjCProtocolFromRef(ObjCProtocolRef Ref) const override;

  // Overrides ObjC Class Implementation.
  Expected<StringRef> getObjCClassName(ObjCClass ClassRef) const override;
  Expected<StringRef> getObjCSuperClassName(ObjCClass Data) const override;
  Expected<bool> isObjCClassSwift(ObjCClass Data) const override;
  Expected<bool> isObjCClassMetaClass(ObjCClass Data) const override;
  Expected<ObjCPropertyList>
  materializePropertyList(ObjCClass Data) const override;
  Expected<ObjCPropertyList>
  materializeClassPropertyList(ObjCClass Data) const override;
  Expected<ObjCMethodList>
  materializeInstanceMethodList(ObjCClass Data) const override;
  Expected<ObjCMethodList>
  materializeClassMethodList(ObjCClass Data) const override;

  // Overrides ObjC Category Implementation.
  Expected<StringRef> getObjCCategoryName(ObjCCategory Data) const override;
  Expected<StringRef>
  getObjCCategoryBaseClassName(ObjCCategory Data) const override;
  Expected<ObjCPropertyList>
  materializePropertyList(ObjCCategory Data) const override;
  Expected<ObjCMethodList>
  materializeInstanceMethodList(ObjCCategory Data) const override;
  Expected<ObjCMethodList>
  materializeClassMethodList(ObjCCategory Data) const override;

  // Overrides ObjC Protocol Implementation.
  Expected<StringRef> getObjCProtocolName(ObjCProtocol Data) const override;
  Expected<ObjCPropertyList>
  materializePropertyList(ObjCProtocol Data) const override;
  Expected<ObjCMethodList>
  materializeInstanceMethodList(ObjCProtocol Data) const override;
  Expected<ObjCMethodList>
  materializeClassMethodList(ObjCProtocol Data) const override;
  Expected<ObjCMethodList>
  materializeOptionalInstanceMethodList(ObjCProtocol Data) const override;
  Expected<ObjCMethodList>
  materializeOptionalClassMethodList(ObjCProtocol Data) const override;

  // Override ObjC Property Implementation.
  Expected<StringRef> getPropertyName(ObjCProperty Data) const override;
  Expected<StringRef> getPropertyAttribute(ObjCProperty Data) const override;

  // Override ObjC Method Implementation.
  Expected<StringRef> getMethodName(ObjCMethod Data) const override;
  Expected<StringRef> getMethodType(ObjCMethod Data) const override;

  // Override ObjC Selector Ref Implementation.
  Expected<ObjCSelectorList> referencedSelectors() const override;
  Expected<StringRef> getObjCSelectorName(ObjCSelectorRef Ref) const override;

  // Override Potentially defined selectors Implemetation.
  void getAllPotentiallyDefinedSelectors(StringSet<> &Set) const override;

  // Other.
  StringRef getSymbolNameFromRef(ObjCRef Ref) const override;
  StringRef guessClassNameBasedOnSymbol(StringRef Sym) const override;

  // Helper classes
  const object::SectionRef getSection(const char *segname,
                                      const char *sectname) const;
  Expected<object::SectionRef> getSectionFromAddress(uint64_t Address) const;
  bool isAddressEncrypted(uint64_t Address) const;

  // ObjC2 helper classes
  Expected<ObjCRef> getObjC2ClassRO64(ObjCClass Data) const;
  Expected<ObjCRef> getObjC2ClassRO32(ObjCClass Data) const;
  Expected<ObjCClass> getObjC2MetaClass64(ObjCClass Data) const;
  Expected<ObjCClass> getObjC2MetaClass32(ObjCClass Data) const;
  // ObjC1 helper classes
  Expected<ObjCRefList> getObjC1Modules() const;
  Expected<ObjCRef> getObjC1Symtab(ObjCRef ObjCModule) const;
  Expected<ObjCClassList> getObjC1ClassesFromSymtab(ObjCRef Symtab) const;
  Expected<ObjCCategoryList> getObjC1CategoriesFromSymtab(ObjCRef Symtab) const;
  Expected<ObjCMethodList> getObjC1MethodList(ObjCRef Methods) const;

  // Get symbol from VMAddr. If there is no symbol at vmaddr, return empty
  // StringRef.
  StringRef getSymbol(uint64_t VMAddr) const;
  // Get data from VMAddr. Caller needs to make sure there are no
  // relocations/rebases/binds in range.
  template <typename T> Expected<T> getData(uint64_t VMAddr) const;
  // Get string from VMAddr. Caller needs to make sure there are no
  // relocations/rebases/binds in range.
  Expected<StringRef> getString(uint64_t VMAddr, bool AllowEmpty = false) const;
  // Get pointer value from VMAddr.
  Expected<uint64_t> getPointerValue64(uint64_t VMAddr) const;
  Expected<uint32_t> getPointerValue32(uint64_t VMAddr) const;

private:
  object::MachOObjectFile *OwningBinary;
  enum ObjCRuntimeVersion {
    Unknown = 0,
    ObjC1 = 1,
    ObjC2 = 2
  };
  ObjCRuntimeVersion ObjCVersion;

  // Cache all RawPointer has a name associated with it.
  std::unordered_map<uint64_t, StringRef> VMAddrToSymbolMap;
  // Cache all RawPointer that has a VM Addr associcated with it.
  std::unordered_map<uint64_t, uint64_t> VMAddrPointToValueMap;
};

}

#endif
