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
#include <llvm/ObjCMetadata/ObjCMetadata.h>

namespace llvm {

class MachOMetadata : public ObjCMetaDataReader {
public:
  MachOMetadata(const object::MachOObjectFile *Binary)
      : OwningBinary(Binary) {}

  const object::MachOObjectFile *getMachOObject() {
    return cast<object::MachOObjectFile>(OwningBinary);
  }

  bool isObjC1() const override;
  bool isObjC2() const override;

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

  Expected<StringRef> getValueFromAddress(uint64_t Address, uint64_t size,
                                          bool objc_only = false) const;
  Expected<StringRef> getStringFromAddress(uint64_t Address,
                                           bool allowEmpty = false) const;
  Expected<StringRef> getSymbolFromAddress(uint64_t Address,
                                           uint64_t &offset) const;
  bool isAddressEncrypted(uint64_t Address) const;
  // Only available on pointer size.
  Expected<uint64_t> getResolvedValueFromAddress64(uint64_t Address) const;
  Expected<uint32_t> getResolvedValueFromAddress32(uint64_t Address) const;

  // ObjC2 helper classes
  Expected<ObjCRef> getObjC2ClassRO64(ObjCClass Data) const;
  Expected<ObjCRef> getObjC2ClassRO32(ObjCClass Data) const;
  Expected<ObjCClass> getObjC2MetaClass64(ObjCClass Data) const;
  Expected<ObjCClass> getObjC2MetaClass32(ObjCClass Data) const;
  // ObjC1 helper classes

private:
  const object::MachOObjectFile *OwningBinary;
};

}

#endif
