//===- ObjCMetadata.h - Objective C Metadata Interface ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares ObjCMetadata Interfaces.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_OBJCMETADATA_OBJCMETADATA_H
#define LLVM_OBJCMETADATA_OBJCMETADATA_H

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/StringSet.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/IR/Constant.h>
#include <llvm/Object/SymbolicFile.h>
#include <llvm/Support/Error.h>
#include <utility>

namespace llvm {

class ObjCRef {
public:
  uint64_t getValue() const { return v.a; }
  void setValue(uint64_t value) { v.a = value; }

  Constant *getLLVMConstant() const { return v.c; }
  void setLLVMConstant(Constant *C) { v.c = C; }

  int64_t getOffset() const { return offset; }
  void setOffset(int64_t bytes) { offset = bytes; }

  ObjCRef() {
    memset(&v, 0, sizeof(v));
  }
  ObjCRef(uint64_t value) : offset (0) {
    v.a = value;
  }
  ObjCRef(Constant *C) : offset (0) {
    v.c = C;
  }
  ObjCRef(Constant *C, int64_t offset) : offset(offset) {
    v.c = C;
  }

  bool operator==(const ObjCRef &Other) const {
    return std::memcmp(&v, &(Other.v), sizeof(v)) == 0;
  }

  bool operator!=(const ObjCRef &Other) const {
    return !(*this == Other);
  }

private:
  union {
    uint64_t a;
    Constant *c;
  } v;
  int64_t offset; // only used with v is Constant. FIXME: this is a hack.
};

class ObjCMetaDataReader;
class ObjCClassRef;
class ObjCProtocolRef;
class ObjCCategoryRef;
class ObjCMethod;
class ObjCProperty;
class ObjCSelectorRef;

typedef SmallVector<ObjCRef, 2> ObjCRefList;
typedef SmallVector<ObjCMethod, 2> ObjCMethodList;
typedef SmallVector<ObjCProperty, 2> ObjCPropertyList;
typedef SmallVector<ObjCClassRef, 2> ObjCClassList;
typedef SmallVector<ObjCCategoryRef, 2> ObjCCategoryList;
typedef SmallVector<ObjCProtocolRef, 2> ObjCProtocolList;
typedef SmallVector<ObjCSelectorRef, 2> ObjCSelectorList;

class ObjCInfoBase {
public:
  ObjCInfoBase(const ObjCMetaDataReader *Reader)
      : Data(), MetadataReader(Reader) {}
  ObjCInfoBase(const ObjCMetaDataReader *Reader, ObjCRef Data)
      : Data(Data), MetadataReader(Reader) {}

  bool operator==(const ObjCInfoBase &Other) const {
    return Data == Other.Data;
  }

  // Returns the VMAddr of the struct.
  ObjCRef &getAddress() { return Data; }

protected:
  ObjCRef Data;
  const ObjCMetaDataReader *MetadataReader;
};

class ObjCProperty : public ObjCInfoBase {
public:
  ObjCProperty(const ObjCMetaDataReader *Reader) : ObjCInfoBase(Reader) {}
  ObjCProperty(const ObjCMetaDataReader *Reader, ObjCRef Data)
      : ObjCInfoBase(Reader, Data) {}

  Expected<StringRef> getName() const;
  Expected<StringRef> getAttribute() const;
  Expected<bool> isDynamic() const;

  // Query for setter and getter.
  // Return empty string if doesn't exists.
  Expected<std::string> getGetter() const;
  Expected<std::string> getSetter() const;
};

class ObjCMethod : public ObjCInfoBase {
public:
  ObjCMethod(const ObjCMetaDataReader *Reader)
      : ObjCInfoBase(Reader), UseRelativeAddress(false) {}
  ObjCMethod(const ObjCMetaDataReader *Reader, ObjCRef Data,
             bool RelativeAddress = false)
      : ObjCInfoBase(Reader, Data), UseRelativeAddress(RelativeAddress) {}

  Expected<StringRef> getName() const;
  Expected<StringRef> getType() const;

  bool isRelativeAddress() const { return UseRelativeAddress; }

private:
  bool UseRelativeAddress;
};

// Interface to read the ObjCClass.
class ObjCClass : public ObjCInfoBase {
public:
  ObjCClass(const ObjCMetaDataReader *Reader) : ObjCInfoBase(Reader) {}
  ObjCClass(const ObjCMetaDataReader *Reader, ObjCRef Data)
      : ObjCInfoBase(Reader, Data) {}

  Expected<StringRef> getName() const;
  Expected<StringRef> getSuperClassName() const;

  Expected<bool> isSwift() const;
  Expected<bool> isMetaClass() const;

  Expected<ObjCPropertyList> properties() const;
  Expected<ObjCPropertyList> classProperties() const;
  Expected<ObjCMethodList> instanceMethods() const;
  Expected<ObjCMethodList> classMethods() const;
};

class ObjCCategory : public ObjCInfoBase {
public:
  ObjCCategory(const ObjCMetaDataReader *Reader) : ObjCInfoBase(Reader) {}
  ObjCCategory(const ObjCMetaDataReader *Reader, ObjCRef Data)
      : ObjCInfoBase(Reader, Data) {}

  Expected<StringRef> getName() const;
  Expected<StringRef> getBaseClassName() const;

  Expected<ObjCPropertyList> properties() const;
  Expected<ObjCMethodList> instanceMethods() const;
  Expected<ObjCMethodList> classMethods() const;
};

class ObjCProtocol : public ObjCInfoBase {
public:
  ObjCProtocol(const ObjCMetaDataReader *Reader) : ObjCInfoBase(Reader) {}
  ObjCProtocol(const ObjCMetaDataReader *Reader, ObjCRef Data)
      : ObjCInfoBase(Reader, Data) {}

  Expected<StringRef> getName() const;

  Expected<ObjCPropertyList> properties() const;
  Expected<ObjCMethodList> instanceMethods() const;
  Expected<ObjCMethodList> classMethods() const;
  Expected<ObjCMethodList> optionalInstanceMethods() const;
  Expected<ObjCMethodList> optionalClassMethods() const;
};

class ObjCClassRef : public ObjCInfoBase {
public:
  ObjCClassRef(const ObjCMetaDataReader *Reader) : ObjCInfoBase(Reader) {}
  ObjCClassRef(const ObjCMetaDataReader *Reader, ObjCRef Data)
      : ObjCInfoBase(Reader, Data) {}

  Expected<StringRef> getClassName() const;
  bool isExternal() const;
  Expected<ObjCClass> getObjCClass() const;
  Expected<ObjCClass> operator*() const { return getObjCClass(); }
};

class ObjCCategoryRef : public ObjCInfoBase {
public:
  ObjCCategoryRef(const ObjCMetaDataReader *Reader) : ObjCInfoBase(Reader) {}
  ObjCCategoryRef(const ObjCMetaDataReader *Reader, ObjCRef Data)
      : ObjCInfoBase(Reader, Data) {}

  Expected<ObjCCategory> getObjCCategory() const;
  Expected<ObjCCategory> operator*() const { return getObjCCategory(); }
};

class ObjCProtocolRef : public ObjCInfoBase {
public:
  ObjCProtocolRef(const ObjCMetaDataReader *Reader) : ObjCInfoBase(Reader) {}
  ObjCProtocolRef(const ObjCMetaDataReader *Reader, ObjCRef Data)
      : ObjCInfoBase(Reader, Data) {}

  Expected<ObjCProtocol> getObjCProtocol() const;
  Expected<ObjCProtocol> operator*() const { return getObjCProtocol(); }
};

class ObjCSelectorRef : public ObjCInfoBase {
public:
  ObjCSelectorRef(const ObjCMetaDataReader *Reader) : ObjCInfoBase(Reader) {}
  ObjCSelectorRef(const ObjCMetaDataReader *Reader, ObjCRef Data)
      : ObjCInfoBase(Reader, Data) {}

  Expected<StringRef> getSelector() const;
};

// ObjC Metadata Reader.
class ObjCMetaDataReader {
public:
  virtual ~ObjCMetaDataReader() {}

  // General
  virtual bool isObjC1() const = 0;
  virtual bool isObjC2() const = 0;

  virtual Expected<StringRef> getSwiftVersion() const = 0;

  // ObjCClassRef.
  virtual Expected<ObjCClassList> classes() const = 0;
  virtual Expected<ObjCClass> getObjCClassFromRef(ObjCClassRef Ref) const = 0;
  virtual bool isObjCClassExternal(ObjCClassRef Ref) const = 0;

  // ObjCCategoryRef.
  virtual Expected<ObjCCategoryList> categories() const = 0;
  virtual Expected<ObjCCategory>
  getObjCCategoryFromRef(ObjCCategoryRef Ref) const = 0;

  // ObjCProtocolRef.
  virtual Expected<ObjCProtocolList> protocols() const = 0;
  virtual Expected<ObjCProtocol>
  getObjCProtocolFromRef(ObjCProtocolRef Ref) const = 0;

  // ObjCClass.
  virtual Expected<StringRef> getObjCClassName(ObjCClass Data) const = 0;
  virtual Expected<StringRef>
  getObjCSuperClassName(ObjCClass Data) const = 0;
  virtual Expected<bool> isObjCClassSwift(ObjCClass Data) const = 0;
  virtual Expected<bool> isObjCClassMetaClass(ObjCClass Data) const = 0;
  virtual Expected<ObjCPropertyList>
  materializePropertyList(ObjCClass Data) const = 0;
  virtual Expected<ObjCPropertyList>
  materializeClassPropertyList(ObjCClass Data) const = 0;
  virtual Expected<ObjCMethodList>
  materializeInstanceMethodList(ObjCClass Data) const = 0;
  virtual Expected<ObjCMethodList>
  materializeClassMethodList(ObjCClass Data) const = 0;

  // ObjCCategory.
  virtual Expected<StringRef> getObjCCategoryName(ObjCCategory Data) const = 0;
  virtual Expected<StringRef>
  getObjCCategoryBaseClassName(ObjCCategory Data) const = 0;
  virtual Expected<ObjCPropertyList>
  materializePropertyList(ObjCCategory Data) const = 0;
  virtual Expected<ObjCMethodList>
  materializeInstanceMethodList(ObjCCategory Data) const = 0;
  virtual Expected<ObjCMethodList>
  materializeClassMethodList(ObjCCategory Data) const = 0;

  // ObjCProtocol.
  virtual Expected<StringRef> getObjCProtocolName(ObjCProtocol Data) const = 0;
  virtual Expected<ObjCPropertyList>
  materializePropertyList(ObjCProtocol Data) const = 0;
  virtual Expected<ObjCMethodList>
  materializeInstanceMethodList(ObjCProtocol Data) const = 0;
  virtual Expected<ObjCMethodList>
  materializeClassMethodList(ObjCProtocol Data) const = 0;
  virtual Expected<ObjCMethodList>
  materializeOptionalInstanceMethodList(ObjCProtocol Data) const = 0;
  virtual Expected<ObjCMethodList>
  materializeOptionalClassMethodList(ObjCProtocol Data) const = 0;

  // ObjCProperty.
  virtual Expected<StringRef> getPropertyName(ObjCProperty Data) const = 0;
  virtual Expected<StringRef>
  getPropertyAttribute(ObjCProperty Data) const = 0;

  // ObjCMethod.
  virtual Expected<StringRef> getMethodName(ObjCMethod Data) const = 0;
  virtual Expected<StringRef> getMethodType(ObjCMethod Data) const = 0;

  // ObjCSelectorRef.
  virtual Expected<ObjCSelectorList> referencedSelectors() const = 0;
  virtual Expected<StringRef>
  getObjCSelectorName(ObjCSelectorRef Ref) const = 0;

  // Potentially defined selectors.
  virtual void getAllPotentiallyDefinedSelectors(StringSet<> &Set) const = 0;

  // Other.
  virtual StringRef getSymbolNameFromRef(ObjCRef Ref) const = 0;
  virtual StringRef guessClassNameBasedOnSymbol(StringRef Sym) const = 0;

  // Helper.
  Expected<StringRef> convertSwiftVersion(unsigned raw) const;
};

}

#endif
