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

#include "tapi/ObjCMetadata/ObjCMachOBinary.h"
#include "macho-obj.h"

#include "tapi/ObjCMetadata/ObjCMetadata.h"
#include "llvm/Object/Error.h"
#include "llvm/Object/MachO.h"

using namespace llvm;
using namespace object;

static Error unsupportedObjCRuntimeError() {
  return make_error<StringError>("Unsupported ObjC Runtime",
                                 object_error::parse_failed);
}

MachOMetadata::MachOMetadata(object::MachOObjectFile *Binary, Error &Err)
    : OwningBinary(Binary) {
  // Identify ObjC Version.
  if (getSection("__OBJC", "__module_info") != SectionRef() &&
      !OwningBinary->is64Bit())
    ObjCVersion = ObjC1;
  else if ((getSection("__OBJC2", "__module_info") != SectionRef()) ||
           (getSection("__DATA_CONST", "__objc_imageinfo") != SectionRef()) ||
           (getSection("__DATA", "__objc_imageinfo") != SectionRef()))
    ObjCVersion = ObjC2;
  else
    ObjCVersion = Unknown;

  // Build Address to symbol name lookup.
  struct SectionInfo {
    uint64_t Address;
    uint64_t Size;
    uint64_t OffsetInSegment;
    uint64_t SegmentStartAddress;
    uint32_t SegmentIndex;
  };
  SmallVector<SectionInfo, 32> Sections;
  uint32_t CurSegIndex = Binary->hasPageZeroSegment() ? 1 : 0;
  StringRef CurSegName;
  uint64_t CurSegAddress;
  for (const SectionRef &Section : Binary->sections()) {
    SectionInfo Info;
    Info.Address = Section.getAddress();
    Info.Size = Section.getSize();
    StringRef SegmentName =
        Binary->getSectionFinalSegmentName(Section.getRawDataRefImpl());
    if (!SegmentName.equals(CurSegName)) {
      ++CurSegIndex;
      CurSegName = SegmentName;
      CurSegAddress = Info.Address;
    }
    Info.SegmentIndex = CurSegIndex - 1;
    Info.OffsetInSegment = Info.Address - CurSegAddress;
    Info.SegmentStartAddress = CurSegAddress;
    Sections.push_back(Info);
  }

  // Cache export entries for lookups.
  StringMap<uint64_t> ExportsToAddress;
  for (const auto &Export : OwningBinary->exports(Err))
    ExportsToAddress[Export.name()] = Export.address();

  if (Err)
    return;

  // Cache the address of binds from the binary. Since we are walking ObjC
  // Metadata, the external symbols cannot be weak binds.

  // For binaries uses chained fixups.
    // FIXME: Threaded rebase logic should move to libObject.
    uint64_t TextAddress = 0;
    for (const auto &Command : Binary->load_commands()) {
      if (Command.C.cmd == MachO::LC_SEGMENT) {
        MachO::segment_command SLC = Binary->getSegmentLoadCommand(Command);
        if (StringRef(SLC.segname) == StringRef("__TEXT")) {
          TextAddress = SLC.vmaddr;
          break;
        }
      } else if (Command.C.cmd == MachO::LC_SEGMENT_64) {
        MachO::segment_command_64 SLC_64 =
            Binary->getSegment64LoadCommand(Command);
        if (StringRef(SLC_64.segname) == StringRef("__TEXT")) {
          TextAddress = SLC_64.vmaddr;
          break;
        }
      }
    }
    // For none chained fixup, walk bind table and rebase table.
    for (const auto &Entry : OwningBinary->bindTable(Err)) {
      if (Err)
        return;
      if (Entry.ordinal() == MachO::BIND_SPECIAL_DYLIB_SELF) {
        // This is a bind to local symbol. Resolve it to actual address.
        auto Addr = ExportsToAddress.find(Entry.symbolName());
        if (Addr != ExportsToAddress.end()) {
          VMAddrPointToValueMap[Entry.address()] = Addr->second;
          continue;
        }
      }
      VMAddrToSymbolMap[Entry.address()] = Entry.symbolName();
    }
    for (const auto &Entry : OwningBinary->rebaseTable(Err)) {
      if (Err) {
        // Workaround: walking all the rebase entries can return errors
        // due to <rdar://problem/59527883> and <rdar://problem/59555514>.
        // Ignore the errors when it one.
        consumeError(std::move(Err));
        Err = Error::success();
        break;
      }
    }
    if (Err) {
      consumeError(std::move(Err));
      Err = Error::success();
    }

  // cache all external relocations.
  for (const auto &S : Binary->sections()) {
    for (const auto &Reloc : S.relocations()) {
      uint64_t Address = S.getAddress() + Reloc.getOffset();
      auto Rel = Reloc.getRawDataRefImpl();
      auto RE = Binary->getRelocation(Rel);
      if (Binary->isRelocationScattered(RE) ||
          !Binary->getPlainRelocationExternal(RE))
        continue;

      uint64_t PtrValue = 0;
      if (Binary->is64Bit()) {
        auto Addend = getData<uint64_t>(Address);
        if (!Addend) {
          consumeError(Addend.takeError());
          continue;
        }
        PtrValue = *Addend;
      } else {
        auto Addend = getData<uint32_t>(Address);
        if (!Addend) {
          consumeError(Addend.takeError());
          continue;
        }
        PtrValue = *Addend;
      }
      if (OwningBinary->getAnyRelocationType(RE) ==
          MachO::ARM64_RELOC_AUTHENTICATED_POINTER) {
        PtrValue = 0xffffffffULL & PtrValue;
        if (PtrValue & 0x80000000ULL) {
          PtrValue |= 0xffffffff00000000ULL;
        }
      }
      // Cache the external relocation names as well.
      auto Symbol = Reloc.getSymbol();
      auto NameOrError = Symbol->getName();
      if (!NameOrError) {
        consumeError(NameOrError.takeError());
        continue;
      }
      VMAddrToSymbolMap[Address] = *NameOrError;
      Expected<uint64_t> SymbolVal = Symbol->getValue();
      if (!SymbolVal)
        Err = SymbolVal.takeError();
      PtrValue += *SymbolVal;
      VMAddrPointToValueMap[Address] = PtrValue;
    }
  }
}

Expected<ObjCClassList> MachOMetadata::classes() const {
  ObjCClassList ClassList;
  if (isObjC2()) {
    SectionRef ClassSect = getSection("__DATA_CONST", "__objc_classlist");
    if (ClassSect == SectionRef())
      ClassSect = getSection("__DATA", "__objc_classlist");
    if (ClassSect == SectionRef())
      ClassSect = getSection("__OBJC2", "__class_list");
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
  } else if (isObjC1()) {
    auto ObjCModules = getObjC1Modules();
    if (!ObjCModules)
      return ObjCModules.takeError();
    for (auto &M : *ObjCModules) {
      auto sym = getObjC1Symtab(M);
      if (!sym)
        return sym.takeError();
      // Handle empty symtab.
      if (sym->getValue() == 0)
        continue;
      if (auto cls = getObjC1ClassesFromSymtab(*sym))
        ClassList.append(cls->begin(), cls->end());
      else
        return cls.takeError();
    }
  }

  return ClassList;
}

Expected<ObjCClass> MachOMetadata::getObjCClassFromRef(ObjCClassRef Ref) const {
  if (OwningBinary->is64Bit()) {
    if (auto class_ptr = getPointerValue64(Ref.getAddress().getValue()))
      return ObjCClass(this, *class_ptr);
    else
      return class_ptr.takeError();
  } else {
    if (auto class_ptr = getPointerValue32(Ref.getAddress().getValue()))
      return ObjCClass(this, *class_ptr);
    else
      return class_ptr.takeError();
  }
}

bool MachOMetadata::isObjCClassExternal(ObjCClassRef Ref) const {
  // If there are external relocations or external symbols at the address,
  // return true;
  if (VMAddrToSymbolMap.count(Ref.getAddress().getValue()))
    return true;

  // If the pointer is nullptr, just return true.
  if (OwningBinary->is64Bit()) {
    if (auto Value = getPointerValue64(Ref.getAddress().getValue()))
      return !*Value;
    else {
      consumeError(Value.takeError());
      return true;
    }
  } else {
    if (auto Value = getPointerValue32(Ref.getAddress().getValue()))
      return !*Value;
    else {
      consumeError(Value.takeError());
      return true;
    }
  }

  return false;
}

StringRef MachOMetadata::getSymbolNameFromRef(ObjCRef Ref) const {
  return getSymbol(Ref.getValue());
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
  ObjCCategoryList CategoryList;
  if (isObjC2()) {
    SectionRef CatSect = getSection("__DATA_CONST", "__objc_catlist");
    if (CatSect == SectionRef())
      CatSect = getSection("__DATA", "__objc_catlist");
    if (CatSect == SectionRef())
      CatSect = getSection("__OBJC2", "__category_list");
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
  } else if (isObjC1()) {
    auto ObjCModules = getObjC1Modules();
    if (!ObjCModules)
      return ObjCModules.takeError();
    for (auto &M : *ObjCModules) {
      auto sym = getObjC1Symtab(M);
      if (!sym)
        return sym.takeError();
      // Handle empty symtab.
      if (sym->getValue() == 0)
        continue;
      if (auto cat = getObjC1CategoriesFromSymtab(*sym))
        CategoryList.append(cat->begin(), cat->end());
      else
        return cat.takeError();
    }
  }

  return CategoryList;
}

Expected<ObjCCategory>
MachOMetadata::getObjCCategoryFromRef(ObjCCategoryRef Ref) const {
  if (OwningBinary->is64Bit()) {
    if (auto cat_ptr = getPointerValue64(Ref.getAddress().getValue()))
      return ObjCCategory(this, *cat_ptr);
    else
      return cat_ptr.takeError();
  } else {
    if (auto cat_ptr = getPointerValue32(Ref.getAddress().getValue()))
      return ObjCCategory(this, *cat_ptr);
    else
      return cat_ptr.takeError();
  }
}

Expected<ObjCProtocolList> MachOMetadata::protocols() const {
  ObjCProtocolList ProtocolList;
  if (isObjC2()) {
    SectionRef ProtoSect = getSection("__DATA_CONST", "__objc_protolist");
    if (ProtoSect == SectionRef())
      ProtoSect = getSection("__DATA", "__objc_protolist");
    if (ProtoSect == SectionRef())
      ProtoSect = getSection("__OBJC2", "__protocol_list");
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
  }
  // TODO: Add support for ObjC1, return empty list for now.
  return ProtocolList;
}

Expected<ObjCProtocol>
MachOMetadata::getObjCProtocolFromRef(ObjCProtocolRef Ref) const {
  if (OwningBinary->is64Bit()) {
    if (auto proto_ptr = getPointerValue64(Ref.getAddress().getValue()))
      return ObjCProtocol(this, *proto_ptr);
    else
      return proto_ptr.takeError();
  } else {
    if (auto proto_ptr = getPointerValue32(Ref.getAddress().getValue()))
      return ObjCProtocol(this, *proto_ptr);
    else
      return proto_ptr.takeError();
  }
}

Expected<StringRef> MachOMetadata::getObjCClassName(ObjCClass ClassRef) const {
  if (isObjC2()) {
    if (OwningBinary->is64Bit()) {
      if (auto ro = getObjC2ClassRO64(ClassRef)) {
        if (auto name = getPointerValue64(ro->getValue() +
                                          offsetof(class_ro64_t, name)))
          return getString(*name);
        else
          return name.takeError();
      } else
        return ro.takeError();
    } else {
      if (auto ro = getObjC2ClassRO32(ClassRef)) {
        if (auto name = getPointerValue32(ro->getValue() +
                                          offsetof(class_ro32_t, name)))
          return getString(*name);
        else
          return name.takeError();
      } else
        return ro.takeError();
    }
  } else if (isObjC1()) {
    if (auto name = getPointerValue32(ClassRef.getAddress().getValue() +
                                      offsetof(objc_class_t, name)))
      return getString(*name);
    else
      return name.takeError();
  }
  return unsupportedObjCRuntimeError();
}

Expected<StringRef> MachOMetadata::getObjCSuperClassName(ObjCClass Data) const {
  if (isObjC2()) {
    if (OwningBinary->is64Bit()) {
      uint64_t superAddr =
          Data.getAddress().getValue() + offsetof(class64_t, superclass);
      ObjCClassRef super(this, superAddr);
      return super.getClassName();
    } else {
      uint64_t superAddr =
          Data.getAddress().getValue() + offsetof(class32_t, superclass);
      ObjCClassRef super(this, superAddr);
      return super.getClassName();
    }
  } else if (isObjC1()) {
    if (auto super = getPointerValue32(Data.getAddress().getValue() +
                                       offsetof(objc_class_t, super_class))) {
      if (*super)
        return getString(*super);
      else
        return StringRef();
    } else
      return super.takeError();
  }
  return unsupportedObjCRuntimeError();
}

Expected<bool> MachOMetadata::isObjCClassSwift(ObjCClass Data) const {
  if (isObjC1())
    return false;
  if (OwningBinary->is64Bit()) {
    if (auto class_ro = getPointerValue64(Data.getAddress().getValue() +
                                          offsetof(class64_t, data))) {
      return ((*class_ro & ~0x7) != 0);
    } else
      return class_ro.takeError();
  } else {
    if (auto class_ro = getPointerValue32(Data.getAddress().getValue() +
                                          offsetof(class32_t, data))) {
      return ((*class_ro & ~0x3) != 0);
    } else
      return class_ro.takeError();
  }
}

Expected<bool> MachOMetadata::isObjCClassMetaClass(ObjCClass Data) const {
  if (isObjC2()) {
    if (OwningBinary->is64Bit()) {
      if (auto class_ro = getObjC2ClassRO64(Data)) {
        if (auto flags = getData<uint32_t>(class_ro->getValue() +
                                           offsetof(class_ro64_t, flags))) {
          return (*flags & RO_META);
        } else
          return flags.takeError();
      } else
        return class_ro.takeError();
    } else {
      if (auto class_ro = getObjC2ClassRO32(Data)) {
        if (auto flags = getData<uint32_t>(class_ro->getValue() +
                                           offsetof(class_ro32_t, flags))) {
          return (*flags & RO_META);
        } else
          return flags.takeError();
      } else
        return class_ro.takeError();
    }
  } else if (isObjC1()) {
    if (auto ClassInfo = getData<int32_t>(Data.getAddress().getValue() +
                                          offsetof(objc_class_t, info))) {
      return (*ClassInfo & CLS_META);
    } else
      return ClassInfo.takeError();
  }
  return unsupportedObjCRuntimeError();
}

Expected<ObjCPropertyList>
MachOMetadata::materializePropertyList(ObjCClass ClassRef) const {
  ObjCPropertyList PropertyList;
  if (isObjC2()) {
    if (OwningBinary->is64Bit()) {
      auto RO = getObjC2ClassRO64(ClassRef);
      if (!RO)
        return RO.takeError();

      auto BaseProperties = getPointerValue64(
          RO->getValue() + offsetof(class_ro64_t, baseProperties));
      if (!BaseProperties)
        return BaseProperties.takeError();
      if (!*BaseProperties)
        return PropertyList;

      auto PropList = getData<objc_property_list64>(*BaseProperties);
      if (!PropList)
        return PropList.takeError();
      for (unsigned Idx = 0; Idx < PropList->count; ++Idx)
        PropertyList.emplace_back(this, *BaseProperties +
                                            sizeof(objc_property_list64) +
                                            Idx * sizeof(objc_property64));

      return PropertyList;
    } else {
      auto RO = getObjC2ClassRO32(ClassRef);
      if (!RO)
        return RO.takeError();

      auto BaseProperties = getPointerValue32(
          RO->getValue() + offsetof(class_ro32_t, baseProperties));
      if (!BaseProperties)
        return BaseProperties.takeError();
      if (!*BaseProperties)
        return PropertyList;

      auto PropList = getData<objc_property_list32>(*BaseProperties);
      if (!PropList)
        return PropList.takeError();
      for (unsigned Idx = 0; Idx < PropList->count; ++Idx)
        PropertyList.emplace_back(this, *BaseProperties +
                                            sizeof(objc_property_list32) +
                                            Idx * sizeof(objc_property32));
      return PropertyList;
    }
  } else if (isObjC1()) {
    // Just return empty list.
    return PropertyList;
  }
  return unsupportedObjCRuntimeError();
}

Expected<ObjCPropertyList>
MachOMetadata::materializeClassPropertyList(ObjCClass ClassRef) const {
  if (isObjC2()) {
    if (OwningBinary->is64Bit()) {
      if (auto meta = getObjC2MetaClass64(ClassRef)) {
        if (meta->getAddress().getValue() == 0)
          return ObjCPropertyList();
        else
          return materializePropertyList(*meta);
      } else
        return meta.takeError();
    } else {
      if (auto meta = getObjC2MetaClass32(ClassRef)) {
        if (meta->getAddress().getValue() == 0)
          return ObjCPropertyList();
        else
          return materializePropertyList(*meta);
      } else
        return meta.takeError();
    }
  } else if (isObjC1())
    return ObjCPropertyList();

  return unsupportedObjCRuntimeError();
}

Expected<StringRef> MachOMetadata::getPropertyName(ObjCProperty Data) const {
  if (!isObjC2())
    return unsupportedObjCRuntimeError();
  if (OwningBinary->is64Bit()) {
    auto NamePtr = getPointerValue64(Data.getAddress().getValue() +
                                     offsetof(objc_property64, name));
    if (!NamePtr)
      return NamePtr.takeError();
    return getString(*NamePtr);
  } else {
    auto NamePtr = getPointerValue32(Data.getAddress().getValue() +
                                     offsetof(objc_property32, name));
    if (!NamePtr)
      return NamePtr.takeError();
    return getString(*NamePtr);
  }
}

Expected<StringRef>
MachOMetadata::getPropertyAttribute(ObjCProperty Data) const {
  if (!isObjC2())
    return unsupportedObjCRuntimeError();
  if (OwningBinary->is64Bit()) {
    auto NamePtr = getPointerValue64(Data.getAddress().getValue() +
                                     offsetof(objc_property64, attributes));
    if (!NamePtr)
      return NamePtr.takeError();
    return getString(*NamePtr);
  } else {
    auto NamePtr = getPointerValue32(Data.getAddress().getValue() +
                                     offsetof(objc_property32, attributes));
    if (!NamePtr)
      return NamePtr.takeError();
    return getString(*NamePtr);
  }
}

Expected<ObjCMethodList>
MachOMetadata::materializeInstanceMethodList(ObjCClass ClassRef) const {
  if (isObjC2()) {
    ObjCMethodList MethodList;
    if (OwningBinary->is64Bit()) {
      auto RO = getObjC2ClassRO64(ClassRef);
      if (!RO)
        return RO.takeError();
      auto BaseMethods = getPointerValue64(RO->getValue() +
                                           offsetof(class_ro64_t, baseMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;

      auto MList = getData<method_list64_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();
      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method64_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list64_t) + Idx * EntrySize,
            RA));
      return MethodList;
    } else {
      auto RO = getObjC2ClassRO32(ClassRef);
      if (!RO)
        return RO.takeError();
      auto BaseMethods = getPointerValue32(RO->getValue() +
                                           offsetof(class_ro32_t, baseMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;

      auto MList = getData<method_list32_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();
      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method32_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list32_t) + Idx * EntrySize,
            RA));
      return MethodList;
    }
  } else if (isObjC1()) {
    uint64_t Methods =
        ClassRef.getAddress().getValue() + offsetof(objc_class_t, methodLists);
    return getObjC1MethodList(ObjCRef(Methods));
  }
  return unsupportedObjCRuntimeError();
}

Expected<ObjCMethodList>
MachOMetadata::materializeClassMethodList(ObjCClass ClassRef) const {
  if (isObjC2()) {
    if (OwningBinary->is64Bit()) {
      if (auto meta = getObjC2MetaClass64(ClassRef)) {
        if (meta->getAddress().getValue() == 0)
          return ObjCMethodList();
        else
          return materializeInstanceMethodList(*meta);
      } else
        return meta.takeError();
    } else {
      if (auto meta = getObjC2MetaClass32(ClassRef)) {
        if (meta->getAddress().getValue() == 0)
          return ObjCMethodList();
        else
          return materializeInstanceMethodList(*meta);
      } else
        return meta.takeError();
    }
  } else if (isObjC1()) {
    ObjCClassRef isa(this, ClassRef.getAddress().getValue() +
                               offsetof(objc_class_t, isa));
    auto C = getObjCClassFromRef(isa);
    if (!C)
      return C.takeError();
    return materializeInstanceMethodList(*C);
  }
  return unsupportedObjCRuntimeError();
}

Expected<StringRef> MachOMetadata::getObjCCategoryName(ObjCCategory Cat) const {
  if (isObjC2()) {
    if (OwningBinary->is64Bit()) {
      auto CatPtr = getPointerValue64(Cat.getAddress().getValue() +
                                      offsetof(category64_t, name));
      if (!CatPtr)
        return CatPtr.takeError();
      return getString(*CatPtr);
    } else {
      auto CatPtr = getPointerValue32(Cat.getAddress().getValue() +
                                      offsetof(category32_t, name));
      if (!CatPtr)
        return CatPtr.takeError();
      return getString(*CatPtr);
    }
  } else if (isObjC1()) {
    auto NamePtr = getPointerValue32(Cat.getAddress().getValue() +
                                     offsetof(objc_category_t, category_name));
    if (!NamePtr)
      return NamePtr.takeError();
    return getString(*NamePtr);
  }

  return unsupportedObjCRuntimeError();
}

Expected<StringRef>
MachOMetadata::getObjCCategoryBaseClassName(ObjCCategory Cat) const {
  if (isObjC2()) {
    if (OwningBinary->is64Bit()) {
      uint64_t baseClassAddr =
          Cat.getAddress().getValue() + offsetof(category64_t, cls);
      ObjCClassRef cls(this, baseClassAddr);
      if (auto clsName = cls.getClassName())
        return *clsName;
      else
        return clsName.takeError();
    } else {
      uint64_t baseClassAddr =
          Cat.getAddress().getValue() + offsetof(category32_t, cls);
      ObjCClassRef cls(this, baseClassAddr);
      if (auto clsName = cls.getClassName())
        return *clsName;
      else
        return clsName.takeError();
    }
  } else if (isObjC1()) {
    auto NamePtr = getPointerValue32(Cat.getAddress().getValue() +
                                     offsetof(objc_category_t, class_name));
    if (!NamePtr)
      return NamePtr.takeError();
    return getString(*NamePtr);
  }

  return unsupportedObjCRuntimeError();
}

Expected<ObjCPropertyList>
MachOMetadata::materializePropertyList(ObjCCategory Data) const {
  if (isObjC2()) {
    ObjCPropertyList PropertyList;
    if (OwningBinary->is64Bit()) {
      auto BaseProperties =
          getPointerValue64(Data.getAddress().getValue() +
                            offsetof(category64_t, instanceProperties));
      if (!BaseProperties)
        return BaseProperties.takeError();
      if (!*BaseProperties)
        return PropertyList;
      auto PropList = getData<objc_property_list64>(*BaseProperties);
      if (!PropList)
        return PropList.takeError();

      for (unsigned Idx = 0; Idx < PropList->count; ++Idx)
        PropertyList.push_back(
            ObjCProperty(this, *BaseProperties + sizeof(objc_property_list64) +
                                   Idx * sizeof(objc_property64)));
      return PropertyList;
    } else {
      auto BaseProperties =
          getPointerValue32(Data.getAddress().getValue() +
                            offsetof(category32_t, instanceProperties));
      if (!BaseProperties)
        return BaseProperties.takeError();
      if (!*BaseProperties)
        return PropertyList;
      auto PropList = getData<objc_property_list32>(*BaseProperties);
      if (!PropList)
        return PropList.takeError();

      for (unsigned Idx = 0; Idx < PropList->count; ++Idx)
        PropertyList.push_back(
            ObjCProperty(this, *BaseProperties + sizeof(objc_property_list32) +
                                   Idx * sizeof(objc_property32)));
      return PropertyList;
    }
  } else if (isObjC1())
    return ObjCPropertyList();

  return unsupportedObjCRuntimeError();
}

Expected<ObjCMethodList>
MachOMetadata::materializeInstanceMethodList(ObjCCategory Data) const {
  if (isObjC2()) {
    ObjCMethodList MethodList;
    if (OwningBinary->is64Bit()) {
      auto BaseMethods =
          getPointerValue64(Data.getAddress().getValue() +
                            offsetof(category64_t, instanceMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;
      auto MList = getData<method_list64_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();

      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method64_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list64_t) + Idx * EntrySize,
            RA));
      return MethodList;
    } else {
      auto BaseMethods =
          getPointerValue32(Data.getAddress().getValue() +
                            offsetof(category32_t, instanceMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;
      auto MList = getData<method_list32_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();

      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method32_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list32_t) + Idx * EntrySize,
            RA));
      return MethodList;
    }
  } else if (isObjC1()) {
    uint64_t Methods = Data.getAddress().getValue() +
                       offsetof(objc_category_t, instance_methods);
    return getObjC1MethodList(ObjCRef(Methods));
  }
  return unsupportedObjCRuntimeError();
}

Expected<ObjCMethodList>
MachOMetadata::materializeClassMethodList(ObjCCategory Data) const {
  if (isObjC2()) {
    ObjCMethodList MethodList;
    if (OwningBinary->is64Bit()) {
      auto BaseMethods = getPointerValue64(
          Data.getAddress().getValue() + offsetof(category64_t, classMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;
      auto MList = getData<method_list64_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();

      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method64_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list64_t) + Idx * EntrySize,
            RA));
      return MethodList;
    } else {
      auto BaseMethods = getPointerValue32(
          Data.getAddress().getValue() + offsetof(category32_t, classMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;
      auto MList = getData<method_list32_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();

      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method32_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list32_t) + Idx * EntrySize,
            RA));
      return MethodList;
    }
  } else if (isObjC1()) {
    uint64_t Methods =
        Data.getAddress().getValue() + offsetof(objc_category_t, class_methods);
    return getObjC1MethodList(ObjCRef(Methods));
  }
  return unsupportedObjCRuntimeError();
}

Expected<StringRef>
MachOMetadata::getObjCProtocolName(ObjCProtocol Data) const {
  if (isObjC2()) {
    if (OwningBinary->is64Bit()) {
      auto NamePtr = getPointerValue64(Data.getAddress().getValue() +
                                       offsetof(protocol64_t, name));
      if (!NamePtr)
        return NamePtr.takeError();
      return getString(*NamePtr);
    } else {
      auto NamePtr = getPointerValue32(Data.getAddress().getValue() +
                                       offsetof(protocol32_t, name));
      if (!NamePtr)
        return NamePtr.takeError();
      return getString(*NamePtr);
    }
  }
  return unsupportedObjCRuntimeError();
}

Expected<ObjCPropertyList>
MachOMetadata::materializePropertyList(ObjCProtocol Data) const {
  if (isObjC2()) {
    ObjCPropertyList PropertyList;
    if (OwningBinary->is64Bit()) {
      auto BaseProperties =
          getPointerValue64(Data.getAddress().getValue() +
                            offsetof(protocol64_t, instanceProperties));
      if (!BaseProperties)
        return BaseProperties.takeError();
      if (!*BaseProperties)
        return PropertyList;
      auto PropList = getData<objc_property_list64>(*BaseProperties);
      if (!PropList)
        return PropList.takeError();

      for (unsigned Idx = 0; Idx < PropList->count; ++Idx)
        PropertyList.push_back(
            ObjCProperty(this, *BaseProperties + sizeof(objc_property_list64) +
                                   Idx * sizeof(objc_property64)));
      return PropertyList;
    } else {
      auto BaseProperties =
          getPointerValue32(Data.getAddress().getValue() +
                            offsetof(protocol32_t, instanceProperties));
      if (!BaseProperties)
        return BaseProperties.takeError();
      if (!*BaseProperties)
        return PropertyList;
      auto PropList = getData<objc_property_list32>(*BaseProperties);
      if (!PropList)
        return PropList.takeError();

      for (unsigned Idx = 0; Idx < PropList->count; ++Idx)
        PropertyList.push_back(
            ObjCProperty(this, *BaseProperties + sizeof(objc_property_list32) +
                                   Idx * sizeof(objc_property32)));
      return PropertyList;
    }
  }
  return unsupportedObjCRuntimeError();
}

Expected<ObjCMethodList>
MachOMetadata::materializeInstanceMethodList(ObjCProtocol Data) const {
  if (isObjC2()) {
    ObjCMethodList MethodList;
    if (OwningBinary->is64Bit()) {
      auto BaseMethods =
          getPointerValue64(Data.getAddress().getValue() +
                            offsetof(protocol64_t, instanceMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;
      auto MList = getData<method_list64_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();
      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method64_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list64_t) + Idx * EntrySize,
            RA));
      return MethodList;
    } else {
      auto BaseMethods =
          getPointerValue32(Data.getAddress().getValue() +
                            offsetof(protocol32_t, instanceMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;
      auto MList = getData<method_list32_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();
      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method32_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list32_t) + Idx * EntrySize,
            RA));
      return MethodList;
    }
  }
  return unsupportedObjCRuntimeError();
}

Expected<ObjCMethodList>
MachOMetadata::materializeClassMethodList(ObjCProtocol Data) const {
  if (isObjC2()) {
    ObjCMethodList MethodList;
    if (OwningBinary->is64Bit()) {
      auto BaseMethods = getPointerValue64(
          Data.getAddress().getValue() + offsetof(protocol64_t, classMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;
      auto MList = getData<method_list64_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();
      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method64_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list64_t) + Idx * EntrySize,
            RA));
      return MethodList;
    } else {
      auto BaseMethods = getPointerValue32(
          Data.getAddress().getValue() + offsetof(protocol32_t, classMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;
      auto MList = getData<method_list32_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();
      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method32_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list32_t) + Idx * EntrySize,
            RA));
      return MethodList;
    }
  }
  return unsupportedObjCRuntimeError();
}

Expected<ObjCMethodList>
MachOMetadata::materializeOptionalInstanceMethodList(ObjCProtocol Data) const {
  if (isObjC2()) {
    ObjCMethodList MethodList;
    if (OwningBinary->is64Bit()) {
      auto BaseMethods =
          getPointerValue64(Data.getAddress().getValue() +
                            offsetof(protocol64_t, optionalInstanceMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;
      auto MList = getData<method_list64_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();
      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method64_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list64_t) + Idx * EntrySize,
            RA));
      return MethodList;
    } else {
      auto BaseMethods =
          getPointerValue32(Data.getAddress().getValue() +
                            offsetof(protocol32_t, optionalInstanceMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;
      auto MList = getData<method_list32_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();
      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method32_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list32_t) + Idx * EntrySize,
            RA));
      return MethodList;
    }
  }
  return unsupportedObjCRuntimeError();
}

Expected<ObjCMethodList>
MachOMetadata::materializeOptionalClassMethodList(ObjCProtocol Data) const {
  if (isObjC2()) {
    ObjCMethodList MethodList;
    if (OwningBinary->is64Bit()) {
      auto BaseMethods =
          getPointerValue64(Data.getAddress().getValue() +
                            offsetof(protocol64_t, optionalClassMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;
      auto MList = getData<method_list64_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();
      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method64_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list64_t) + Idx * EntrySize,
            RA));
      return MethodList;
    } else {
      auto BaseMethods =
          getPointerValue32(Data.getAddress().getValue() +
                            offsetof(protocol32_t, optionalClassMethods));
      if (!BaseMethods)
        return BaseMethods.takeError();
      if (!*BaseMethods)
        return MethodList;
      auto MList = getData<method_list32_t>(*BaseMethods);
      if (!MList)
        return MList.takeError();
      bool RA = MList->entsize & METHOD_LIST_ENTSIZE_FLAG_RELATIVE;
      auto EntrySize = RA ? sizeof(method_rel_t) : sizeof(method32_t);
      for (unsigned Idx = 0; Idx < MList->count; ++Idx)
        MethodList.push_back(ObjCMethod(
            this, *BaseMethods + sizeof(method_list32_t) + Idx * EntrySize,
            RA));
      return MethodList;
    }
  }
  return unsupportedObjCRuntimeError();
}

Expected<StringRef> MachOMetadata::getMethodName(ObjCMethod Data) const {
  if (isObjC1() || isObjC2()) {
    if (OwningBinary->is64Bit()) {
      if (Data.isRelativeAddress()) {
        uint64_t MethodAddress =
            Data.getAddress().getValue() + offsetof(method_rel_t, name);
        auto MethodOffset = getData<int32_t>(MethodAddress);
        if (!MethodOffset)
          return MethodOffset.takeError();
        auto MethodName = getPointerValue64(MethodAddress + *MethodOffset);
        if (!MethodName)
          return MethodName.takeError();
        return getString(*MethodName, /*allow empty*/ true);
      } else {
        auto MethodName = getPointerValue64(Data.getAddress().getValue() +
                                            offsetof(method64_t, name));
        if (!MethodName)
          return MethodName.takeError();
        return getString(*MethodName);
      }
    } else {
      if (Data.isRelativeAddress()) {
        uint64_t MethodAddress =
            Data.getAddress().getValue() + offsetof(method_rel_t, name);
        auto MethodOffset = getData<int32_t>(MethodAddress);
        if (!MethodOffset)
          return MethodOffset.takeError();
        auto MethodName = getPointerValue32(MethodAddress + *MethodOffset);
        if (!MethodName)
          return MethodName.takeError();
        return getString(*MethodName, /*allow empty*/ true);
      } else {
        auto MethodName = getPointerValue32(Data.getAddress().getValue() +
                                            offsetof(method32_t, name));
        if (!MethodName)
          return MethodName.takeError();
        return getString(*MethodName);
      }
    }
  }
  return unsupportedObjCRuntimeError();
}

Expected<StringRef> MachOMetadata::getMethodType(ObjCMethod Data) const {
  if (isObjC1() || isObjC2()) {
    if (OwningBinary->is64Bit()) {
      if (Data.isRelativeAddress()) {
        uint64_t TypeAddress =
            Data.getAddress().getValue() + offsetof(method_rel_t, types);
        auto TypeOffset = getData<int32_t>(TypeAddress);
        if (!TypeOffset)
          return TypeOffset.takeError();
        // zero offset represents a nullptr.
        if (!*TypeOffset)
          return StringRef();
        return getString(TypeAddress + *TypeOffset);
      } else {
        auto MethodTypes = getPointerValue64(Data.getAddress().getValue() +
                                             offsetof(method64_t, types));
        if (!MethodTypes)
          return MethodTypes.takeError();
        return getString(*MethodTypes);
      }
    } else {
      if (Data.isRelativeAddress()) {
        uint64_t TypeAddress =
            Data.getAddress().getValue() + offsetof(method_rel_t, types);
        auto TypeOffset = getData<int32_t>(TypeAddress);
        if (!TypeOffset)
          return TypeOffset.takeError();
        // zero offset represents a nullptr.
        if (!*TypeOffset)
          return StringRef();
        return getString(TypeAddress + *TypeOffset);
      } else {
        auto MethodTypes = getPointerValue32(Data.getAddress().getValue() +
                                             offsetof(method32_t, types));
        if (!MethodTypes)
          return MethodTypes.takeError();
        return getString(*MethodTypes);
      }
    }
  }
  return unsupportedObjCRuntimeError();
}

Expected<ObjCSelectorList> MachOMetadata::referencedSelectors() const {
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
  if (isObjC2()) {
    if (OwningBinary->is64Bit()) {
      auto Sel = getPointerValue64(Ref.getAddress().getValue());
      if (!Sel)
        return Sel.takeError();
      return getString(*Sel);
    } else {
      auto Sel = getPointerValue32(Ref.getAddress().getValue());
      if (!Sel)
        return Sel.takeError();
      return getString(*Sel);
    }
  }
  return unsupportedObjCRuntimeError();
}

void MachOMetadata::getAllPotentiallyDefinedSelectors(StringSet<> &Set) const {
  // Clear the set.
  Set.clear();
  SectionRef ObjCConstSect = getSection("__DATA_CONST", "__objc_const");
  if (ObjCConstSect == SectionRef())
    ObjCConstSect = getSection("__DATA", "__objc_const");
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
        auto methodName = getPointerValue64(A);
        if (!methodName) {
          ignoreError(methodName.takeError());
          continue;
        }
        if (*methodName >= MethodNameSect.getAddress() &&
            *methodName <
                MethodNameSect.getAddress() + MethodNameSect.getSize()) {
          if (auto name = getString(*methodName)) {
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
        auto methodName = getPointerValue32(A);
        if (!methodName) {
          ignoreError(methodName.takeError());
          continue;
        }
        if (*methodName >= MethodNameSect.getAddress() &&
            *methodName <
                MethodNameSect.getAddress() + MethodNameSect.getSize()) {
          if (auto name = getString(*methodName)) {
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

Expected<StringRef> MachOMetadata::getSwiftVersion() const {
  SectionRef Sect = getSection("__OBJC2", "__module_info");
  if (Sect == SectionRef())
    Sect = getSection("__DATA_CONST", "__objc_imageinfo");
  if (Sect == SectionRef())
    Sect = getSection("__DATA", "__objc_imageinfo");
  if (Sect == SectionRef())
    return "";
  auto SectContents = Sect.getContents();
  if (!SectContents)
    return SectContents.takeError();
  imageInfo_t info;
  if (SectContents->size() < sizeof(info))
    return make_error<StringError>("ObjC Image Info section is too small",
                                   object_error::parse_failed);
  memcpy(&info, SectContents->data(), sizeof(info));
  if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
    swapStruct(info);

  return convertSwiftVersion((info.flags >> 8) & 0xff);
}

const SectionRef MachOMetadata::getSection(const char *segname,
                                           const char *sectname) const {
  for (const SectionRef &Section : OwningBinary->sections()) {
    auto MaybeName = Section.getName();
    if (!MaybeName)
      continue;
    StringRef SectName = *MaybeName;
    DataRefImpl Ref = Section.getRawDataRefImpl();
    StringRef SegName = OwningBinary->getSectionFinalSegmentName(Ref);
    if (SegName == segname && SectName == sectname)
      return Section;
  }
  return SectionRef();
}

Expected<object::SectionRef>
MachOMetadata::getSectionFromAddress(uint64_t Address) const {
  for (const SectionRef &Section : OwningBinary->sections()) {
    uint64_t SectAddress = Section.getAddress();
    uint64_t SectSize = Section.getSize();
    if (SectSize == 0)
      continue;
    if (Address >= SectAddress && Address < SectAddress + SectSize)
      return Section;
  }
  return make_error<StringError>("requested address not in section",
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

StringRef MachOMetadata::getSymbol(uint64_t VMAddr) const {
  auto E = VMAddrToSymbolMap.find(VMAddr);
  if (E == VMAddrToSymbolMap.end())
    return StringRef();

  return E->second;
}

template <typename T>
Expected<T> MachOMetadata::getData(uint64_t VMAddr) const {
  if (isAddressEncrypted(VMAddr))
    return make_error<StringError>("vmaddr is encrypted",
                                   object_error::parse_failed);
  for (const SectionRef &Section : OwningBinary->sections()) {
    uint64_t SectAddress = Section.getAddress();
    uint64_t SectSize = Section.getSize();
    if (SectSize == 0)
      continue;
    if (VMAddr >= SectAddress && VMAddr < SectAddress + SectSize) {
      uint64_t Offset = VMAddr - SectAddress;
      auto SectContents = Section.getContents();
      if (!SectContents)
        return SectContents.takeError();
      if (Offset + sizeof(T) > SectSize)
        return make_error<StringError>(
            "Data extends pass the end of the section",
            object_error::parse_failed);
      T Data;
      memcpy(&Data, SectContents->data() + Offset, sizeof(T));
      if (OwningBinary->isLittleEndian() != sys::IsLittleEndianHost)
        swapStruct(Data);
      return Data;
    }
  }
  return make_error<StringError>("requested address out of bound",
                                 object_error::parse_failed);
}

Expected<StringRef> MachOMetadata::getString(uint64_t VMAddr,
                                             bool AllowEmpty) const {
  if (isAddressEncrypted(VMAddr))
    return "#EncryptedString#";

  for (const SectionRef &Section : OwningBinary->sections()) {
    uint64_t SectAddress = Section.getAddress();
    uint64_t SectSize = Section.getSize();
    if (SectSize == 0)
      continue;
    if (VMAddr >= SectAddress && VMAddr < SectAddress + SectSize) {
      uint64_t Offset = VMAddr - SectAddress;
      auto SectContents = Section.getContents();
      if (!SectContents)
        return SectContents.takeError();
      StringRef Target = StringRef(SectContents->data() + Offset);
      if (Target.size() + Offset >= SectSize)
        return make_error<StringError>(
            "Data extends pass the end of the section",
            object_error::parse_failed);
      if (Target.empty() && !AllowEmpty)
        return make_error<StringError>(
            "Expect to read a none zero length string",
            object_error::parse_failed);

      return Target;
    }
  }
  return make_error<StringError>("string out of bound",
                                 object_error::parse_failed);
}

Expected<uint64_t> MachOMetadata::getPointerValue64(uint64_t VMAddr) const {
  // If there are rebase at the vmaddr, return cached pointer value.
  auto E = VMAddrPointToValueMap.find(VMAddr);
  if (E != VMAddrPointToValueMap.end())
    return E->second;

  // Otherwise, just load the content from the address.
  return getData<uint64_t>(VMAddr);
}

Expected<uint32_t> MachOMetadata::getPointerValue32(uint64_t VMAddr) const {
  auto E = VMAddrPointToValueMap.find(VMAddr);
  if (E != VMAddrPointToValueMap.end())
    return E->second;

  return getData<uint32_t>(VMAddr);
}

Expected<ObjCRef> MachOMetadata::getObjC2ClassRO64(ObjCClass Data) const {
  assert(isObjC2() && OwningBinary->is64Bit() &&
         "Only support 64 bit Obj2 runtime");
  if (auto class_ro = getPointerValue64(Data.getAddress().getValue() +
                                        offsetof(class64_t, data))) {
    return *class_ro & ~0x7;
  } else
    return class_ro.takeError();
}

Expected<ObjCRef> MachOMetadata::getObjC2ClassRO32(ObjCClass Data) const {
  assert(isObjC2() && !OwningBinary->is64Bit() &&
         "Only support 32 bit Obj2 runtime");
  if (auto class_ro = getPointerValue32(Data.getAddress().getValue() +
                                        offsetof(class32_t, data))) {
    return *class_ro & ~0x3;
  } else
    return class_ro.takeError();
}

Expected<ObjCClass> MachOMetadata::getObjC2MetaClass64(ObjCClass Data) const {
  assert(isObjC2() && OwningBinary->is64Bit() &&
         "Only support 64 bit Obj2 runtime");
  if (auto isa = getPointerValue64(Data.getAddress().getValue() +
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
  if (auto isa = getPointerValue32(Data.getAddress().getValue() +
                                   offsetof(class32_t, isa))) {
    if (*isa) {
      return ObjCClass(this, *isa);
    } else
      return ObjCClass(this);
  } else
    return isa.takeError();
}

Expected<ObjCRefList> MachOMetadata::getObjC1Modules() const {
  ObjCRefList Modules;
  auto ModuleInfo = getSection("__OBJC", "__module_info");
  if (ModuleInfo == SectionRef())
    return Modules;

  if (ModuleInfo.getSize() % sizeof(objc_module_t) != 0)
    return make_error<StringError>(
        "__module_info is not a multiple of objc_module_t size",
        object_error::parse_failed);

  for (unsigned i = 0; i < ModuleInfo.getSize(); i += sizeof(objc_module_t))
    Modules.push_back(ModuleInfo.getAddress() + i);

  return Modules;
}

Expected<ObjCRef> MachOMetadata::getObjC1Symtab(ObjCRef ObjCModule) const {
  if (auto sym = getPointerValue32(ObjCModule.getValue() +
                                   offsetof(objc_module_t, symtab)))
    return ObjCRef(*sym);
  else
    return sym.takeError();
}

Expected<ObjCClassList>
MachOMetadata::getObjC1ClassesFromSymtab(ObjCRef Symtab) const {
  ObjCClassList Classes;
  auto def_start = Symtab.getValue() + sizeof(objc_symtab_t);
  auto cls_count = getData<uint16_t>(Symtab.getValue() +
                                     offsetof(objc_symtab_t, cls_def_cnt));
  if (!cls_count)
    return cls_count.takeError();

  auto section = getSectionFromAddress(def_start);
  if (section) {
    if ((def_start + *cls_count) > (section->getAddress() + section->getSize()))
      return make_error<StringError>("Symtab extends out of the range",
                                     object_error::parse_failed);
  } else
    return section.takeError();

  for (unsigned i = 0; i < *cls_count; ++i)
    Classes.push_back(ObjCClassRef(this, def_start + i * sizeof(uint32_t)));

  return Classes;
}

Expected<ObjCCategoryList>
MachOMetadata::getObjC1CategoriesFromSymtab(ObjCRef Symtab) const {
  ObjCCategoryList Categories;
  auto cls_count = getData<uint16_t>(Symtab.getValue() +
                                     offsetof(objc_symtab_t, cls_def_cnt));
  if (!cls_count)
    return cls_count.takeError();

  auto def_start =
      Symtab.getValue() + sizeof(objc_symtab_t) + *cls_count * sizeof(uint32_t);

  auto cat_count = getData<uint16_t>(Symtab.getValue() +
                                     offsetof(objc_symtab_t, cat_def_cnt));
  if (!cat_count)
    return cat_count.takeError();

  auto section = getSectionFromAddress(def_start);
  if (section) {
    if ((def_start + *cat_count) > (section->getAddress() + section->getSize()))
      return make_error<StringError>("Symtab extends out of the range",
                                     object_error::parse_failed);
  } else
    return section.takeError();

  for (unsigned i = 0; i < *cat_count; ++i)
    Categories.push_back(
        ObjCCategoryRef(this, def_start + i * sizeof(uint32_t)));

  return Categories;
}

Expected<ObjCMethodList>
MachOMetadata::getObjC1MethodList(ObjCRef Methods) const {
  auto ML = getPointerValue32(Methods.getValue());
  if (!ML)
    return ML.takeError();
  if (!*ML)
    return ObjCMethodList();

  auto L = getData<objc_method_list_t>(*ML);
  if (!L)
    return L.takeError();

  ObjCMethodList MethodList;
  uint64_t start = *ML + sizeof(objc_method_list_t);

  auto section = getSectionFromAddress(start);
  if (section) {
    if ((start + L->method_count) >
        (section->getAddress() + section->getSize()))
      return make_error<StringError>("Method list extends out of the range",
                                     object_error::parse_failed);
  } else
    return section.takeError();

  for (int32_t i = 0; i < L->method_count; ++i)
    MethodList.push_back(ObjCMethod(this, start + i * sizeof(objc_method_t),
                                    /*relative address=*/false));

  return MethodList;
}
