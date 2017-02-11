//===- APIUsageAnalysis.cpp - API Usage Analysis --------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the API usage analysis
//
//===----------------------------------------------------------------------===//

#include "ModuleAnalyzer.h"

#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/InstructionSimplify.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InlineAsm.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Operator.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectFileInfo.h"
#include "llvm/MC/MCParser/MCAsmParser.h"
#include "llvm/MC/MCParser/MCTargetAsmParser.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetRegistry.h"

#include <vector>
using namespace llvm;

//===----------------------------------------------------------------------===//
//  APIUsageAnalysis Implementation
//===----------------------------------------------------------------------===//

int AnalyzeModuleImpl(Module *M, APIAnalysisIntermediateResult &result,
                      const APIAnalysisOptions &options);

int AnalyzeModule(StringRef filePath, APIAnalysisIntermediateResult &result,
                  const APIAnalysisOptions &options) {
  std::unique_ptr<LLVMContext> Context(new LLVMContext());

  SMDiagnostic Err;
  std::unique_ptr<Module> M = parseIRFile(filePath, Err, *Context);

  if (!M) {
    errs() << "Could not load module: " << Err.getMessage() << "\n";
    return 1;
  }

  return AnalyzeModuleImpl(M.get(), result, options);
}

int AnalyzeModule(llvm::MemoryBufferRef data,
                  APIAnalysisIntermediateResult &result,
                  const APIAnalysisOptions &options) {
  std::unique_ptr<LLVMContext> Context(new LLVMContext());

  SMDiagnostic Err;
  std::unique_ptr<Module> M = parseIR(data, Err, *Context);

  if (!M) {
    errs() << "Could not load module: " << Err.getMessage() << "\n";
    return 1;
  }

  return AnalyzeModuleImpl(M.get(), result, options);
}

template <class ContainerType>
void InsertUnique(ContainerType &container, StringRef key) {
  auto I = container.find(key);
  if (container.end() == I)
    container.insert(std::make_pair(key, false));
}

template <> void InsertUnique(StringSet<> &container, StringRef key) {
  auto I = container.find(key);
  if (container.end() == I)
    container.insert(key);
}

void UpdateStringMapWithValue(StringMap<bool> &container, StringRef key,
                              bool value) {
  auto I = container.find(key);
  if (container.end() == I)
    container.insert(std::make_pair(key, value));
  else
    I->second = I->second || value;
}

void UpdateObjCClassCollection(StringRef SymName, bool mangled,
                               ObjCClassCollection &collection) {
  if (mangled) {
    if (SymName.startswith("_OBJC_CLASS_$_")) {
      auto classname = SymName.drop_front(strlen("_OBJC_CLASS_$_"));
      collection.insert({classname, ObjCClass(classname)});
    } else if (SymName.startswith(".objc_class_name_")) {
      auto classname = SymName.drop_front(strlen(".objc_class_name_"));
      collection.insert({classname, ObjCClass(classname)});
    }
  } else {
    if (SymName.startswith("OBJC_CLASS_$_")) {
      auto classname = SymName.drop_front(strlen("OBJC_CLASS_$_"));
      collection.insert({classname, ObjCClass(classname)});
    } else if (SymName.startswith("\01.objc_class_name_")) {
      auto classname = SymName.drop_front(strlen("\01.objc_class_name_"));
      collection.insert({classname, ObjCClass(classname)});
    }
  }
}

StringRef TrimNull(StringRef str) {
  if (str.endswith(StringRef("\0")))
    return str.drop_back(1);
  return str;
}

User *GlobalOpToUser(Value *v) {
  auto base_user = dyn_cast<User>(v);
  assert(base_user && "Provided value is not a user!");
  auto op0 = dyn_cast<User>(base_user->getOperand(0));
  assert(op0 && "User doesn't have User operands!");
  return op0;
}

Value *GetValueAtStructOffset(ConstantStruct *CS, uint64_t offset,
                              const DataLayout &DL) {
  StructType* ST = CS->getType();
  unsigned index = DL.getStructLayout(ST)->getElementContainingOffset(offset);
  assert(DL.getStructLayout(ST)->getElementOffset(index) == offset &&
         "offset must point to the beginning of an element");
  return CS->getOperand(index);
}

GlobalAlias *GetAliasOfImpl(Value* GV, int max_lvl, int current_lvl) {
  // Find alias of GlobalValue using depth first search.
  // Return nullptr if not found.
  if (current_lvl > max_lvl)
    return nullptr;
  ++ current_lvl;

  for (const auto &U : GV->users()) {
    if (auto alias = dyn_cast<GlobalAlias>(U))
      return alias;
    else if (auto alias = GetAliasOfImpl(U, max_lvl, current_lvl))
      return alias;
  }

  return nullptr;
}

GlobalAlias *GetAliasOf(Value *GV, int level = 3) {
  return GetAliasOfImpl(GV, level, 0);
}

bool isDefinedGlobal(const GlobalValue *GV) {
  if (GV->isDeclaration())
    return false;

  // Handle available externally linkage, assume no inliner will run again.
  // If there are uses for the function, it is undefined.
  if (GV->hasAvailableExternallyLinkage() && GV->getNumUses() > 0)
    return false;

  return true;
}

bool ShouldRecordGlobal(const GlobalValue *GV) {
  if (!GV->hasName())
    return false;

  StringRef name = GV->getName();
  if (name.startswith("llvm.") || name.find("__hidden#") != StringRef::npos)
    return false;

  if (auto *F = dyn_cast<Function>(GV))
    if (F->isIntrinsic())
      return false;

  // Skip private and internal linkage which are local to the Translation Unit.
  if (isDefinedGlobal(GV) &&
      (GV->hasInternalLinkage() || GV->hasPrivateLinkage()))
    return false;

  // Handle available externally linkage, assume no inliner will run again.
  // If there are no uses for the function, it is not emitted.
  if (GV->hasAvailableExternallyLinkage() && GV->getNumUses() == 0)
    return false;

  return true;
}

bool isLinkageUnitLocalGlobal(const GlobalValue *GV) {
  // Defined globals with hidden visibility are local to the LinkageUnit.
  if (isDefinedGlobal(GV) && GV->hasHiddenVisibility())
    return true;

  return false;
}

// This is basically the GetUnderlyingObject in ValueTracking Analysis,
// except it works through non-pointer types.
Value *GetUnderlyingValue(Value *V, const DataLayout &DL, unsigned MaxLookup) {
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
bool hasUserInMethodList(const Value *V, Module *M, unsigned maxDepth = 4) {
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

template <typename MapType>
void HandleObjCMethods(Module *M, User *objc_meth_list,
                       MapType &methods) {
  for (auto &op : objc_meth_list->operands()) {
    User *objc_meth = dyn_cast<User>(op.get());
    if (!objc_meth)
      continue;
    auto name =
        GetUnderlyingObject(objc_meth->getOperand(0), M->getDataLayout(), 2);
    auto func =
        GetUnderlyingObject(objc_meth->getOperand(2), M->getDataLayout(), 2);
    GlobalVariable *nameVar = dyn_cast<GlobalVariable>(name);
    if (!nameVar)
      continue;
    ConstantDataSequential *CD =
        dyn_cast<ConstantDataSequential>(nameVar->getInitializer());
    if (!CD)
      continue;
    auto methName = TrimNull(CD->getAsString());
    auto methodIt = methods.find(methName);
    if (methodIt == methods.end()) {
      methods.insert(std::make_pair(methName, false));
      methodIt = methods.find(methName);
    }
    methodIt->second = methodIt->second || !isa<ConstantPointerNull>(func);
  }
}

template <typename MapType>
void HandleObjCPropertyList(Module *M, GlobalVariable *objc_prop_list,
                            MapType &methods) {
  if (objc_prop_list && objc_prop_list->hasInitializer()) {
    auto property_list_t = GlobalOpToUser(objc_prop_list);
    auto property_list = cast<Constant>(property_list_t->getOperand(2));
    for (auto &prop : property_list->operands()) {
      auto name = GetUnderlyingValue(cast<User>(prop)->getOperand(0),
                                     M->getDataLayout(), 2);
      auto nameVar = dyn_cast<GlobalVariable>(name);
      if (!nameVar) {
        continue;
      }
      ConstantDataSequential *CD =
          dyn_cast<ConstantDataSequential>(nameVar->getInitializer());
      if (!CD) {
        continue;
      }
      // setter and getter count as a defined instance method
      auto getter = TrimNull(CD->getAsString());
      std::string setter =
          "set" + getter.substr(0, 1).upper() + getter.substr(1).str() + ":";
      methods.insert({getter, true});
      methods.insert({setter, true});
    }
  }
}

// Handle ObjC Class, return true on failure.
// struct _class_t {                          // offsets
//   struct _class_t *isa;                    // 0/0 byte
//   struct _class_t * const superclass;      // 4/8 byte
//   void *cache;                             // 8/16 byte
//   IMP *vtable;                             // 12/24 byte
//   struct class_ro_t *ro;                   // 16/32 byte
// }
ObjCClass *HandleObjCClass(Module *M, Value *objc_class_ptr,
                           llvm::StringMap<std::string> &class_map,
                           ObjCClassCollection &collection) {
  int64_t class_offset = 0;
  auto objc_class = dyn_cast<GlobalVariable>(GetPointerBaseWithConstantOffset(
      objc_class_ptr, class_offset, M->getDataLayout()));
  if (!objc_class) {
      errs() << "objc_class is not global variable\n";
      return nullptr;
  }

  if (!objc_class->hasInitializer()) {
    // External class
    StringRef className;
    auto symbol_name = objc_class->getName();
    auto sep = symbol_name.find("$");
    // Swift class don't follow objc class variable naming conventions.
    if (sep == StringRef::npos)
      className = symbol_name;
    else
      className = symbol_name.substr(sep + 2);

    return &collection.insert({className, ObjCClass(className)}).first->second;
  }

  auto objc_class_t = cast<ConstantStruct>(
      GlobalOpToUser(GetUnderlyingValue(objc_class, M->getDataLayout(), 2)));

  // read class_ro
  Triple TT = Triple(M->getTargetTriple());
  unsigned offset = TT.isArch64Bit() ? 32 : 16;
  auto class_ro_v = GetValueAtStructOffset(objc_class_t, class_offset + offset,
                                           M->getDataLayout());
  if (auto CE = dyn_cast<ConstantExpr>(class_ro_v)) {
    // swift generates class_ro with an offset using add constant expr
    class_ro_v = CE->getOperand(0);
  }
  auto class_ro_global = dyn_cast<GlobalVariable>(
      GetUnderlyingValue(class_ro_v, M->getDataLayout(), 2));
  if (!class_ro_global) {
    // class_ro is NULL.
    errs() << "Could not read ObjC class ro\n";
    return nullptr;
  }
  auto class_ro = dyn_cast<ConstantStruct>(GlobalOpToUser(
      GetUnderlyingValue(class_ro_global, M->getDataLayout(), 4)));
  if (!class_ro) {
    errs() << "Could not process ObjC class ro\n";
    return nullptr;
  }

  // Extract class_ro
  // struct _class_ro_t {                                       // offsets
  //   uint32_t const flags;                                    // 0/0 byte
  //   uint32_t const instanceStart;                            // 4/4 byte
  //   uint32_t const instanceSize;                             // 8/8 byte
  //   uint32_t const reserved;                                 // -/12 byte
  //   const uint8_t * const ivarLayout;                        // 12/16 byte
  //   const char *const name;                                  // 16/24 byte
  //   const struct _method_list_t * const baseMethods;         // 20/32 byte
  //   const struct _objc_protocol_list *const baseProtocols;   // 24/40 byte
  //   const struct _ivar_list_t *const ivars;                  // 28/48 byte
  //   const uint8_t * const weakIvarLayout;                    // 32/56 byte
  //   const struct _prop_list_t * const properties;            // 36/64 byte
  // }

  // read the class name
  offset = TT.isArch64Bit() ? 24 : 16;
  auto class_name_global = dyn_cast<GlobalVariable>(GetUnderlyingObject(
      GetValueAtStructOffset(class_ro, offset, M->getDataLayout()),
      M->getDataLayout(), 2));
  if (!class_name_global) {
    errs() << "Could not read ObjC class name\n";
    return nullptr;
  }
  ConstantDataSequential *CD =
      dyn_cast<ConstantDataSequential>(class_name_global->getInitializer());
  if (!CD) {
    errs() << "Class Name must be a ConstantDataSequential\n";
    return nullptr;
  }
  StringRef className = TrimNull(CD->getAsString());
  // Lookup the collection, if the class is processed, just return the value
  // in the collection.
  ObjCClass *classInfo =
      &collection.insert({className, ObjCClass(className)}).first->second;

  // Insert name of the global to the map for lookup.
  class_map.insert({objc_class->getName(), className.str()});
  // If there is alias for the it, it should resolve to the same name.
  if (auto alias = GetAliasOf(objc_class)) {
    class_map.insert({alias->getName(), className.str()});
    // Look for alias for alias.
    auto *ClassAlias = GetAliasOf(alias);
    while (ClassAlias != nullptr) {
      class_map.insert({ClassAlias->getName(), className.str()});
      ClassAlias = GetAliasOf(ClassAlias);
    }
  }

  // read super class
  offset = TT.isArch64Bit() ? 8 : 4;
  auto superclass_t = GetValueAtStructOffset(
      objc_class_t, class_offset + offset, M->getDataLayout());
  if (!isa<ConstantPointerNull>(superclass_t)) {
    // super class can be null.
    auto superclassInfo =
        HandleObjCClass(M, superclass_t, class_map, collection);
    if (superclassInfo)
      classInfo->superClass = superclassInfo;
    else
      errs() << "Could not read ObjC superclass\n";
  }
  
  // read instance methods
  offset = TT.isArch64Bit() ? 32 : 20;
  auto method_list_global = dyn_cast<User>(GetUnderlyingValue(
      GetValueAtStructOffset(class_ro, offset, M->getDataLayout()),
      M->getDataLayout(), 2));
  if (method_list_global->getNumOperands() != 0) {
    auto objc_meth_list =
        dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
    HandleObjCMethods(M, objc_meth_list, classInfo->instanceMethods);
  }

  // read class methods
  auto metaclass_t_global = cast<GlobalVariable>(GetUnderlyingValue(
      GetValueAtStructOffset(objc_class_t, class_offset, M->getDataLayout()),
      M->getDataLayout(), 2));
  if (metaclass_t_global->hasInitializer()) {
    unsigned ro_offset = TT.isArch64Bit() ? 32 : 16;
    auto metaclass_t =
        dyn_cast<ConstantStruct>(GlobalOpToUser(metaclass_t_global));
    auto metaclass_ro =
        dyn_cast<ConstantStruct>(GlobalOpToUser(GetUnderlyingValue(
            GetValueAtStructOffset(metaclass_t, ro_offset, M->getDataLayout()),
            M->getDataLayout(), 2)));
    if (metaclass_ro) {
      auto method_list_global = dyn_cast<User>(GetUnderlyingValue(
          GetValueAtStructOffset(metaclass_ro, offset, M->getDataLayout()),
          M->getDataLayout(), 4));
      if (method_list_global->getNumOperands() != 0) {
        auto objc_meth_list =
            dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
        HandleObjCMethods(M, objc_meth_list, classInfo->classMethods);
      }
    }
  }

  // read properties
  offset = TT.isArch64Bit() ? 64 : 36;
  auto property_list_global = dyn_cast<GlobalVariable>(GetUnderlyingValue(
      GetValueAtStructOffset(class_ro, offset, M->getDataLayout()),
      M->getDataLayout(), 2));
  HandleObjCPropertyList(M, property_list_global, classInfo->instanceMethods);

  classInfo->isDefined = true;
  return classInfo;
}

class InlineAsmSymbolStreamer : public MCStreamer {
  APIAnalysisIntermediateResult &result;

  void visitUsedSymbol(const MCSymbol &Sym) override {
    InsertUnique(result.asmSymbols, Sym.getName());
  }

public:
  InlineAsmSymbolStreamer(MCContext &Context,
                          APIAnalysisIntermediateResult &res)
      : MCStreamer(Context), result(res) {}

  // required methods I don't care about.
  bool EmitSymbolAttribute(MCSymbol *, MCSymbolAttr) override { return true; }
  void EmitZerofill(MCSection *, MCSymbol *, uint64_t, unsigned) override {}
  void EmitCommonSymbol(MCSymbol *, uint64_t, unsigned) override {}
};

class ModuleAsmSymbolStreamer : public MCStreamer {
  APIAnalysisIntermediateResult &result;
  ObjCClassCollection &ObjCClasses;

  void visitUsedSymbol(const MCSymbol &Sym) override {
    // Assume symbols are all functions.
    if (Sym.isUndefined()) {
      auto I = result.functionNames.find(Sym.getName());
      if (result.functionNames.end() == I)
        result.functionNames.insert(std::make_pair(Sym.getName(), false));
      UpdateObjCClassCollection(Sym.getName(), true, ObjCClasses);
    } else
      InsertUnique(result.functionNames, Sym.getName());
  }

public:
  ModuleAsmSymbolStreamer(MCContext &Context,
                          APIAnalysisIntermediateResult &res,
                          ObjCClassCollection &col)
      : MCStreamer(Context), result(res), ObjCClasses(col) {}

  // required methods I don't care about.
  bool EmitSymbolAttribute(MCSymbol *, MCSymbolAttr) override { return true; }
  void EmitZerofill(MCSection *, MCSymbol *, uint64_t, unsigned) override {}
  void EmitCommonSymbol(MCSymbol *, uint64_t, unsigned) override {}
};

void HandleInlineAsm(Module *M, APIAnalysisIntermediateResult &result,
                     ObjCClassCollection &collection) {
  const DataLayout &DL = M->getDataLayout();

  StringRef TT(M->getTargetTriple());
  std::string Err;
  const Target *T = TargetRegistry::lookupTarget(TT.str(), Err);
  if (!T)
    return;

  std::unique_ptr<MCRegisterInfo> MRI(T->createMCRegInfo(TT.str()));
  if (!MRI)
    return;

  std::unique_ptr<MCAsmInfo> MAI(T->createMCAsmInfo(*MRI, TT.str()));
  if (!MAI)
    return;

  std::unique_ptr<MCSubtargetInfo> STI(
      T->createMCSubtargetInfo(TT.str(), "", ""));
  if (!STI)
    return;

  std::unique_ptr<MCInstrInfo> MCII(T->createMCInstrInfo());
  if (!MCII)
    return;

  MCObjectFileInfo MOFI;
  MCContext MCCtx(MAI.get(), MRI.get(), &MOFI);
  MOFI.InitMCObjectFileInfo(Triple(TT), Reloc::Default, CodeModel::Default,
                            MCCtx);
  std::unique_ptr<MCStreamer> Streamer(
      new InlineAsmSymbolStreamer(MCCtx, result));
  T->createNullTargetStreamer(*Streamer);

  for (auto &F : *M) {
    for (auto &bb : F) {
      for (auto &i : bb) {
        auto callInst = dyn_cast<CallInst>(&i);
        if (!callInst || !callInst->isInlineAsm())
          continue;
        auto str = GetUnderlyingObject(callInst->getOperand(0), DL, 2);
        auto *asmInst = dyn_cast<InlineAsm>(str);
        if (!asmInst)
          continue;
        // ignore marker for ObjCARC optimization.
        if (StringRef(asmInst->getAsmString())
                .endswith("marker for objc_retainAutoreleaseReturnValue"))
          continue;
        std::unique_ptr<MemoryBuffer> Buffer(
            MemoryBuffer::getMemBuffer(asmInst->getAsmString()));
        SourceMgr SrcMgr;
        SrcMgr.AddNewSourceBuffer(std::move(Buffer), SMLoc());
        std::unique_ptr<MCAsmParser> Parser(
            createMCAsmParser(SrcMgr, MCCtx, *Streamer, *MAI));

        MCTargetOptions MCOptions;
        std::unique_ptr<MCTargetAsmParser> TAP(
            T->createMCAsmParser(*STI, *Parser, *MCII, MCOptions));
        if (!TAP)
          continue;

        Parser->setTargetParser(*TAP);
        if (Parser->Run(false))
          continue;
      }
    }
  }

  Streamer.reset(new ModuleAsmSymbolStreamer(MCCtx, result, collection));
  T->createNullTargetStreamer(*Streamer);
  const std::string &InlineAsm = M->getModuleInlineAsm();
  if (InlineAsm.empty())
    return;

  std::unique_ptr<MemoryBuffer> Buffer(MemoryBuffer::getMemBuffer(InlineAsm));
  SourceMgr SrcMgr;
  SrcMgr.AddNewSourceBuffer(std::move(Buffer), SMLoc());
  std::unique_ptr<MCAsmParser> Parser(
      createMCAsmParser(SrcMgr, MCCtx, *Streamer, *MAI));

  MCTargetOptions MCOptions;
  std::unique_ptr<MCTargetAsmParser> TAP(
      T->createMCAsmParser(*STI, *Parser, *MCII, MCOptions));
  if (!TAP)
    return;

  Parser->setTargetParser(*TAP);
  if (Parser->Run(false))
    return;
}

bool IsUsed(GlobalVariable &GV) {
  SmallVector<User *, 8> worklist;
  SmallPtrSet<User *, 8> visited;
  worklist.push_back(&GV);
  visited.insert(&GV);
  while (!worklist.empty()) {
    User *item = worklist.back();
    if (isa<Instruction>(item))
      return true;
    worklist.pop_back();
    for (User *user : item->users())
      if (visited.insert(user).second)
        worklist.push_back(user);
  }
  return false;
}

void TraverseObjCClassList(GlobalVariable &GV, Module *M,
                           APIAnalysisIntermediateResult &result,
                           ObjCClassCollection & collection) {
  auto classList = dyn_cast<Constant>(
      GetUnderlyingObject(GV.getInitializer(), M->getDataLayout(), 2));
  assert(classList && "__objc_classlist should be a Constant");
  for (const auto &op : classList->operands()) {
    HandleObjCClass(M, op, result.classLinkMap, collection);
  }
}

/// struct _protocol_t {
///   id isa;  // NULL
///   const char * const protocol_name;
///   const struct _protocol_list_t * protocol_list; // super protocols
///   const struct method_list_t * const instance_methods;
///   const struct method_list_t * const class_methods;
///   const struct method_list_t *optionalInstanceMethods;
///   const struct method_list_t *optionalClassMethods;
///   const struct _prop_list_t * properties;
///   const uint32_t size;  // sizeof(struct _protocol_t)
///   const uint32_t flags;  // = 0
///   const char ** extendedMethodTypes;
///   const char *demangledName;
/// }
void TraverseObjCProtoList(GlobalVariable &GV, Module *M,
                           APIAnalysisIntermediateResult &result) {
  auto protocol_t = GlobalOpToUser(
      GetUnderlyingObject(GV.getInitializer(), M->getDataLayout(), 2));

  // Read the protocol name.
  auto protocol_name_global = dyn_cast<GlobalVariable>(
      GetUnderlyingObject(protocol_t->getOperand(1), M->getDataLayout(), 2));
  assert(protocol_name_global && "Protocol Name must be a global");
  ConstantDataSequential *CD =
      dyn_cast<ConstantDataSequential>(protocol_name_global->getInitializer());
  assert(CD && "Protocol Name must be a ConstantDataSequential");
  StringRef protocolName = TrimNull(CD->getAsString());

  auto method_list_global = dyn_cast<User>(
      GetUnderlyingObject(protocol_t->getOperand(3), M->getDataLayout(), 2));
  if (method_list_global->getNumOperands() != 0) {
    auto objc_meth_list =
        dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
    HandleObjCMethods(M, objc_meth_list,
                      result.protocolInstanceMethods[protocolName]);
  }

  method_list_global = dyn_cast<User>(
      GetUnderlyingObject(protocol_t->getOperand(4), M->getDataLayout(), 2));
  if (method_list_global->getNumOperands() != 0) {
    auto objc_meth_list =
        dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
    HandleObjCMethods(M, objc_meth_list,
                      result.protocolClassMethods[protocolName]);
  }

  // Optional instance methods.
  method_list_global = dyn_cast<User>(
      GetUnderlyingObject(protocol_t->getOperand(5), M->getDataLayout(), 2));
  if (method_list_global->getNumOperands() != 0) {
    auto objc_meth_list =
        dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
    HandleObjCMethods(M, objc_meth_list,
                      result.protocolInstanceMethods[protocolName]);
  }

  // Optional class methods.
  method_list_global = dyn_cast<User>(
      GetUnderlyingObject(protocol_t->getOperand(6), M->getDataLayout(), 2));
  if (method_list_global->getNumOperands() != 0) {
    auto objc_meth_list =
        dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
    HandleObjCMethods(M, objc_meth_list,
                      result.protocolClassMethods[protocolName]);
  }

  // Properties, getter and setter are instance methods.
  auto prop_list_global = dyn_cast<GlobalVariable>(
      GetUnderlyingObject(protocol_t->getOperand(7), M->getDataLayout(), 2));
  if (prop_list_global != nullptr)
    HandleObjCPropertyList(M, prop_list_global,
                           result.protocolInstanceMethods[protocolName]);
}

void TraverseObjCCategoryList(GlobalVariable &GV, Module *M,
                              APIAnalysisIntermediateResult &result,
                              ObjCClassCollection &collection) {
  auto categoryList = dyn_cast<User>(GV.getInitializer());
  assert(categoryList &&
         "__objc_classlist should be a User because it is an array");
  for (auto &op : categoryList->operands()) {
    int64_t cat_offset = 0;
    auto category = dyn_cast<GlobalVariable>(GetPointerBaseWithConstantOffset(
        op, cat_offset, M->getDataLayout()));
    if (!category) {
        errs() << "objc category is not global variable\n";
        continue;
    }
    auto category_t_global = GlobalOpToUser(op);
    auto category_t =
        dyn_cast<ConstantStruct>(GlobalOpToUser(category_t_global));
    if (!category_t) {
        errs() << "objc category is not constant struct\n";
        continue;
    }

    ConstantDataSequential *CD;

    // read the category name
    auto cat_name_global = dyn_cast<GlobalVariable>(GetUnderlyingObject(
        GetValueAtStructOffset(category_t, cat_offset, M->getDataLayout()),
        M->getDataLayout(), 2));
    assert(cat_name_global && "Category name must be a global");
    CD = dyn_cast<ConstantDataSequential>(cat_name_global->getInitializer());
    assert(CD && "Category name must be a ConstantDataSequential");
    StringRef catName = TrimNull(CD->getAsString());

    // Get extended class name
    Triple TT(M->getTargetTriple());
    unsigned offset = TT.isArch64Bit() ? 8 : 4;
    auto class_t = GetValueAtStructOffset(category_t, cat_offset + offset,
                                          M->getDataLayout());
    auto classInfo =
        HandleObjCClass(M, class_t, result.classLinkMap, collection);
    assert(classInfo && "Could not read class info in Category");
    StringRef className = classInfo->className;

    offset = TT.isArch64Bit() ? 16 : 8;
    auto method_list_global = dyn_cast<User>(GetUnderlyingObject(
        GetValueAtStructOffset(category_t, cat_offset + offset,
                               M->getDataLayout()),
        M->getDataLayout(), 2));
    if (method_list_global->getNumOperands() != 0) {
      auto objc_meth_list =
          dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
      HandleObjCMethods(M, objc_meth_list,
                        result.categoryInstanceMethods[className][catName]);
    }

    offset = TT.isArch64Bit() ? 24 : 12;
    method_list_global = dyn_cast<User>(GetUnderlyingObject(
        GetValueAtStructOffset(category_t, cat_offset + offset,
                               M->getDataLayout()),
        M->getDataLayout(), 2));
    if (method_list_global->getNumOperands() != 0) {
      auto objc_meth_list =
          dyn_cast<User>(GlobalOpToUser(method_list_global)->getOperand(2));
      HandleObjCMethods(M, objc_meth_list,
                        result.categoryClassMethods[className][catName]);
    }

    // property list
    offset = TT.isArch64Bit() ? 40 : 20;
    auto property_list_global = dyn_cast<GlobalVariable>(GetUnderlyingValue(
        GetValueAtStructOffset(category_t, offset, M->getDataLayout()),
        M->getDataLayout(), 2));
    HandleObjCPropertyList(M, property_list_global,
                           result.categoryInstanceMethods[className][catName]);
  }
}

int AnalyzeModuleImpl(Module *M, APIAnalysisIntermediateResult &result,
                      const APIAnalysisOptions &options) {
  // get selectors called via normal obj-c syntax
  ObjCClassCollection collection; // FIXME: Leaking memory.
  Mangler mangler;

  auto versionMD = M->getModuleFlag("Objective-C Version");
  if (versionMD) {
    auto objcVer =
        cast<ConstantInt>(cast<ConstantAsMetadata>(versionMD)->getValue())
            ->getZExtValue();
    if (objcVer != 2) {
      llvm::errs() << "ObjC Version not supported\n";
      return 1;
    }
  }

  for (auto &GV : M->globals()) {
    if (GV.hasSection()) {
      StringRef Section = GV.getSection();
      std::string SectionName;
      // Trim all spaces in the section name.
      for (auto c : Section) {
        if (c != ' ')
          SectionName.push_back(c);
      }
      if (SectionName ==
                 "__DATA,__objc_classlist,regular,no_dead_strip") {
        TraverseObjCClassList(GV, M, result, collection);
      } else if (SectionName ==
                 "__DATA,__objc_protolist,coalesced,no_dead_strip") {
        TraverseObjCProtoList(GV, M, result);
      } else if (SectionName ==
                 "__DATA,__objc_catlist,regular,no_dead_strip") {
        TraverseObjCCategoryList(GV, M, result, collection);
      } else if (SectionName ==
                 "__DATA,__objc_selrefs,literal_pointers,no_dead_strip") {
        auto sel_global =
            GetUnderlyingObject(GV.getInitializer(), M->getDataLayout(), 2);
        auto CD = dyn_cast<ConstantDataSequential>(GlobalOpToUser(sel_global));
        if (CD)
          result.messageNames.insert(TrimNull(CD->getAsString()));
        else
          errs() << "Cound not read selrefs\n";
        if (hasUserInMethodList(sel_global, M))
          result.potentiallyDefinedSelectors.insert(
              TrimNull(CD->getAsString()));
      } else if (SectionName ==
                 "__DATA,__objc_classrefs,regular,no_dead_strip") {
        ObjCClass *objc_info = HandleObjCClass(M, GV.getInitializer(),
                                               result.classLinkMap, collection);
        if (objc_info) {
          InsertUnique(result.classNames, objc_info->className);
        }
      }
    }
    if (!ShouldRecordGlobal(&GV))
      continue;

    SmallString<256> NameSS;
    if (!options.demangle) {
      mangler.getNameWithPrefix(NameSS, &GV, true);
    }

    UpdateObjCClassCollection(NameSS, !options.demangle, collection);

    if (isLinkageUnitLocalGlobal(&GV))
      result.localSymbols.insert(NameSS);
    else
      UpdateStringMapWithValue(result.globals, NameSS, isDefinedGlobal(&GV));
  }

  // aliases
  for (const auto& A : M->aliases()) {
    if (!ShouldRecordGlobal(&A))
      continue;

    SmallString<256> NameSS;
    if (!options.demangle) {
      mangler.getNameWithPrefix(NameSS, &A, true);
    }

    UpdateObjCClassCollection(NameSS, !options.demangle, collection);

    if (isLinkageUnitLocalGlobal(&A))
      result.localSymbols.insert(NameSS);
    else
      UpdateStringMapWithValue(result.globals, NameSS, isDefinedGlobal(&A));
  }

  // get function names
  for (auto &F : M->functions()) {
    if (!ShouldRecordGlobal(&F))
      continue;

    StringRef Name;
    SmallString<256> NameSS;
    if (options.demangle)
      Name = F.getName();
    else {
      mangler.getNameWithPrefix(NameSS, &F, true);
      Name = NameSS;
    }

    UpdateObjCClassCollection(NameSS, !options.demangle, collection);

    // Keep track of functions that are declared vs defined. Declared
    // functions
    // that are not defined anywhere will become unresolved external
    // references.
    if (isLinkageUnitLocalGlobal(&F))
      result.localSymbols.insert(NameSS);
    else
      UpdateStringMapWithValue(result.functionNames, Name, isDefinedGlobal(&F));

    // Track NSSelectorFromString calls to find selectors referenced by
    // NSString
    // literals. This is a bit fragile based on the CFString ABI.
    if (options.scanSelFromString && "NSSelectorFromString" == F.getName()) {
      for (auto &u : F.uses()) {
        auto op = u.getUser()->getOperand(0)->stripPointerCasts();
        if (GlobalValue *GV = dyn_cast<GlobalValue>(op)) {
          if (ConstantStruct *CS =
                  dyn_cast<ConstantStruct>(GV->getOperand(0))) {
            auto str =
                GetUnderlyingObject(CS->getOperand(2), M->getDataLayout(), 2);
            if (GlobalVariable *GV = dyn_cast<GlobalVariable>(str))
              if (ConstantDataSequential *CD =
                      dyn_cast<ConstantDataSequential>(GV->getInitializer()))
                result.messageNames.insert(TrimNull(CD->getAsString()));
          }
        }
      }
    }

    if (options.scanDLSym && "dlsym" == F.getName()) {
      for (auto &u : F.uses()) {
        auto str = GetUnderlyingObject(u.getUser()->getOperand(1),
                                       M->getDataLayout(), 2);
        if (GlobalVariable *GV = dyn_cast<GlobalVariable>(str))
          if (ConstantDataSequential *CD =
                  dyn_cast<ConstantDataSequential>(GV->getInitializer()))
            result.functionNames.insert(
                std::make_pair(TrimNull(CD->getAsString()), false));
      }
    }

  }

  // Handle inline asm
  HandleInlineAsm(M, result, collection);

  // process objc meta data.
  for (const auto &entry : collection) {
    auto c = &entry.second;
    UpdateStringMapWithValue(result.classNames, c->className, c->isDefined);
    if (c->superClass)
      result.superClasses.insert(
          std::make_pair(c->className, c->superClass->className));
    if (c->isDefined) {
      for (auto &method : c->instanceMethods)
        result.instanceMethods[c->className].insert({method.first,
                                                     method.second});
      for (auto &method : c->classMethods)
        result.classMethods[c->className].insert({method.first, method.second});
    }
  }

  return 0;
}
