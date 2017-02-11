//===- ObjCBitcode.h - ObjC Metadata Bitcode Interface ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares ObjCBitcode interface.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_OBJCMETADATA_OBJCBITCODE_H
#define LLVM_OBJCMETADATA_OBJCBITCODE_H

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/Triple.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/Module.h>
#include <llvm/ObjCMetadata/ObjCMetadata.h>

namespace llvm {

class BitcodeMetadata : public ObjCMetaDataReader {
public:
  BitcodeMetadata(Module *M);

  bool isObjC1() const override;
  bool isObjC2() const override;

  const Module *getOwningModule() const {
    return OwningModule;
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

  // Helper function.
  bool is64Bit() const {
    return Triple(OwningModule->getTargetTriple()).isArch64Bit();
  }
  const DataLayout &getDataLayout() const {
    return OwningModule->getDataLayout();
  }
  Expected<ConstantStruct*> getObjC2ClassRO(ObjCClass Data) const;

private:
  Module *OwningModule;
};

}

#endif
