//===- ObjCBitcode.cpp - Bitcode ObjC Metadata Reader -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the classes needed to parse ObjCMetadata in Bitcode.
//
//===----------------------------------------------------------------------===//

#include "llvm/ObjCMetadata/ObjCBitcode.h"
#include "llvm/ObjCMetadata/ObjCMetadata.h"
#include "llvm/Analysis/InstructionSimplify.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalAlias.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Operator.h"
#include "llvm/Support/Error.h"

#include "macho-obj.h"

using namespace llvm;
using namespace object;

static Value *GetValueAtStructOffset(ConstantStruct *CS, uint64_t offset,
                                     const DataLayout &DL) {
  StructType* ST = CS->getType();
  unsigned index = DL.getStructLayout(ST)->getElementContainingOffset(offset);
  Value* Ret = CS->getOperand(index);
  offset -= DL.getStructLayout(ST)->getElementOffset(index);
  if (offset != 0) {
    CS = dyn_cast<ConstantStruct>(Ret);
    ST = CS->getType();
    return GetValueAtStructOffset(CS, offset, DL);
  }
  return Ret;
}

Value *GetRealValue(Value *GV, const DataLayout &DL) {
  int64_t offset = 0;
  auto basePtr = dyn_cast<GlobalVariable>(
      GetPointerBaseWithConstantOffset(GV, offset, DL));
  if (!basePtr || !basePtr->hasInitializer()) {
      return nullptr;
  }
  if (offset == 0)
    return basePtr->getInitializer();
  else if (auto CS = dyn_cast<ConstantStruct>(basePtr->getInitializer()))
    return GetValueAtStructOffset(CS, offset, DL);
  else
    return nullptr;
}

static User *GlobalOpToUser(Value *v) {
  auto base_user = dyn_cast<User>(v);
  assert(base_user && "Provided value is not a user!");
  auto op0 = dyn_cast<User>(base_user->getOperand(0));
  assert(op0 && "User doesn't have User operands!");
  return op0;
}

// This is basically the GetUnderlyingObject in ValueTracking Analysis,
// except it works through non-pointer types.
static Value *GetUnderlyingValue(Value *V, const DataLayout &DL,
                                 unsigned MaxLookup) {
  for (unsigned Count = 0; MaxLookup == 0 || Count < MaxLookup; ++Count) {
    if (GEPOperator *GEP = dyn_cast<GEPOperator>(V)) {
      V = GEP->getPointerOperand();
    } else if (Operator::getOpcode(V) == Instruction::BitCast ||
               Operator::getOpcode(V) == Instruction::AddrSpaceCast) {
      V = cast<Operator>(V)->getOperand(0);
    } else if (GlobalAlias *GA = dyn_cast<GlobalAlias>(V)) {
      if (GA->mayBeOverridden())
        return V;
      V = GA->getAliasee();
    } else if (PtrToIntOperator *PTI = dyn_cast<PtrToIntOperator>(V)) {
      V = PTI->getPointerOperand();
    } else {
      // See if InstructionSimplify knows any relevant tricks.
      if (Instruction *I = dyn_cast<Instruction>(V))
        // TODO: Acquire a DominatorTree and AssumptionCache and use them.
        if (Value *Simplified = SimplifyInstruction(I, DL, nullptr)) {
          V = Simplified;
          continue;
        }

      return V;
    }
  }
  return V;
}

// Check if a selector is actually defined with corresponding method,
// even though it is not showing up when walking through the objc class
// info. For example, this will find the selector defined in @objc swift
// generic class.
static bool hasUserInMethodList(const Value *V, const Module *M,
                                unsigned maxDepth = 4) {
  // Search through all the uses of the selector string to see if there are any
  // usage in the __DATA,__objc_const section that looks like a method entry.
  for (const auto &user : V->users()) {
    if (auto *GV = dyn_cast<GlobalValue>(user)) {
      if (GV->hasSection()) {
        StringRef Section = GV->getSection();
        std::string SectionName;
        // Trim all spaces in the section name.
        for (auto c : Section) {
          if (c != ' ')
            SectionName.push_back(c);
        }
        // Potential match of a method list.
        // The method list should have type {i32, i32, [n x {i8*, i8*, i8*}]}.
        if (StringRef(SectionName).startswith("__DATA,__objc_const") &&
            V->getType()->isStructTy() &&
            V->getType()->getStructNumElements() == 3 &&
            V->getType()->getStructElementType(0)->isIntegerTy() &&
            V->getType()->getStructElementType(1)->isIntegerTy() &&
            V->getType()->getStructElementType(2)->isArrayTy())
          return true;
      }
    }
    // Recursively find method list.
    if (maxDepth != 0 && hasUserInMethodList(user, M, maxDepth - 1))
      return true;
  }
  return false;
}

BitcodeMetadata::BitcodeMetadata(Module *M) : OwningModule(M) {}

Expected<ObjCClassList> BitcodeMetadata::classes() const {
  ObjCClassList ClassList;
  for (auto &GV : OwningModule->globals()) {
    if (GV.hasSection()) {
      StringRef Section = GV.getSection();
      std::string SectionName;
      // Trim all spaces in the section name.
      for (auto c : Section) {
        if (c != ' ')
          SectionName.push_back(c);
      }
      if (SectionName == "__DATA,__objc_classlist,regular,no_dead_strip") {
        auto structList = dyn_cast<Constant>(
            GetUnderlyingObject(GV.getInitializer(), getDataLayout(), 2));
        for (const auto &op : structList->operands())
          ClassList.push_back(ObjCClassRef(this, cast<Constant>(&op)));
      }
    }
  }

  return ClassList;
}

Expected<ObjCClass>
BitcodeMetadata::getObjCClassFromRef(ObjCClassRef Ref) const {
  auto GV = Ref.getRawContent().getLLVMConstant();
  int64_t class_offset = 0;
  auto objc_class = dyn_cast<GlobalVariable>(
      GetPointerBaseWithConstantOffset(GV, class_offset, getDataLayout()));
  if (!objc_class)
    return make_error<StringError>("Unable to find class associsated",
                                   object_error::parse_failed);

  return ObjCClass(this, {objc_class, class_offset});
}

bool BitcodeMetadata::isObjCClassExternal(ObjCClassRef Ref) const {
  if (auto GV = cast<GlobalVariable>(Ref.getRawContent().getLLVMConstant())) {
    if (GV->hasInitializer())
      return false;
  }
  return true;
}

Expected<ObjCCategoryList> BitcodeMetadata::categories() const {
  ObjCCategoryList CategoryList;
  for (auto &GV : OwningModule->globals()) {
    if (GV.hasSection()) {
      StringRef Section = GV.getSection();
      std::string SectionName;
      // Trim all spaces in the section name.
      for (auto c : Section) {
        if (c != ' ')
          SectionName.push_back(c);
      }
      if (SectionName == "__DATA,__objc_catlist,regular,no_dead_strip") {
        auto structList = dyn_cast<Constant>(
            GetUnderlyingObject(GV.getInitializer(), getDataLayout(), 2));
        for (const auto &op : structList->operands())
          CategoryList.push_back(ObjCCategoryRef(this, cast<Constant>(&op)));
      }
    }
  }
  return CategoryList;
}

Expected<ObjCCategory>
BitcodeMetadata::getObjCCategoryFromRef(ObjCCategoryRef Ref) const {
  auto GV = Ref.getRawContent().getLLVMConstant();
  int64_t cat_offset = 0;
  auto objc_category = dyn_cast<GlobalVariable>(
      GetPointerBaseWithConstantOffset(GV, cat_offset, getDataLayout()));
  if (!objc_category)
    return make_error<StringError>("Unable to find category associsated",
                                   object_error::parse_failed);

  return ObjCCategory(this, {objc_category, cat_offset});
}

Expected<ObjCProtocolList> BitcodeMetadata::protocols() const {
  ObjCProtocolList ProtocolList;
  for (auto &GV : OwningModule->globals()) {
    if (GV.hasSection()) {
      StringRef Section = GV.getSection();
      std::string SectionName;
      // Trim all spaces in the section name.
      for (auto c : Section) {
        if (c != ' ')
          SectionName.push_back(c);
      }
      if (SectionName == "__DATA,__objc_protolist,coalesced,no_dead_strip") {
        ProtocolList.push_back(ObjCProtocolRef(this, &GV));
      }
    }
  }
  return ProtocolList;
}

Expected<ObjCProtocol>
BitcodeMetadata::getObjCProtocolFromRef(ObjCProtocolRef Ref) const {
  auto GV = cast<GlobalVariable>(Ref.getRawContent().getLLVMConstant());
  auto protocol = dyn_cast<GlobalVariable>(
      GetUnderlyingObject(GV->getInitializer(), getDataLayout(), 2));
  if (!protocol)
    return make_error<StringError>("Unable to find protocol associsated",
                                   object_error::parse_failed);

  return ObjCProtocol(this, protocol);
}

Expected<StringRef>
BitcodeMetadata::getObjCClassName(ObjCClass ClassRef) const {
  if (auto ro = getObjC2ClassRO(ClassRef)) {
    unsigned offset =
        is64Bit() ? offsetof(class_ro64_t, name) : offsetof(class_ro32_t, name);
    auto class_name_global = dyn_cast<GlobalVariable>(GetUnderlyingObject(
        GetValueAtStructOffset(*ro, offset, getDataLayout()), getDataLayout(),
        2));
    if (!class_name_global)
      return make_error<StringError>("Could not read ObjC class name",
                                     object_error::parse_failed);
    ConstantDataSequential *CD =
        dyn_cast<ConstantDataSequential>(class_name_global->getInitializer());
    if (!CD)
      return make_error<StringError>(
          "Class name must be a ConstantDataSequential",
          object_error::parse_failed);
    return CD->getAsString().rtrim('\0');
  } else
    return ro.takeError();
}

Expected<StringRef>
BitcodeMetadata::getObjCSuperClassName(ObjCClass Data) const {
  auto objc_class =
      cast<GlobalVariable>(Data.getRawContent().getLLVMConstant());
  auto objc_class_t = cast<ConstantStruct>(
      GlobalOpToUser(GetUnderlyingValue(objc_class, getDataLayout(), 2)));
  int64_t class_offset = Data.getRawContent().getOffset();
  // read class_ro
  unsigned offset = is64Bit() ? offsetof(class64_t, superclass)
                              : offsetof(class32_t, superclass);
  auto superclass_t = GetValueAtStructOffset(
      objc_class_t, class_offset + offset, getDataLayout());
  if (!isa<ConstantPointerNull>(superclass_t)) {
    if (auto superclass_global = dyn_cast<GlobalVariable>(superclass_t)) {
      auto superclassref = ObjCClassRef(this, superclass_global);
      return superclassref.getClassName();
    } else
      return make_error<StringError>("Cannot process superclass",
                                     object_error::parse_failed);
  }
  return StringRef();
}

Expected<bool> BitcodeMetadata::isObjCClassSwift(ObjCClass Data) const {
  ConstantStruct *objc_class_t = nullptr;
  if (auto objc_class =
          dyn_cast<GlobalVariable>(Data.getRawContent().getLLVMConstant())) {
    objc_class_t = cast<ConstantStruct>(
        GlobalOpToUser(GetUnderlyingValue(objc_class, getDataLayout(), 2)));
  } else if (auto objc_class = dyn_cast<ConstantStruct>(
                 Data.getRawContent().getLLVMConstant()))
    objc_class_t = objc_class;
  else
    return make_error<StringError>("Unexpected type for objc class",
                                   object_error::parse_failed);

  int64_t class_offset = Data.getRawContent().getOffset();
  // read class_ro
  unsigned offset =
      is64Bit() ? offsetof(class64_t, data) : offsetof(class32_t, data);
  auto class_ro_v = GetValueAtStructOffset(objc_class_t, class_offset + offset,
                                           getDataLayout());
  if (isa<ConstantExpr>(class_ro_v)) {
    // swift generates class_ro with an offset using add constant expr
    return true;
  }
  return false;
}

Expected<bool> BitcodeMetadata::isObjCClassMetaClass(ObjCClass Data) const {
  if (auto ro = getObjC2ClassRO(Data)) {
    unsigned offset = is64Bit() ? offsetof(class_ro64_t, flags)
                                : offsetof(class_ro32_t, flags);
    auto ClassFlag = dyn_cast<ConstantInt>(
        GetUnderlyingValue(GetValueAtStructOffset(*ro, offset, getDataLayout()),
                           getDataLayout(), 2));
    if (!ClassFlag)
      return make_error<StringError>("Could not get flags from objc class",
                                     object_error::parse_failed);
    return ((ClassFlag->getZExtValue() & RO_META) != 0);
  } else
    return ro.takeError();
}

Expected<ObjCPropertyList>
BitcodeMetadata::materializePropertyList(ObjCClass ClassRef) const {
  ObjCPropertyList PropertyList;
  if (auto ro = getObjC2ClassRO(ClassRef)) {
    unsigned offset = is64Bit() ? offsetof(class_ro64_t, baseProperties)
                                : offsetof(class_ro32_t, baseProperties);
    auto objc_prop_list = dyn_cast<GlobalVariable>(
        GetUnderlyingValue(GetValueAtStructOffset(*ro, offset, getDataLayout()),
                           getDataLayout(), 2));
    if (objc_prop_list && objc_prop_list->hasInitializer()) {
      auto property_list_t = GlobalOpToUser(objc_prop_list);
      auto property_list = cast<Constant>(property_list_t->getOperand(2));
      for (auto &op : property_list->operands())
        PropertyList.push_back(ObjCProperty(this, cast<Constant>(&op)));
    }
    return PropertyList;
  } else
    return ro.takeError();
}

Expected<StringRef> BitcodeMetadata::getPropertyName(ObjCProperty Data) const {
  auto NameVar = dyn_cast<GlobalVariable>(
      GetUnderlyingValue(Data.getRawContent().getLLVMConstant()->getOperand(0),
                         getDataLayout(), 2));
  if (!NameVar)
    return make_error<StringError>("Could not find property name",
                                   object_error::parse_failed);

  ConstantDataSequential *CD =
      dyn_cast<ConstantDataSequential>(NameVar->getInitializer());
  if (!CD)
    return make_error<StringError>("Could not parse property name",
                                   object_error::parse_failed);

  return CD->getAsString().rtrim('\0');
}

Expected<StringRef>
BitcodeMetadata::getPropertyAttribute(ObjCProperty Data) const {
  auto AttrVar = dyn_cast<GlobalVariable>(
      GetUnderlyingValue(Data.getRawContent().getLLVMConstant()->getOperand(1),
                         getDataLayout(), 2));
  if (!AttrVar)
    return make_error<StringError>("Could not find property attr",
                                   object_error::parse_failed);

  ConstantDataSequential *CD =
      dyn_cast<ConstantDataSequential>(AttrVar->getInitializer());
  if (!CD)
    return make_error<StringError>("Could not parse property attr",
                                   object_error::parse_failed);

  return CD->getAsString().rtrim('\0');
}

Expected<ObjCMethodList>
BitcodeMetadata::materializeInstanceMethodList(ObjCClass ClassRef) const {
  ObjCMethodList MethodList;
  if (auto ro = getObjC2ClassRO(ClassRef)) {
    unsigned offset = is64Bit() ? offsetof(class_ro64_t, baseMethods)
                                : offsetof(class_ro32_t, baseMethods);
    auto objc_method_list = dyn_cast<User>(
        GetUnderlyingValue(GetValueAtStructOffset(*ro, offset, getDataLayout()),
                           getDataLayout(), 2));
    if (objc_method_list->getNumOperands() != 0) {
      auto methList =
          cast<User>(GlobalOpToUser(objc_method_list)->getOperand(2));
      for (auto &op : methList->operands())
        MethodList.push_back(ObjCMethod(this, cast<Constant>(&op)));
    }
    return MethodList;
  } else
    return ro.takeError();
}

Expected<ObjCMethodList>
BitcodeMetadata::materializeClassMethodList(ObjCClass ClassRef) const {
  ConstantStruct *objc_class_t = nullptr;
  if (auto objc_class = dyn_cast<GlobalVariable>(
          ClassRef.getRawContent().getLLVMConstant())) {
    objc_class_t = cast<ConstantStruct>(
        GlobalOpToUser(GetUnderlyingValue(objc_class, getDataLayout(), 2)));
  } else
    return make_error<StringError>("Unexpected type for objc meta class",
                                   object_error::parse_failed);

  int64_t class_offset = ClassRef.getRawContent().getOffset();
  auto metaclass =
      GetValueAtStructOffset(objc_class_t, class_offset, getDataLayout());
  int64_t metaclass_offset = 0;
  auto metaclass_g = dyn_cast<GlobalVariable>(GetPointerBaseWithConstantOffset(
      metaclass, metaclass_offset, getDataLayout()));

  if (!metaclass_g)
    return make_error<StringError>("Unable to find meta class associsated",
                                   object_error::parse_failed);

  auto MetaClass = ObjCClass(this, {metaclass_g, metaclass_offset});
  return materializeInstanceMethodList(MetaClass);
}

Expected<StringRef>
BitcodeMetadata::getObjCCategoryName(ObjCCategory Cat) const {
  ConstantStruct *objc_cat_t = nullptr;
  if (auto objc_category =
          dyn_cast<GlobalVariable>(Cat.getRawContent().getLLVMConstant())) {
    objc_cat_t = cast<ConstantStruct>(
        GlobalOpToUser(GetUnderlyingValue(objc_category, getDataLayout(), 2)));
  } else
    return make_error<StringError>("Unexpected type for objc category",
                                   object_error::parse_failed);

  int64_t cat_offset = Cat.getRawContent().getOffset();
  unsigned offset =
      is64Bit() ? offsetof(category64_t, name) : offsetof(category32_t, name);
  auto catname = dyn_cast<GlobalVariable>(GetUnderlyingObject(
      GetValueAtStructOffset(objc_cat_t, cat_offset + offset, getDataLayout()),
      getDataLayout(), 2));
  if (!catname)
    return make_error<StringError>("Cannot extract catogery name",
                                   object_error::parse_failed);

  ConstantDataSequential *CD =
      dyn_cast<ConstantDataSequential>(catname->getInitializer());
  if (!CD)
    return make_error<StringError>(
        "Category name must be a ConstantDataSequential",
        object_error::parse_failed);
  return CD->getAsString().rtrim('\0');
}

Expected<StringRef>
BitcodeMetadata::getObjCCategoryBaseClassName(ObjCCategory Cat) const {
  ConstantStruct *objc_cat_t = nullptr;
  if (auto objc_category =
          dyn_cast<GlobalVariable>(Cat.getRawContent().getLLVMConstant())) {
    objc_cat_t = cast<ConstantStruct>(
        GlobalOpToUser(GetUnderlyingValue(objc_category, getDataLayout(), 2)));
  } else
    return make_error<StringError>("Unexpected type for objc category",
                                   object_error::parse_failed);

  int64_t cat_offset = Cat.getRawContent().getOffset();
  unsigned offset =
      is64Bit() ? offsetof(category64_t, cls) : offsetof(category32_t, cls);
  auto baseCls = dyn_cast<GlobalVariable>(GetUnderlyingObject(
      GetValueAtStructOffset(objc_cat_t, cat_offset + offset, getDataLayout()),
      getDataLayout(), 2));
  if (!baseCls)
    return make_error<StringError>("Cannot extract category base class",
                                   object_error::parse_failed);
  auto baseClsRef = ObjCClassRef(this, baseCls);
  return baseClsRef.getClassName();
}

Expected<ObjCPropertyList>
BitcodeMetadata::materializePropertyList(ObjCCategory Cat) const {
  ConstantStruct *objc_cat_t = nullptr;
  if (auto objc_category =
          dyn_cast<GlobalVariable>(Cat.getRawContent().getLLVMConstant())) {
    objc_cat_t = cast<ConstantStruct>(
        GlobalOpToUser(GetUnderlyingValue(objc_category, getDataLayout(), 2)));
  } else
    return make_error<StringError>("Unexpected type for objc category",
                                   object_error::parse_failed);

  ObjCPropertyList PropertyList;
  int64_t cat_offset = Cat.getRawContent().getOffset();
  unsigned offset = is64Bit() ? offsetof(category64_t, instanceProperties)
                              : offsetof(category32_t, instanceProperties);
  auto props = dyn_cast<GlobalVariable>(GetUnderlyingObject(
      GetValueAtStructOffset(objc_cat_t, cat_offset + offset, getDataLayout()),
      getDataLayout(), 2));

  if (props && props->hasInitializer()) {
    auto property_list_t = GlobalOpToUser(props);
    auto property_list = cast<Constant>(property_list_t->getOperand(2));
    for (auto &op : property_list->operands())
      PropertyList.push_back(ObjCProperty(this, cast<Constant>(&op)));
  }
  return PropertyList;
}

Expected<ObjCMethodList>
BitcodeMetadata::materializeInstanceMethodList(ObjCCategory Cat) const {
  ConstantStruct *objc_cat_t = nullptr;
  if (auto objc_category =
          dyn_cast<GlobalVariable>(Cat.getRawContent().getLLVMConstant())) {
    objc_cat_t = cast<ConstantStruct>(
        GlobalOpToUser(GetUnderlyingValue(objc_category, getDataLayout(), 2)));
  } else
    return make_error<StringError>("Unexpected type for objc category",
                                   object_error::parse_failed);

  ObjCMethodList MethodList;
  int64_t cat_offset = Cat.getRawContent().getOffset();
  unsigned offset = is64Bit() ? offsetof(category64_t, instanceMethods)
                              : offsetof(category32_t, instanceMethods);
  auto method_list_global = dyn_cast<User>(GetUnderlyingObject(
      GetValueAtStructOffset(objc_cat_t, cat_offset + offset, getDataLayout()),
      getDataLayout(), 2));
  if (method_list_global->getNumOperands() != 0) {
    auto methList =
        dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
    for (auto &op : methList->operands())
      MethodList.push_back(ObjCMethod(this, cast<Constant>(&op)));
  }
  return MethodList;
}

Expected<ObjCMethodList>
BitcodeMetadata::materializeClassMethodList(ObjCCategory Cat) const {
  ConstantStruct *objc_cat_t = nullptr;
  if (auto objc_category =
          dyn_cast<GlobalVariable>(Cat.getRawContent().getLLVMConstant())) {
    objc_cat_t = cast<ConstantStruct>(
        GlobalOpToUser(GetUnderlyingValue(objc_category, getDataLayout(), 2)));
  } else
    return make_error<StringError>("Unexpected type for objc category",
                                   object_error::parse_failed);

  ObjCMethodList MethodList;
  int64_t cat_offset = Cat.getRawContent().getOffset();
  unsigned offset = is64Bit() ? offsetof(category64_t, classMethods)
                              : offsetof(category32_t, classMethods);
  auto method_list_global = dyn_cast<User>(GetUnderlyingObject(
      GetValueAtStructOffset(objc_cat_t, cat_offset + offset, getDataLayout()),
      getDataLayout(), 2));
  if (method_list_global->getNumOperands() != 0) {
    auto methList =
        dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
    for (auto &op : methList->operands())
      MethodList.push_back(ObjCMethod(this, cast<Constant>(&op)));
  }
  return MethodList;
}

Expected<StringRef>
BitcodeMetadata::getObjCProtocolName(ObjCProtocol Data) const {
  auto *objc_proto_t = cast<ConstantStruct>(GlobalOpToUser(GetUnderlyingValue(
      Data.getRawContent().getLLVMConstant(), getDataLayout(), 2)));
  if (!objc_proto_t)
    return make_error<StringError>("Unexpected type for objc protocol",
                                   object_error::parse_failed);

  unsigned offset =
      is64Bit() ? offsetof(protocol64_t, name) : offsetof(protocol32_t, name);
  auto protoName = dyn_cast<GlobalVariable>(GetUnderlyingObject(
      GetValueAtStructOffset(objc_proto_t, offset, getDataLayout()),
      getDataLayout(), 2));
  if (!protoName)
    return make_error<StringError>("Cannot extract protocol name",
                                   object_error::parse_failed);

  ConstantDataSequential *CD =
      dyn_cast<ConstantDataSequential>(protoName->getInitializer());
  if (!CD)
    return make_error<StringError>(
        "Protocol name must be a ConstantDataSequential",
        object_error::parse_failed);
  return CD->getAsString().rtrim('\0');
}

Expected<ObjCPropertyList>
BitcodeMetadata::materializePropertyList(ObjCProtocol Data) const {
  auto *objc_proto_t = cast<ConstantStruct>(GlobalOpToUser(GetUnderlyingValue(
      Data.getRawContent().getLLVMConstant(), getDataLayout(), 2)));
  if (!objc_proto_t)
    return make_error<StringError>("Unexpected type for objc protocol",
                                   object_error::parse_failed);

  unsigned offset = is64Bit() ? offsetof(protocol64_t, instanceProperties)
                              : offsetof(protocol32_t, instanceProperties);

  ObjCPropertyList PropertyList;
  auto props = dyn_cast<GlobalVariable>(GetUnderlyingObject(
      GetValueAtStructOffset(objc_proto_t, offset, getDataLayout()),
      getDataLayout(), 2));

  if (props && props->hasInitializer()) {
    auto property_list_t = GlobalOpToUser(props);
    auto property_list = cast<Constant>(property_list_t->getOperand(2));
    for (auto &op : property_list->operands())
      PropertyList.push_back(ObjCProperty(this, cast<Constant>(&op)));
  }
  return PropertyList;
}

Expected<ObjCMethodList>
BitcodeMetadata::materializeInstanceMethodList(ObjCProtocol Data) const {
  auto *objc_proto_t = cast<ConstantStruct>(GlobalOpToUser(GetUnderlyingValue(
      Data.getRawContent().getLLVMConstant(), getDataLayout(), 2)));
  if (!objc_proto_t)
    return make_error<StringError>("Unexpected type for objc protocol",
                                   object_error::parse_failed);

  unsigned offset = is64Bit() ? offsetof(protocol64_t, instanceMethods)
                              : offsetof(protocol32_t, instanceMethods);

  ObjCMethodList MethodList;
  auto method_list_global = dyn_cast<User>(GetUnderlyingObject(
      GetValueAtStructOffset(objc_proto_t, offset, getDataLayout()),
      getDataLayout(), 2));
  if (method_list_global->getNumOperands() != 0) {
    auto methList =
        dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
    for (auto &op : methList->operands())
      MethodList.push_back(ObjCMethod(this, cast<Constant>(&op)));
  }
  return MethodList;
}

Expected<ObjCMethodList>
BitcodeMetadata::materializeClassMethodList(ObjCProtocol Data) const {
  auto *objc_proto_t = cast<ConstantStruct>(GlobalOpToUser(GetUnderlyingValue(
      Data.getRawContent().getLLVMConstant(), getDataLayout(), 2)));
  if (!objc_proto_t)
    return make_error<StringError>("Unexpected type for objc protocol",
                                   object_error::parse_failed);

  unsigned offset = is64Bit() ? offsetof(protocol64_t, classMethods)
                              : offsetof(protocol32_t, classMethods);

  ObjCMethodList MethodList;
  auto method_list_global = dyn_cast<User>(GetUnderlyingObject(
      GetValueAtStructOffset(objc_proto_t, offset, getDataLayout()),
      getDataLayout(), 2));
  if (method_list_global->getNumOperands() != 0) {
    auto methList =
        dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
    for (auto &op : methList->operands())
      MethodList.push_back(ObjCMethod(this, cast<Constant>(&op)));
  }
  return MethodList;
}

Expected<ObjCMethodList> BitcodeMetadata::materializeOptionalInstanceMethodList(
    ObjCProtocol Data) const {
  auto *objc_proto_t = cast<ConstantStruct>(GlobalOpToUser(GetUnderlyingValue(
      Data.getRawContent().getLLVMConstant(), getDataLayout(), 2)));
  if (!objc_proto_t)
    return make_error<StringError>("Unexpected type for objc protocol",
                                   object_error::parse_failed);

  unsigned offset = is64Bit() ? offsetof(protocol64_t, optionalInstanceMethods)
                              : offsetof(protocol32_t, optionalInstanceMethods);

  ObjCMethodList MethodList;
  auto method_list_global = dyn_cast<User>(GetUnderlyingObject(
      GetValueAtStructOffset(objc_proto_t, offset, getDataLayout()),
      getDataLayout(), 2));
  if (method_list_global->getNumOperands() != 0) {
    auto methList =
        dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
    for (auto &op : methList->operands())
      MethodList.push_back(ObjCMethod(this, cast<Constant>(&op)));
  }
  return MethodList;
}

Expected<ObjCMethodList>
BitcodeMetadata::materializeOptionalClassMethodList(ObjCProtocol Data) const {
  auto *objc_proto_t = cast<ConstantStruct>(GlobalOpToUser(GetUnderlyingValue(
      Data.getRawContent().getLLVMConstant(), getDataLayout(), 2)));
  if (!objc_proto_t)
    return make_error<StringError>("Unexpected type for objc protocol",
                                   object_error::parse_failed);

  unsigned offset = is64Bit() ? offsetof(protocol64_t, optionalClassMethods)
                              : offsetof(protocol32_t, optionalClassMethods);

  ObjCMethodList MethodList;
  auto method_list_global = dyn_cast<User>(GetUnderlyingObject(
      GetValueAtStructOffset(objc_proto_t, offset, getDataLayout()),
      getDataLayout(), 2));
  if (method_list_global->getNumOperands() != 0) {
    auto methList =
        dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
    for (auto &op : methList->operands())
      MethodList.push_back(ObjCMethod(this, cast<Constant>(&op)));
  }
  return MethodList;
}

Expected<StringRef> BitcodeMetadata::getMethodName(ObjCMethod Data) const {
  auto objcMeth = Data.getRawContent().getLLVMConstant();
  auto NameVar = dyn_cast<GlobalVariable>(
      GetUnderlyingObject(objcMeth->getOperand(0), getDataLayout(), 2));
  if (!NameVar)
    return make_error<StringError>("Could not find property name",
                                   object_error::parse_failed);

  ConstantDataSequential *CD =
      dyn_cast<ConstantDataSequential>(NameVar->getInitializer());
  if (!CD)
    return make_error<StringError>("Could not parse property name",
                                   object_error::parse_failed);
  return CD->getAsString().rtrim('\0');
}

Expected<StringRef> BitcodeMetadata::getMethodType(ObjCMethod Data) const {
  auto objcMeth = Data.getRawContent().getLLVMConstant();
  auto TypeVar = dyn_cast<GlobalVariable>(
      GetUnderlyingObject(objcMeth->getOperand(1), getDataLayout(), 2));
  if (!TypeVar)
    return make_error<StringError>("Could not find property name",
                                   object_error::parse_failed);

  ConstantDataSequential *CD =
      dyn_cast<ConstantDataSequential>(TypeVar->getInitializer());
  if (!CD)
    return make_error<StringError>("Could not parse property name",
                                   object_error::parse_failed);
  return CD->getAsString().rtrim('\0');
}

Expected<ObjCSelectorList> BitcodeMetadata::referencedSelectors() const {
  ObjCSelectorList SelectorList;
  for (auto &GV : OwningModule->globals()) {
    if (GV.hasSection()) {
      StringRef Section = GV.getSection();
      std::string SectionName;
      // Trim all spaces in the section name.
      for (auto c : Section) {
        if (c != ' ')
          SectionName.push_back(c);
      }
      if (SectionName ==
          "__DATA,__objc_selrefs,literal_pointers,no_dead_strip") {
        SelectorList.push_back(ObjCSelectorRef(this, &GV));
      }
    }
  }
  return SelectorList;
}

Expected<StringRef>
BitcodeMetadata::getObjCSelectorName(ObjCSelectorRef Ref) const {
  auto GV = dyn_cast<GlobalVariable>(Ref.getRawContent().getLLVMConstant());
  auto sel_global =
      GetUnderlyingObject(GV->getInitializer(), getDataLayout(), 2);
  auto CD = dyn_cast<ConstantDataSequential>(GlobalOpToUser(sel_global));
  if (!CD)
    return make_error<StringError>("could not read selref",
                                   object_error::parse_failed);
  return CD->getAsString().rtrim('\0');
}

void BitcodeMetadata::getAllPotentiallyDefinedSelectors(
    StringSet<> &Set) const {
  auto ignoreError = [](Error &&E) {
    handleAllErrors(std::move(E), [&](const ErrorInfoBase &EI) {
      return; // ignore.
    });
  };
  if (auto selectors = referencedSelectors()) {
    for (auto &sel : *selectors) {
      auto GV = dyn_cast<GlobalVariable>(sel.getRawContent().getLLVMConstant());
      auto sel_global =
          GetUnderlyingObject(GV->getInitializer(), getDataLayout(), 2);
      if (hasUserInMethodList(sel_global, OwningModule)) {
        if (auto name = sel.getSelector())
          Set.insert(*name);
        else
          ignoreError(name.takeError());
      }
    }
  } else
    ignoreError(selectors.takeError());
}

bool BitcodeMetadata::isObjC1() const { return !isObjC2(); }

bool BitcodeMetadata::isObjC2() const {
  auto versionMD = OwningModule->getModuleFlag("Objective-C Version");
  if (versionMD) {
    auto objcVer =
        cast<ConstantInt>(cast<ConstantAsMetadata>(versionMD)->getValue())
            ->getZExtValue();
    if (objcVer == 2) {
      return true;
    }
  }
  return false;
}

Expected<ConstantStruct *>
BitcodeMetadata::getObjC2ClassRO(ObjCClass Data) const {
  ConstantStruct *objc_class_t = nullptr;
  if (auto objc_class =
          dyn_cast<GlobalVariable>(Data.getRawContent().getLLVMConstant())) {
    objc_class_t = cast<ConstantStruct>(
        GlobalOpToUser(GetUnderlyingValue(objc_class, getDataLayout(), 2)));
  } else
    return make_error<StringError>("Unexpected type for objc class",
                                   object_error::parse_failed);

  int64_t class_offset = Data.getRawContent().getOffset();
  // read class_ro
  unsigned offset =
      is64Bit() ? offsetof(class64_t, data) : offsetof(class32_t, data);
  auto class_ro_v = GetValueAtStructOffset(objc_class_t, class_offset + offset,
                                           getDataLayout());
  if (auto CE = dyn_cast<ConstantExpr>(class_ro_v)) {
    // swift generates class_ro with an offset using add constant expr
    class_ro_v = CE->getOperand(0);
  }
  auto class_ro_global = dyn_cast<GlobalVariable>(
      GetUnderlyingValue(class_ro_v, getDataLayout(), 2));
  if (!class_ro_global)
    return make_error<StringError>("Could not read ObjCClass RO data",
                                   object_error::parse_failed);
  auto class_ro = dyn_cast<ConstantStruct>(GlobalOpToUser(
      GetUnderlyingValue(class_ro_global, getDataLayout(), 4)));
  if (!class_ro)
    return make_error<StringError>("Could not process ObjCClass RO data",
                                   object_error::parse_failed);
  return class_ro;
}

StringRef BitcodeMetadata::getSymbolNameFromRef(ObjCRef Ref) const {
  return Ref.getLLVMConstant()->getName();
}

StringRef BitcodeMetadata::guessClassNameBasedOnSymbol(StringRef Sym) const {
  if (Sym.startswith("OBJC_CLASS_$_"))
    return Sym.drop_front(strlen("OBJC_CLASS_$_"));
  else if (Sym.startswith("\01.objc_class_name_"))
    return Sym.drop_front(strlen("\01.objc_class_name_"));
  else
    return Sym;
}
