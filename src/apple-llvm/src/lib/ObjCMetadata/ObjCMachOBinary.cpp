//===- ObjCMachOBinary.cpp - Mach-O ObjC Metadata Reader --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the classes needed to parse ObjCMetadata in MachO Object.
//
//===----------------------------------------------------------------------===//

#include "llvm/ObjCMetadata/ObjCMetadata.h"
#include "llvm/ObjCMetadata/ObjCMachOBinary.h"
#include "llvm/Object/MachO.h"

#include "macho-obj.h"

using namespace llvm;
using namespace object;

Expected<ObjCClassList> MachOMetadata::classes() const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCClassList ClassList;
  SectionRef ClassSect = getSection("__OBJC2", "__class_list");
  if (ClassSect == SectionRef())
    ClassSect = getSection("__DATA", "__objc_classlist");
  // Return empty if the section doesn't exists.
  if (ClassSect == SectionRef())
    return ClassList;
  if (ClassSect.getSize() % OwningBinary->getBytesInAddress() != 0)
    return make_error<StringError>(
        "objc class section size is not a multiple of pointer size",
        object_error::parse_failed);

  for (unsigned i = 0; i < ClassSect.getSize();
       i += OwningBinary->getBytesInAddress())
    ClassList.push_back(ObjCClassRef(this, ClassSect.getAddress() + i));

  return ClassList;
}

Expected<ObjCClass> MachOMetadata::getObjCClassFromRef(ObjCClassRef Ref) const {
  if (OwningBinary->is64Bit()) {
    if (auto class_ptr =
            getResolvedValueFromAddress64(Ref.getRawContent().getValue()))
      return ObjCClass(this, *class_ptr);
    else
      return class_ptr.takeError();
  } else {
    if (auto class_ptr =
            getResolvedValueFromAddress32(Ref.getRawContent().getValue()))
      return ObjCClass(this, *class_ptr);
    else
      return class_ptr.takeError();
  }
}

bool MachOMetadata::isObjCClassExternal(ObjCClassRef Ref) const {
  if (OwningBinary->is64Bit()) {
    if (auto addr =
            getResolvedValueFromAddress64(Ref.getRawContent().getValue())) {
      if (*addr == 0)
        return true;
      else
        return false;
    } else {
      // assume external if can't get class.
      // ignore error and return true.
      handleAllErrors(addr.takeError(),
                      [&](const ErrorInfoBase &EI) { return; });
      return true;
    }
  } else {
    if (auto addr =
            getResolvedValueFromAddress32(Ref.getRawContent().getValue())) {
      if (*addr == 0)
        return true;
      else
        return false;
    } else {
      // assume external if can't get class.
      // ignore error and return true.
      handleAllErrors(addr.takeError(),
                      [&](const ErrorInfoBase &EI) { return; });
      return true;
    }
  }
}

StringRef MachOMetadata::getSymbolNameFromRef(ObjCRef Ref) const {
  uint64_t offset;
  if (auto symName =
          getSymbolFromAddress(Ref.getValue(), offset))
    return *symName;
  else {
    // ignore error and return empty string.
    handleAllErrors(symName.takeError(),
                    [&](const ErrorInfoBase &EI) { return; });
    return StringRef();
  }
}

StringRef MachOMetadata::guessClassNameBasedOnSymbol(StringRef Sym) const {
  if (Sym.startswith("_OBJC_CLASS_$_"))
    return Sym.drop_front(strlen("_OBJC_CLASS_$_"));
  else if (Sym.startswith(".objc_class_name_"))
    return Sym.drop_front(strlen(".objc_class_name_"));
  else
    return Sym;
}

Expected<ObjCCategoryList> MachOMetadata::categories() const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCCategoryList CategoryList;
  SectionRef CatSect = getSection("__OBJC2", "__category_list");
  if (CatSect == SectionRef())
    CatSect = getSection("__DATA", "__objc_catlist");
  // Return empty if the section doesn't exists.
  if (CatSect == SectionRef())
    return CategoryList;
  if (CatSect.getSize() % OwningBinary->getBytesInAddress() != 0)
    return make_error<StringError>(
        "objc category section size is not a multiple of pointer size",
        object_error::parse_failed);

  for (unsigned i = 0; i < CatSect.getSize();
       i += OwningBinary->getBytesInAddress())
    CategoryList.push_back(ObjCCategoryRef(this, CatSect.getAddress() + i));

  return CategoryList;
}

Expected<ObjCCategory>
MachOMetadata::getObjCCategoryFromRef(ObjCCategoryRef Ref) const {
  if (OwningBinary->is64Bit()) {
    if (auto cat_ptr =
            getResolvedValueFromAddress64(Ref.getRawContent().getValue()))
      return ObjCCategory(this, *cat_ptr);
    else
      return cat_ptr.takeError();
  } else {
    if (auto cat_ptr =
            getResolvedValueFromAddress32(Ref.getRawContent().getValue()))
      return ObjCCategory(this, *cat_ptr);
    else
      return cat_ptr.takeError();
  }
}

Expected<ObjCProtocolList> MachOMetadata::protocols() const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCProtocolList ProtocolList;
  SectionRef ProtoSect = getSection("__OBJC2", "__protocol_list");
  if (ProtoSect == SectionRef())
    ProtoSect = getSection("__DATA", "__objc_protolist");
  // Return empty if the section doesn't exists.
  if (ProtoSect == SectionRef())
    return ProtocolList;
  if (ProtoSect.getSize() % OwningBinary->getBytesInAddress() != 0)
    return make_error<StringError>(
        "objc protocol section size is not a multiple of pointer size",
        object_error::parse_failed);

  for (unsigned i = 0; i < ProtoSect.getSize();
       i += OwningBinary->getBytesInAddress())
    ProtocolList.push_back(ObjCProtocolRef(this, ProtoSect.getAddress() + i));

  return ProtocolList;
}

Expected<ObjCProtocol>
MachOMetadata::getObjCProtocolFromRef(ObjCProtocolRef Ref) const {
  if (OwningBinary->is64Bit()) {
    if (auto proto_ptr =
            getResolvedValueFromAddress64(Ref.getRawContent().getValue()))
      return ObjCProtocol(this, *proto_ptr);
    else
      return proto_ptr.takeError();
  } else {
    if (auto proto_ptr =
            getResolvedValueFromAddress32(Ref.getRawContent().getValue()))
      return ObjCProtocol(this, *proto_ptr);
    else
      return proto_ptr.takeError();
  }
}

Expected<StringRef> MachOMetadata::getObjCClassName(ObjCClass ClassRef) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  if (OwningBinary->is64Bit()) {
    if (auto ro = getObjC2ClassRO64(ClassRef)) {
      if (auto name = getResolvedValueFromAddress64(
              ro->getValue() + offsetof(class_ro64_t, name)))
        return getStringFromAddress(*name);
      else
        return name.takeError();
    } else
      return ro.takeError();
  } else {
    if (auto ro = getObjC2ClassRO32(ClassRef)) {
      if (auto name = getResolvedValueFromAddress32(
              ro->getValue() + offsetof(class_ro32_t, name)))
        return getStringFromAddress(*name);
      else
        return name.takeError();
    } else
      return ro.takeError();
  }
}

Expected<StringRef> MachOMetadata::getObjCSuperClassName(ObjCClass Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  if (OwningBinary->is64Bit()) {
    uint64_t superAddr =
        Data.getRawContent().getValue() + offsetof(class64_t, superclass);
    ObjCClassRef super(this, superAddr);
    if (auto superName = super.getClassName())
      return *superName;
    else
      return superName.takeError();
  } else {
    uint64_t superAddr =
        Data.getRawContent().getValue() + offsetof(class32_t, superclass);
    ObjCClassRef super(this, superAddr);
    if (auto superName = super.getClassName())
      return *superName;
    else
      return superName.takeError();
  }
}

Expected<bool> MachOMetadata::isObjCClassSwift(ObjCClass Data) const {
  if (isObjC1())
    return false;
  if (OwningBinary->is64Bit()) {
    if (auto class_ro = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() + offsetof(class64_t, data))) {
      return ((*class_ro & ~0x7) != 0);
    } else
      return class_ro.takeError();
  } else {
    if (auto class_ro = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() + offsetof(class32_t, data))) {
      return ((*class_ro & ~0x3) != 0);
    } else
      return class_ro.takeError();
  }
}

Expected<bool> MachOMetadata::isObjCClassMetaClass(ObjCClass Data) const {
  if (isObjC1())
    return false;
  if (OwningBinary->is64Bit()) {
    if (auto class_ro = getObjC2ClassRO64(Data)) {
      uint32_t f;
      if (auto flags = getValueFromAddress(class_ro->getValue() +
                                               offsetof(class_ro64_t, flags),
                                           sizeof(f))) {
        memcpy(&f, flags->data(), sizeof(f));
        if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
          sys::swapByteOrder(f);
        return ((f & RO_META) != 0);
      } else
        return flags.takeError();
    } else
      return class_ro.takeError();
  } else {
    if (auto class_ro = getObjC2ClassRO32(Data)) {
      uint32_t f;
      if (auto flags = getValueFromAddress(class_ro->getValue() +
                                               offsetof(class_ro32_t, flags),
                                           sizeof(f))) {
        memcpy(&f, flags->data(), sizeof(f));
        if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
          sys::swapByteOrder(f);
        return ((f & RO_META) != 0);
      } else
        return flags.takeError();
    } else
      return class_ro.takeError();
  }
}

Expected<ObjCPropertyList>
MachOMetadata::materializePropertyList(ObjCClass ClassRef) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCPropertyList PropertyList;
  if (OwningBinary->is64Bit()) {
    if (auto ro = getObjC2ClassRO64(ClassRef)) {
      if (auto baseProperties = getResolvedValueFromAddress64(
              ro->getValue() + offsetof(class_ro64_t, baseProperties))) {
        if (!*baseProperties)
          return PropertyList;
        objc_property_list64 propList;
        if (auto pList =
                getValueFromAddress(*baseProperties, sizeof(propList)))
          memcpy(&propList, pList->data(), sizeof(propList));
        else
          return pList.takeError();
        if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
          swapStruct(propList);
        for (unsigned i = 0; i < propList.count; ++ i)
          PropertyList.push_back(ObjCProperty(
              this, *baseProperties + sizeof(objc_property_list64) +
                        i * sizeof(objc_property64)));
        return PropertyList;
      } else
        return baseProperties.takeError();
    } else
      return ro.takeError();
  } else {
    if (auto ro = getObjC2ClassRO32(ClassRef)) {
      if (auto baseProperties = getResolvedValueFromAddress32(
              ro->getValue() + offsetof(class_ro32_t, baseProperties))) {
        if (!*baseProperties)
          return PropertyList;
        objc_property_list32 propList;
        if (auto pList =
                getValueFromAddress(*baseProperties, sizeof(propList)))
          memcpy(&propList, pList->data(), sizeof(propList));
        else
          return pList.takeError();
        if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
          swapStruct(propList);
        for (unsigned i = 0; i < propList.count; ++ i)
          PropertyList.push_back(ObjCProperty(
              this, *baseProperties + sizeof(objc_property_list32) +
                        i * sizeof(objc_property32)));
        return PropertyList;
      } else
        return baseProperties.takeError();
    } else
      return ro.takeError();
  }
}

Expected<StringRef> MachOMetadata::getPropertyName(ObjCProperty Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  if (OwningBinary->is64Bit()) {
    if (auto propName = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() + offsetof(objc_property64, name)))
      return getStringFromAddress(*propName);
    else
      return propName.takeError();
  } else {
    if (auto propName = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() + offsetof(objc_property32, name)))
      return getStringFromAddress(*propName);
    else
      return propName.takeError();
  }
}

Expected<StringRef>
MachOMetadata::getPropertyAttribute(ObjCProperty Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  if (OwningBinary->is64Bit()) {
    if (auto propAttr = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() +
            offsetof(objc_property64, attributes)))
      return getStringFromAddress(*propAttr);
    else
      return propAttr.takeError();
  } else {
    if (auto propAttr = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() +
            offsetof(objc_property32, attributes)))
      return getStringFromAddress(*propAttr);
    else
      return propAttr.takeError();
  }
}

Expected<ObjCMethodList>
MachOMetadata::materializeInstanceMethodList(ObjCClass ClassRef) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCMethodList MethodList;
  if (OwningBinary->is64Bit()) {
    if (auto ro = getObjC2ClassRO64(ClassRef)) {
      if (auto baseMethods = getResolvedValueFromAddress64(
              ro->getValue() + offsetof(class_ro64_t, baseMethods))) {
        if (!*baseMethods)
          return MethodList;
        method_list64_t methodList;
        if (auto mList =
                getValueFromAddress(*baseMethods, sizeof(methodList)))
          memcpy(&methodList, mList->data(), sizeof(methodList));
        else
          return mList.takeError();
        if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
          swapStruct(methodList);
        for (unsigned i = 0; i < methodList.count; ++i)
          MethodList.push_back(ObjCMethod(this, *baseMethods +
                                                    sizeof(method_list64_t) +
                                                    i * sizeof(method64_t)));
        return MethodList;
      } else
        return baseMethods.takeError();
    } else
      return ro.takeError();
  } else {
    if (auto ro = getObjC2ClassRO32(ClassRef)) {
      if (auto baseMethods = getResolvedValueFromAddress32(
              ro->getValue() + offsetof(class_ro32_t, baseMethods))) {
        if (!*baseMethods)
          return MethodList;
        method_list32_t methodList;
        if (auto mList =
                getValueFromAddress(*baseMethods, sizeof(methodList)))
          memcpy(&methodList, mList->data(), sizeof(methodList));
        else
          return mList.takeError();
        if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
          swapStruct(methodList);
        for (unsigned i = 0; i < methodList.count; ++i)
          MethodList.push_back(ObjCMethod(this, *baseMethods +
                                                    sizeof(method_list32_t) +
                                                    i * sizeof(method32_t)));
        return MethodList;
      } else
        return baseMethods.takeError();
    } else
      return ro.takeError();
  }
}

Expected<ObjCMethodList>
MachOMetadata::materializeClassMethodList(ObjCClass ClassRef) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  if (OwningBinary->is64Bit()) {
    if (auto meta = getObjC2MetaClass64(ClassRef)) {
      if (meta->getRawContent().getValue() == 0)
        return ObjCMethodList();
      else
        return materializeInstanceMethodList(*meta);
    } else
      return meta.takeError();
  } else {
    if (auto meta = getObjC2MetaClass32(ClassRef)) {
      if (meta->getRawContent().getValue() == 0)
        return ObjCMethodList();
      else
        return materializeInstanceMethodList(*meta);
    } else
      return meta.takeError();
  }
}

Expected<StringRef> MachOMetadata::getObjCCategoryName(ObjCCategory Cat) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  if (OwningBinary->is64Bit()) {
    if (auto name = getResolvedValueFromAddress64(
            Cat.getRawContent().getValue() + offsetof(category64_t, name)))
      return getStringFromAddress(*name);
    else
      return name.takeError();
  } else {
    if (auto name = getResolvedValueFromAddress32(
            Cat.getRawContent().getValue() + offsetof(category32_t, name)))
      return getStringFromAddress(*name);
    else
      return name.takeError();
  }
}

Expected<StringRef>
MachOMetadata::getObjCCategoryBaseClassName(ObjCCategory Cat) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  if (OwningBinary->is64Bit()) {
    uint64_t baseClassAddr =
        Cat.getRawContent().getValue() + offsetof(category64_t, cls);
    ObjCClassRef cls(this, baseClassAddr);
    if (auto clsName = cls.getClassName())
      return *clsName;
    else
      return clsName.takeError();
  } else {
    uint64_t baseClassAddr =
        Cat.getRawContent().getValue() + offsetof(category32_t, cls);
    ObjCClassRef cls(this, baseClassAddr);
    if (auto clsName = cls.getClassName())
      return *clsName;
    else
      return clsName.takeError();
  }
}

Expected<ObjCPropertyList>
MachOMetadata::materializePropertyList(ObjCCategory Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCPropertyList PropertyList;
  if (OwningBinary->is64Bit()) {
    if (auto iprop = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() +
            offsetof(category64_t, instanceProperties))) {
      if (*iprop == 0)
        return PropertyList;

      objc_property_list64 propList;
      if (auto pList = getValueFromAddress(*iprop, sizeof(propList)))
        memcpy(&propList, pList->data(), sizeof(propList));
      else
        return pList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(propList);
      for (unsigned i = 0; i < propList.count; ++ i)
        PropertyList.push_back(
            ObjCProperty(this, *iprop + sizeof(objc_property_list64) +
                                   i * sizeof(objc_property64)));
      return PropertyList;
    } else
      return iprop.takeError();
  } else {
    if (auto iprop = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() +
            offsetof(category32_t, instanceProperties))) {
      if (*iprop == 0)
        return PropertyList;

      objc_property_list32 propList;
      if (auto pList = getValueFromAddress(*iprop, sizeof(propList)))
        memcpy(&propList, pList->data(), sizeof(propList));
      else
        return pList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(propList);
      for (unsigned i = 0; i < propList.count; ++ i)
        PropertyList.push_back(
            ObjCProperty(this, *iprop + sizeof(objc_property_list32) +
                                   i * sizeof(objc_property32)));
      return PropertyList;
    } else
      return iprop.takeError();
  }
}

Expected<ObjCMethodList>
MachOMetadata::materializeInstanceMethodList(ObjCCategory Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCMethodList MethodList;
  if (OwningBinary->is64Bit()) {
    if (auto iprop = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() +
            offsetof(category64_t, instanceMethods))) {
      if (*iprop == 0)
        return MethodList;

      method_list64_t methodList;
      if (auto mList = getValueFromAddress(*iprop, sizeof(methodList)))
        memcpy(&methodList, mList->data(), sizeof(methodList));
      else
        return mList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(methodList);
      for (unsigned i = 0; i < methodList.count; ++ i)
        MethodList.push_back(ObjCMethod(this, *iprop + sizeof(method_list64_t) +
                                                  i * sizeof(method64_t)));
      return MethodList;
    } else
      return iprop.takeError();
  } else {
    if (auto iprop = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() +
            offsetof(category32_t, instanceMethods))) {
      if (*iprop == 0)
        return MethodList;

      method_list32_t methodList;
      if (auto mList = getValueFromAddress(*iprop, sizeof(methodList)))
        memcpy(&methodList, mList->data(), sizeof(methodList));
      else
        return mList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(methodList);
      for (unsigned i = 0; i < methodList.count; ++ i)
        MethodList.push_back(ObjCMethod(this, *iprop + sizeof(method_list32_t) +
                                                  i * sizeof(method32_t)));
      return MethodList;
    } else
      return iprop.takeError();
  }
}

Expected<ObjCMethodList>
MachOMetadata::materializeClassMethodList(ObjCCategory Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCMethodList MethodList;
  if (OwningBinary->is64Bit()) {
    if (auto iprop = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() +
            offsetof(category64_t, classMethods))) {
      if (*iprop == 0)
        return MethodList;

      method_list64_t methodList;
      if (auto mList = getValueFromAddress(*iprop, sizeof(methodList)))
        memcpy(&methodList, mList->data(), sizeof(methodList));
      else
        return mList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(methodList);
      for (unsigned i = 0; i < methodList.count; ++ i)
        MethodList.push_back(ObjCMethod(this, *iprop + sizeof(method_list64_t) +
                                                  i * sizeof(method64_t)));
      return MethodList;
    } else
      return iprop.takeError();
  } else {
    if (auto iprop = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() +
            offsetof(category32_t, classMethods))) {
      if (*iprop == 0)
        return MethodList;

      method_list32_t methodList;
      if (auto mList = getValueFromAddress(*iprop, sizeof(methodList)))
        memcpy(&methodList, mList->data(), sizeof(methodList));
      else
        return mList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(methodList);
      for (unsigned i = 0; i < methodList.count; ++ i)
        MethodList.push_back(ObjCMethod(this, *iprop + sizeof(method_list32_t) +
                                                  i * sizeof(method32_t)));
      return MethodList;
    } else
      return iprop.takeError();
  }
}

Expected<StringRef>
MachOMetadata::getObjCProtocolName(ObjCProtocol Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  if (OwningBinary->is64Bit()) {
    if (auto name = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() + offsetof(protocol64_t, name)))
      return getStringFromAddress(*name);
    else
      return name.takeError();
  } else {
    if (auto name = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() + offsetof(protocol32_t, name)))
      return getStringFromAddress(*name);
    else
      return name.takeError();
  }
}

Expected<ObjCPropertyList>
MachOMetadata::materializePropertyList(ObjCProtocol Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCPropertyList PropertyList;
  if (OwningBinary->is64Bit()) {
    if (auto iprop = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() +
            offsetof(protocol64_t, instanceProperties))) {
      if (*iprop == 0)
        return PropertyList;

      objc_property_list64 propList;
      if (auto pList = getValueFromAddress(*iprop, sizeof(propList)))
        memcpy(&propList, pList->data(), sizeof(propList));
      else
        return pList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(propList);
      for (unsigned i = 0; i < propList.count; ++ i)
        PropertyList.push_back(
            ObjCProperty(this, *iprop + sizeof(objc_property_list64) +
                                   i * sizeof(objc_property64)));
      return PropertyList;
    } else
      return iprop.takeError();
  } else {
    if (auto iprop = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() +
            offsetof(protocol32_t, instanceProperties))) {
      if (*iprop == 0)
        return PropertyList;

      objc_property_list32 propList;
      if (auto pList = getValueFromAddress(*iprop, sizeof(propList)))
        memcpy(&propList, pList->data(), sizeof(propList));
      else
        return pList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(propList);
      for (unsigned i = 0; i < propList.count; ++ i)
        PropertyList.push_back(
            ObjCProperty(this, *iprop + sizeof(objc_property_list32) +
                                   i * sizeof(objc_property32)));
      return PropertyList;
    } else
      return iprop.takeError();
  }
}

Expected<ObjCMethodList>
MachOMetadata::materializeInstanceMethodList(ObjCProtocol Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCMethodList MethodList;
  if (OwningBinary->is64Bit()) {
    if (auto iprop = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() +
            offsetof(protocol64_t, instanceMethods))) {
      if (*iprop == 0)
        return MethodList;

      method_list64_t methodList;
      if (auto mList = getValueFromAddress(*iprop, sizeof(methodList)))
        memcpy(&methodList, mList->data(), sizeof(methodList));
      else
        return mList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(methodList);
      for (unsigned i = 0; i < methodList.count; ++ i)
        MethodList.push_back(ObjCMethod(this, *iprop + sizeof(method_list64_t) +
                                                  i * sizeof(method64_t)));
      return MethodList;
    } else
      return iprop.takeError();
  } else {
    if (auto iprop = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() +
            offsetof(protocol32_t, instanceMethods))) {
      if (*iprop == 0)
        return MethodList;

      method_list32_t methodList;
      if (auto mList = getValueFromAddress(*iprop, sizeof(methodList)))
        memcpy(&methodList, mList->data(), sizeof(methodList));
      else
        return mList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(methodList);
      for (unsigned i = 0; i < methodList.count; ++ i)
        MethodList.push_back(ObjCMethod(this, *iprop + sizeof(method_list32_t) +
                                                  i * sizeof(method32_t)));
      return MethodList;
    } else
      return iprop.takeError();
  }
}

Expected<ObjCMethodList>
MachOMetadata::materializeClassMethodList(ObjCProtocol Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCMethodList MethodList;
  if (OwningBinary->is64Bit()) {
    if (auto iprop = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() +
            offsetof(protocol64_t, classMethods))) {
      if (*iprop == 0)
        return MethodList;

      method_list64_t methodList;
      if (auto mList = getValueFromAddress(*iprop, sizeof(methodList)))
        memcpy(&methodList, mList->data(), sizeof(methodList));
      else
        return mList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(methodList);
      for (unsigned i = 0; i < methodList.count; ++ i)
        MethodList.push_back(ObjCMethod(this, *iprop + sizeof(method_list64_t) +
                                                  i * sizeof(method64_t)));
      return MethodList;
    } else
      return iprop.takeError();
  } else {
    if (auto iprop = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() +
            offsetof(protocol32_t, classMethods))) {
      if (*iprop == 0)
        return MethodList;

      method_list32_t methodList;
      if (auto mList = getValueFromAddress(*iprop, sizeof(methodList)))
        memcpy(&methodList, mList->data(), sizeof(methodList));
      else
        return mList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(methodList);
      for (unsigned i = 0; i < methodList.count; ++ i)
        MethodList.push_back(ObjCMethod(this, *iprop + sizeof(method_list32_t) +
                                                  i * sizeof(method32_t)));
      return MethodList;
    } else
      return iprop.takeError();
  }
}

Expected<ObjCMethodList>
MachOMetadata::materializeOptionalInstanceMethodList(ObjCProtocol Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCMethodList MethodList;
  if (OwningBinary->is64Bit()) {
    if (auto iprop = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() +
            offsetof(protocol64_t, optionalInstanceMethods))) {
      if (*iprop == 0)
        return MethodList;

      method_list64_t methodList;
      if (auto mList = getValueFromAddress(*iprop, sizeof(methodList)))
        memcpy(&methodList, mList->data(), sizeof(methodList));
      else
        return mList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(methodList);
      for (unsigned i = 0; i < methodList.count; ++ i)
        MethodList.push_back(ObjCMethod(this, *iprop + sizeof(method_list64_t) +
                                                  i * sizeof(method64_t)));
      return MethodList;
    } else
      return iprop.takeError();
  } else {
    if (auto iprop = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() +
            offsetof(protocol32_t, optionalInstanceMethods))) {
      if (*iprop == 0)
        return MethodList;

      method_list32_t methodList;
      if (auto mList = getValueFromAddress(*iprop, sizeof(methodList)))
        memcpy(&methodList, mList->data(), sizeof(methodList));
      else
        return mList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(methodList);
      for (unsigned i = 0; i < methodList.count; ++ i)
        MethodList.push_back(ObjCMethod(this, *iprop + sizeof(method_list32_t) +
                                                  i * sizeof(method32_t)));
      return MethodList;
    } else
      return iprop.takeError();
  }
}

Expected<ObjCMethodList>
MachOMetadata::materializeOptionalClassMethodList(ObjCProtocol Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCMethodList MethodList;
  if (OwningBinary->is64Bit()) {
    if (auto iprop = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() +
            offsetof(protocol64_t, optionalClassMethods))) {
      if (*iprop == 0)
        return MethodList;

      method_list64_t methodList;
      if (auto mList = getValueFromAddress(*iprop, sizeof(methodList)))
        memcpy(&methodList, mList->data(), sizeof(methodList));
      else
        return mList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(methodList);
      for (unsigned i = 0; i < methodList.count; ++ i)
        MethodList.push_back(ObjCMethod(this, *iprop + sizeof(method_list64_t) +
                                                  i * sizeof(method64_t)));
      return MethodList;
    } else
      return iprop.takeError();
  } else {
    if (auto iprop = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() +
            offsetof(protocol32_t, optionalClassMethods))) {
      if (*iprop == 0)
        return MethodList;

      method_list32_t methodList;
      if (auto mList = getValueFromAddress(*iprop, sizeof(methodList)))
        memcpy(&methodList, mList->data(), sizeof(methodList));
      else
        return mList.takeError();
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(methodList);
      for (unsigned i = 0; i < methodList.count; ++ i)
        MethodList.push_back(ObjCMethod(this, *iprop + sizeof(method_list32_t) +
                                                  i * sizeof(method32_t)));
      return MethodList;
    } else
      return iprop.takeError();
  }
}

Expected<StringRef> MachOMetadata::getMethodName(ObjCMethod Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  if (OwningBinary->is64Bit()) {
    if (auto methodName = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() + offsetof(method64_t, name)))
      return getStringFromAddress(*methodName);
    else
      return methodName.takeError();
  } else {
    if (auto methodName = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() + offsetof(method32_t, name)))
      return getStringFromAddress(*methodName);
    else
      return methodName.takeError();
  }
}

Expected<StringRef> MachOMetadata::getMethodType(ObjCMethod Data) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  if (OwningBinary->is64Bit()) {
    if (auto methodTypes = getResolvedValueFromAddress64(
            Data.getRawContent().getValue() + offsetof(method64_t, types)))
      return getStringFromAddress(*methodTypes);
    else
      return methodTypes.takeError();
  } else {
    if (auto methodTypes = getResolvedValueFromAddress32(
            Data.getRawContent().getValue() + offsetof(method32_t, types)))
      return getStringFromAddress(*methodTypes);
    else
      return methodTypes.takeError();
  }
}

Expected<ObjCSelectorList> MachOMetadata::referencedSelectors() const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  ObjCSelectorList SelectorList;
  SectionRef SelSect = getSection("__DATA", "__objc_selrefs");
  // Return empty if the section doesn't exists.
  if (SelSect == SectionRef())
    return SelectorList;

  if (SelSect.getSize() % OwningBinary->getBytesInAddress() != 0)
    return make_error<StringError>(
        "objc class section size is not a multiple of pointer size",
        object_error::parse_failed);

  for (unsigned i = 0; i < SelSect.getSize();
       i += OwningBinary->getBytesInAddress())
    SelectorList.push_back(ObjCSelectorRef(this, SelSect.getAddress() + i));

  return SelectorList;
}

Expected<StringRef>
MachOMetadata::getObjCSelectorName(ObjCSelectorRef Ref) const {
  if (isObjC1())
    return make_error<StringError>("Unsupported ObjC Runtime",
                                   object_error::parse_failed);
  if (OwningBinary->is64Bit()) {
    if (auto sel =
            getResolvedValueFromAddress64(Ref.getRawContent().getValue()))
      return getStringFromAddress(*sel);
    else
      return sel.takeError();
  } else {
    if (auto sel =
            getResolvedValueFromAddress32(Ref.getRawContent().getValue()))
      return getStringFromAddress(*sel);
    else
      return sel.takeError();
  }
}

void MachOMetadata::getAllPotentiallyDefinedSelectors(StringSet<> &Set) const {
  // Clear the set.
  Set.clear();
  SectionRef ObjCConstSect = getSection("__DATA", "__objc_const");
  SectionRef MethodNameSect = getSection("__TEXT", "__objc_methname");
  if (ObjCConstSect == SectionRef() || MethodNameSect == SectionRef())
    return;

  auto ignoreError = [](Error &&E) {
    handleAllErrors(std::move(E), [&](const ErrorInfoBase &EI) {
      return; // ignore.
    });
  };

  if (auto sels = referencedSelectors()) {
    StringSet<> refSels;
    for (auto sel : *sels) {
      if (auto selName = sel.getSelector())
        refSels.insert(*selName);
      else
        ignoreError(selName.takeError());
    }
    if (OwningBinary->is64Bit()) {
      for (uint64_t A = ObjCConstSect.getAddress(),
                    E = ObjCConstSect.getAddress() + ObjCConstSect.getSize();
           A < E; A += OwningBinary->getBytesInAddress()) {
        auto methodName = getResolvedValueFromAddress64(A);
        if (!methodName) {
          ignoreError(methodName.takeError());
          continue;
        }
        if (*methodName >= MethodNameSect.getAddress() &&
            *methodName <
                MethodNameSect.getAddress() + MethodNameSect.getSize()) {
          if (auto name = getStringFromAddress(*methodName)) {
            if (refSels.find(*name) != refSels.end())
              Set.insert(*name);
          } else
            ignoreError(name.takeError());
        }
      }
    } else {
      for (uint64_t A = ObjCConstSect.getAddress(),
                    E = ObjCConstSect.getAddress() + ObjCConstSect.getSize();
           A < E; A += OwningBinary->getBytesInAddress()) {
        auto methodName = getResolvedValueFromAddress32(A);
        if (!methodName) {
          ignoreError(methodName.takeError());
          continue;
        }
        if (*methodName >= MethodNameSect.getAddress() &&
            *methodName <
                MethodNameSect.getAddress() + MethodNameSect.getSize()) {
          if (auto name = getStringFromAddress(*methodName)) {
            if (refSels.find(*name) != refSels.end())
              Set.insert(*name);
          } else
            ignoreError(name.takeError());
        }
      }
    }
  } else
    ignoreError(sels.takeError());
}

bool MachOMetadata::isObjC1() const { return !isObjC2(); }

bool MachOMetadata::isObjC2() const {
  if (OwningBinary->is64Bit() ||
      OwningBinary->getHeader().cputype == MachO::CPU_TYPE_ARM)
    return true;

  return !(getSection("__OBJC", "__module_info") == SectionRef());
}

const SectionRef MachOMetadata::getSection(const char *segname,
                                           const char *sectname) const {
  for (const SectionRef &Section : OwningBinary->sections()) {
    StringRef SectName;
    Section.getName(SectName);
    DataRefImpl Ref = Section.getRawDataRefImpl();
    StringRef SegName = OwningBinary->getSectionFinalSegmentName(Ref);
    if (SegName == segname && SectName == sectname)
      return Section;
  }
  return SectionRef();
}

Expected<StringRef> MachOMetadata::getValueFromAddress(uint64_t Address,
                                                       uint64_t size,
                                                       bool objc_only) const {
  if (isAddressEncrypted(Address))
    return make_error<StringError>("Value at the address is encrypted",
                                   object_error::parse_failed);
  for (const SectionRef &Section : OwningBinary->sections()) {
    uint64_t SectAddress = Section.getAddress();
    uint64_t SectSize = Section.getSize();
    if (SectSize == 0)
      continue;
    if (objc_only) {
      StringRef SectName;
      Section.getName(SectName);
      DataRefImpl Ref = Section.getRawDataRefImpl();
      StringRef SegName = OwningBinary->getSectionFinalSegmentName(Ref);
      if (SegName != "__OBJC" && SectName != "__cstring")
        continue;
    }
    if (Address >= SectAddress && Address < SectAddress + SectSize) {
      uint64_t offset = Address - SectAddress;
      StringRef SectContents;
      Section.getContents(SectContents);
      // size == 0 implies the length of the string.
      StringRef Target = (size == 0)
                             ? StringRef(SectContents.data() + offset)
                             : StringRef(SectContents.data() + offset, size);
      if (Target.size() + offset > SectSize)
        return make_error<StringError>(
            "Data extends pass the end of the section",
            object_error::parse_failed);
      return Target;
    }
  }
  return make_error<StringError>("Pointer out of bound",
                                 object_error::parse_failed);
}

bool MachOMetadata::isAddressEncrypted(uint64_t Address) const {
  // Find encrypted range.
  if (OwningBinary->is64Bit()) {
    uint64_t fileOffset = 0;
    for (const auto &LC : OwningBinary->load_commands()) {
      if (LC.C.cmd == MachO::LC_SEGMENT_64) {
        MachO::segment_command_64 cmd =
            OwningBinary->getSegment64LoadCommand(LC);
        if (Address > cmd.vmaddr && Address < cmd.vmaddr + cmd.vmsize)
          fileOffset = Address - cmd.vmaddr + cmd.fileoff;
      }
    }
    for (const auto &LC : OwningBinary->load_commands()) {
      if (LC.C.cmd == MachO::LC_ENCRYPTION_INFO_64) {
        MachO::encryption_info_command_64 cmd =
            OwningBinary->getEncryptionInfoCommand64(LC);
        if (cmd.cryptid == 0)
          continue;
        if (fileOffset > cmd.cryptoff &&
            fileOffset < cmd.cryptoff + cmd.cryptsize)
          return true;
      }
    }
  } else {
    uint64_t fileOffset = 0;
    for (const auto &LC : OwningBinary->load_commands()) {
      if (LC.C.cmd == MachO::LC_SEGMENT) {
        MachO::segment_command cmd = OwningBinary->getSegmentLoadCommand(LC);
        if (Address > cmd.vmaddr && Address < cmd.vmaddr + cmd.vmsize)
          fileOffset = Address - cmd.vmaddr + cmd.fileoff;
      }
    }
    for (const auto &LC : OwningBinary->load_commands()) {
      if (LC.C.cmd == MachO::LC_ENCRYPTION_INFO) {
        MachO::encryption_info_command cmd =
            OwningBinary->getEncryptionInfoCommand(LC);
        if (cmd.cryptid == 0)
          continue;
        if (fileOffset > cmd.cryptoff &&
            fileOffset < cmd.cryptoff + cmd.cryptsize)
          return true;
      }
    }
  }
  return false;
}

Expected<StringRef> MachOMetadata::getStringFromAddress(uint64_t Address,
                                                        bool allowEmpty) const {
  if (isAddressEncrypted(Address))
    return "#EncryptedString#";

  for (const SectionRef &Section : OwningBinary->sections()) {
    uint64_t SectAddress = Section.getAddress();
    uint64_t SectSize = Section.getSize();
    if (SectSize == 0)
      continue;
    if (Address >= SectAddress && Address < SectAddress + SectSize) {
      uint64_t offset = Address - SectAddress;
      StringRef SectContents;
      Section.getContents(SectContents);
      StringRef Target = StringRef(SectContents.data() + offset);
      if (Target.size() + offset >= SectSize)
        return make_error<StringError>(
            "Data extends pass the end of the section",
            object_error::parse_failed);
      if (Target.empty() && !allowEmpty)
        return make_error<StringError>(
            "Expect to read a none zero length string",
            object_error::parse_failed);

      return Target;
    }
  }
  return make_error<StringError>("Pointer out of bound",
                                 object_error::parse_failed);
}

Expected<StringRef>
MachOMetadata::getSymbolFromAddress(uint64_t Address, uint64_t &offset) const {
  // Reset offset.
  offset = 0;

  // Found the section.
  SectionRef S;
  uint64_t SectOffset = 0;
  for (const SectionRef &Section : OwningBinary->sections()) {
    uint64_t SectAddress = Section.getAddress();
    uint64_t SectSize = Section.getSize();
    if (SectSize == 0)
      continue;
    if (Address >= SectAddress && Address < SectAddress + SectSize) {
      SectOffset = Address - SectAddress;
      S = Section;
      break;
    }
  }
  if (S == SectionRef())
    return make_error<StringError>("Address not in range of the macho file",
                                   object_error::parse_failed);

  // See if there is an external relocation entry at the sect_offset.
  bool reloc_found = false;
  DataRefImpl Rel;
  MachO::any_relocation_info RE;
  bool isExtern = false;
  SymbolRef Symbol;
  for (const RelocationRef &Reloc : S.relocations()) {
    uint64_t RelocOffset = Reloc.getOffset();
    if (RelocOffset == SectOffset) {
      Rel = Reloc.getRawDataRefImpl();
      RE = OwningBinary->getRelocation(Rel);
      if (OwningBinary->isRelocationScattered(RE))
        continue;
      isExtern = OwningBinary->getPlainRelocationExternal(RE);
      if (isExtern) {
        symbol_iterator RelocSym = Reloc.getSymbol();
        Symbol = *RelocSym;
      }
      reloc_found = true;
      break;
    }
  }
  // If there is an external relocation entry for a symbol in this section
  // at this section_offset then use that symbol's value for the n_value
  // and return its name.
  const char *SymbolName = nullptr;
  if (reloc_found && isExtern) {
    offset = Symbol.getValue();
    ErrorOr<StringRef> NameOrError = Symbol.getName();
    if (!NameOrError) {
      return errorOrToExpected(std::move(NameOrError));
    }
    StringRef Name = *NameOrError;
    if (!Name.empty()) {
      SymbolName = Name.data();
      return SymbolName;
    }
  }
  return StringRef();
}

Expected<uint64_t>
MachOMetadata::getResolvedValueFromAddress64(uint64_t Address) const {
  uint64_t ptr;
  if (auto p = getValueFromAddress(Address, sizeof(ptr)))
    memcpy(&ptr, p->data(), sizeof(ptr));
  else
    return p.takeError();
  if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
    sys::swapByteOrder(ptr);
  uint64_t offset;
  if (auto symbol = getSymbolFromAddress(Address, offset))
    ptr += offset;
  else
    return symbol.takeError();
  return ptr;
}

Expected<uint32_t>
MachOMetadata::getResolvedValueFromAddress32(uint64_t Address) const {
  uint32_t ptr;
  if (auto p = getValueFromAddress(Address, sizeof(ptr)))
    memcpy(&ptr, p->data(), sizeof(ptr));
  else
    return p.takeError();
  if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
    sys::swapByteOrder(ptr);
  uint64_t offset;
  if (auto symbol = getSymbolFromAddress(Address, offset))
    ptr += offset;
  else
    return symbol.takeError();
  return ptr;
}

Expected<ObjCRef> MachOMetadata::getObjC2ClassRO64(ObjCClass Data) const {
  assert(isObjC2() && OwningBinary->is64Bit() &&
         "Only support 64 bit Obj2 runtime");
  if (auto class_ro = getResolvedValueFromAddress64(
          Data.getRawContent().getValue() + offsetof(class64_t, data))) {
    return *class_ro & ~0x7;
  } else
    return class_ro.takeError();
}

Expected<ObjCRef> MachOMetadata::getObjC2ClassRO32(ObjCClass Data) const {
  assert(isObjC2() && !OwningBinary->is64Bit() &&
         "Only support 32 bit Obj2 runtime");
  if (auto class_ro = getResolvedValueFromAddress32(
          Data.getRawContent().getValue() + offsetof(class32_t, data))) {
    return *class_ro & ~0x3;
  } else
    return class_ro.takeError();
}

Expected<ObjCClass> MachOMetadata::getObjC2MetaClass64(ObjCClass Data) const {
  assert(isObjC2() && OwningBinary->is64Bit() &&
         "Only support 64 bit Obj2 runtime");
  if (auto isa = getResolvedValueFromAddress64(Data.getRawContent().getValue() +
                                               offsetof(class64_t, isa))) {
    if (*isa) {
      return ObjCClass(this, *isa);
    } else
      return ObjCClass(this);
  } else
    return isa.takeError();
}

Expected<ObjCClass> MachOMetadata::getObjC2MetaClass32(ObjCClass Data) const {
  assert(isObjC2() && !OwningBinary->is64Bit() &&
         "Only support 32 bit Obj2 runtime");
  if (auto isa = getResolvedValueFromAddress32(Data.getRawContent().getValue() +
                                               offsetof(class32_t, isa))) {
    if (*isa) {
      return ObjCClass(this, *isa);
    } else
      return ObjCClass(this);
  } else
    return isa.takeError();
}
