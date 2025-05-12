//===- ObjCMetadata.cpp - ObjC Metadata Reader ------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the classes needed to parse ObjCMetadata.
//
//===----------------------------------------------------------------------===//
#include "tapi/ObjCMetadata/ObjCMetadata.h"

using namespace llvm;

Expected<ObjCClass> ObjCClassRef::getObjCClass() const {
  return MetadataReader->getObjCClassFromRef(*this);
}

Expected<ObjCCategory> ObjCCategoryRef::getObjCCategory() const {
  return MetadataReader->getObjCCategoryFromRef(*this);
}

Expected<ObjCProtocol> ObjCProtocolRef::getObjCProtocol() const {
  return MetadataReader->getObjCProtocolFromRef(*this);
}

Expected<StringRef> ObjCClass::getName() const {
  return MetadataReader->getObjCClassName(*this);
}

Expected<StringRef> ObjCClassRef::getClassName() const {
  if (MetadataReader->isObjCClassExternal(*this)) {
    StringRef symName = MetadataReader->getSymbolNameFromRef(Data);
    return MetadataReader->guessClassNameBasedOnSymbol(symName);
  } else if (auto class_t = getObjCClass())
    return class_t->getName();
  else
    return class_t.takeError();
}

bool ObjCClassRef::isExternal() const {
  return MetadataReader->isObjCClassExternal(*this);
}

Expected<StringRef> ObjCClass::getSuperClassName() const {
  return MetadataReader->getObjCSuperClassName(*this);
}

Expected<bool> ObjCClass::isSwift() const {
  return MetadataReader->isObjCClassSwift(*this);
}

Expected<bool> ObjCClass::isMetaClass() const {
  return MetadataReader->isObjCClassMetaClass(*this);
}

Expected<ObjCPropertyList> ObjCClass::properties() const {
  return MetadataReader->materializePropertyList(*this);
}

Expected<ObjCPropertyList> ObjCClass::classProperties() const {
  return MetadataReader->materializeClassPropertyList(*this);
}

Expected<ObjCMethodList> ObjCClass::instanceMethods() const {
  return MetadataReader->materializeInstanceMethodList(*this);
}

Expected<ObjCMethodList> ObjCClass::classMethods() const {
  return MetadataReader->materializeClassMethodList(*this);
}

Expected<StringRef> ObjCCategory::getName() const {
  return MetadataReader->getObjCCategoryName(*this);
}

Expected<StringRef> ObjCCategory::getBaseClassName() const {
  return MetadataReader->getObjCCategoryBaseClassName(*this);
}

Expected<ObjCPropertyList> ObjCCategory::properties() const {
  return MetadataReader->materializePropertyList(*this);
}

Expected<ObjCMethodList> ObjCCategory::instanceMethods() const {
  return MetadataReader->materializeInstanceMethodList(*this);
}

Expected<ObjCMethodList> ObjCCategory::classMethods() const {
  return MetadataReader->materializeClassMethodList(*this);
}

Expected<StringRef> ObjCProtocol::getName() const {
  return MetadataReader->getObjCProtocolName(*this);
}

Expected<ObjCPropertyList> ObjCProtocol::properties() const {
  return MetadataReader->materializePropertyList(*this);
}

Expected<ObjCMethodList> ObjCProtocol::instanceMethods() const {
  return MetadataReader->materializeInstanceMethodList(*this);
}

Expected<ObjCMethodList> ObjCProtocol::classMethods() const {
  return MetadataReader->materializeClassMethodList(*this);
}

Expected<ObjCMethodList> ObjCProtocol::optionalInstanceMethods() const {
  return MetadataReader->materializeOptionalInstanceMethodList(*this);
}

Expected<ObjCMethodList> ObjCProtocol::optionalClassMethods() const {
  return MetadataReader->materializeOptionalClassMethodList(*this);
}

Expected<StringRef> ObjCProperty::getName() const {
  return MetadataReader->getPropertyName(*this);
}

Expected<StringRef> ObjCProperty::getAttribute() const {
  return MetadataReader->getPropertyAttribute(*this);
}

Expected<bool> ObjCProperty::isDynamic() const {
  auto Attr = getAttribute();
  if (!Attr)
    return Attr.takeError();
  // Find @dynamic attribute.
  SmallVector<StringRef, 4> Attrs;
  Attr->split(Attrs, ',');
  for (auto a : Attrs) {
    if (a == "D")
      return true;
  }
  return false;
}


Expected<std::string> ObjCProperty::getGetter() const {
  auto Name = getName();
  if (!Name)
    return Name.takeError();
  if (*Name == "#EncryptedString#")
    return Name->str();
  auto Attr = getAttribute();
  if (!Attr)
    return Attr.takeError();
  // Find getter attribute.
  SmallVector<StringRef, 4> Attrs;
  Attr->split(Attrs, ',');
  for (auto a : Attrs) {
    if (a.startswith("G"))
      return a.drop_front(1).str();
  }
  // Otherwise returns property name.
  return Name->str();
}

Expected<std::string> ObjCProperty::getSetter() const {
  auto Name = getName();
  if (!Name)
    return Name.takeError();
  if (*Name == "#EncryptedString#")
    return Name->str();
  auto Attr = getAttribute();
  if (!Attr)
    return Attr.takeError();
  // Find setter attribute.
  SmallVector<StringRef, 4> Attrs;
  Attr->split(Attrs, ',');
  for (auto a : Attrs) {
    if (a.startswith("S"))
      return a.drop_front(1).str();
    else if (a.startswith("R"))
      return std::string(); // Read-only property
  }
  // Otherwise returns property name.
  std::string setter =
      "set" + Name->substr(0, 1).upper() + Name->substr(1).str() + ":";
  return setter;
}

Expected<StringRef> ObjCMethod::getName() const {
  return MetadataReader->getMethodName(*this);
}

Expected<StringRef> ObjCMethod::getType() const {
  return MetadataReader->getMethodType(*this);
}

Expected<StringRef> ObjCSelectorRef::getSelector() const {
  return MetadataReader->getObjCSelectorName(*this);
}

Expected<StringRef>
ObjCMetaDataReader::convertSwiftVersion(unsigned raw) const {
  switch (raw) {
  case 0:
    return "";
  case 1:
    return "1.0";
  case 2:
    return "1.1";
  case 3:
    return "2.0";
  case 4:
    return "3.0";
  case 5:
    return "4.0";
  case 6:
    return "4.1/4.2";
  case 7:
    return "5.0 or later";
  default:
    return "Unknown future swift version";
  }
}
