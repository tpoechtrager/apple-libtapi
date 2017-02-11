//===-- LLVMModuleObfuscation.cpp - LTO Module obfuscatior ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides helper classes to perform string obfuscation
//
//===----------------------------------------------------------------------===//

#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/ADT/Triple.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Mangler.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/TypeFinder.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/Obfuscation.h"
#include "llvm/Support/Options.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/Utils/ModuleObfuscator.h"
#include "llvm/Pass.h"

#include <string>

using namespace llvm;

// StringSet holds all the special symbols.
static ManagedStatic<llvm::StringSet<>> SpecialSymbolSet;

// Options that have extra controls over the pass
static cl::opt<std::string>
    SymbolFileOut("obfuscation-symbol-map",
                  cl::ValueRequired,
                  cl::desc("Specify the symbol_map output"),
                  cl::value_desc("filename.bcsymbolmap"));
static cl::list<std::string> PreserveSymbols("obfuscate-preserve",
                                            cl::ZeroOrMore, cl::ValueRequired,
                                            cl::desc("<sym name>"));

namespace {
/// \brief This pass obfuscate the strings inside the module.
struct ObfuscateModule : public ModulePass {
  static char ID; // Pass identification, replacement for typeid

  explicit ObfuscateModule(const TargetMachine *TM = nullptr)
      : ModulePass(ID), TM(TM) {
    initializeObfuscateModulePass(*PassRegistry::getPassRegistry());
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }

  bool runOnModule(Module &M) override {
    return obfuscateModule(M, TM);
  }

private:
  const TargetMachine *TM;
};

/// \brief Helper class to recursively gather and modify all MDStrings from
/// given entry points
///
/// Will only ever visit a given MDNode once. User can feed in MDNodes with
/// "addEntryPoint". User can then incrementally, at any time call "run" to
/// start recursively modifying reachable MDStrings from un-visited entry
/// points.
class MDSRecursiveModify {
  SmallVector<MDNode *, 16> Worklist;
  DenseSet<const MDNode *> Visited;

  LLVMContext &Ctx;
  std::function<StringRef(StringRef)> F;

public:
  MDSRecursiveModify(LLVMContext &Context,
                     std::function<StringRef(StringRef)> NewStr)
      : Ctx(Context), F(NewStr) {}

  /// \brief Add a new entry point into the metadata graph to traverse from
  void addEntryPoint(MDNode *N) {
    if (!N || !Visited.count(N))
      Worklist.push_back(N);
  }
  void addEntryPoint(NamedMDNode *NMD) {
    for (auto I : NMD->operands())
      addEntryPoint(I);
  }
  void addEntryPoint(DbgInfoIntrinsic *DII) {
    if (auto DDI = dyn_cast<DbgDeclareInst>(DII))
      addEntryPoint(DDI->getVariable());
    else if (auto DVI = dyn_cast<DbgValueInst>(DII))
      addEntryPoint(DVI->getVariable());
    else
      llvm_unreachable("invalid debug info intrinsic");
  }

  /// \brief Recursively modify reachable, unvisited MDStrings
  inline void run();
};
} // End llvm namespace

void MDSRecursiveModify::run() {
  while (!Worklist.empty()) {
    auto N = Worklist.pop_back_val();
    if (!N || Visited.count(N))
      continue;
    Visited.insert(N);
    for (unsigned i = 0; i < N->getNumOperands(); ++i) {
      Metadata *MD = N->getOperand(i);
      if (!MD)
        continue;
      if (auto MDS = dyn_cast<MDString>(MD)) {
        assert(N->isResolved() && "Unexpected forward reference");
        N->replaceOperandWith(i, MDString::get(Ctx, F(MDS->getString())));
      } else if (auto NN = dyn_cast<MDNode>(MD))
        Worklist.push_back(NN);
    }
  }
}

static bool
obfuscateModuleModify(Module &M, const TargetMachine *TM, Obfuscator &obfs,
                      std::function<bool(StringRef, StringRef)> preserve) {
  // Obfuscate source file name.
  auto SourceName = M.getSourceFileName();
  if (SourceName.size() != 0)
    M.setSourceFileName("");

  // Obfuscate value names
  SmallString<64> MangledName;
  Mangler mangler;
  auto ShouldObfuscateGlobalName = [&](GlobalValue &V) {
    if (!V.hasName() || V.hasPrivateLinkage() ||
        V.getName().startswith("llvm.") || V.getName().startswith("clang.arc"))
      return StringRef();
    MangledName.clear();
    TM->getNameWithPrefix(MangledName, &V, mangler);
    if (!preserve(V.getName(), MangledName))
      return StringRef(MangledName);
    else
      return StringRef();
  };

  {
    // Obfuscate instruction metadata.
    // Only following metadata attached to the instructions are 
    // obufscated:
    // 1. tbaa and tbaa.struct
    // 2. alias.scope and noalias
    // 3. invariant.group
    // None of them has to be reversible and they dont intersect
    // with other metadata.
    MDSRecursiveModify MDInstrModify(M.getContext(), [&obfs](StringRef S) {
      return obfs.obfuscate(S, false);
    });
    typedef std::pair<GlobalValue*, SmallString<64>> WorkEntry;
    SmallVector<WorkEntry, 64> Worklist;
    for (auto &F : M) {
      // While we're here, drop parameter names if they exist
      for (auto &Arg : F.args())
        Arg.setName("");
      // Drop all BasicBlock names as well
      for (auto &BB : F) {
        BB.setName("");
        for (auto &I : BB) {
          // Obfuscate some of the metadata that contains type info
          // or descriptive strings.
          if (!I.hasMetadata())
            continue;
          if (auto MD = I.getMetadata(LLVMContext::MD_tbaa))
            MDInstrModify.addEntryPoint(MD);
          if (auto MD = I.getMetadata(LLVMContext::MD_tbaa_struct))
            MDInstrModify.addEntryPoint(MD);
          if (auto MD = I.getMetadata(LLVMContext::MD_alias_scope))
            MDInstrModify.addEntryPoint(MD);
          if (auto MD = I.getMetadata(LLVMContext::MD_noalias))
            MDInstrModify.addEntryPoint(MD);
          if (auto MD = I.getMetadata(LLVMContext::MD_invariant_group))
            MDInstrModify.addEntryPoint(MD);
        }
      }

      if (F.isIntrinsic())
        continue;

      auto MName = ShouldObfuscateGlobalName(F);
      if (!MName.empty()) {
        Worklist.push_back({&F, StringRef(MName)});
      }
    }

    // Run MD Modifier.
    MDInstrModify.run();

    for (auto &I : M.globals()) {
      auto MName = ShouldObfuscateGlobalName(I);
      if (!MName.empty()) {
        Worklist.push_back({&I, StringRef(MName)});
      }
    }
    for (auto &I : M.aliases()) {
      auto MName = ShouldObfuscateGlobalName(I);
      if (!MName.empty()) {
        Worklist.push_back({&I, StringRef(MName)});
      }
    }
    // Obfuscate all the symbols in the list.
    // Run two passes across the worklist, first time rename to a temp name
    // to avoid conflicts.
    for (auto &I : Worklist)
      // Just set to something but don't really care what it becames.
      I.first->setName("__obfs_tmp#");
    for (auto &I : Worklist) {
      StringRef obfName = obfs.obfuscate(I.second, true);
      I.first->setName(std::string("\01") + obfName.str());
    }
  }

  // Drop type names
  TypeFinder TF;
  TF.run(M, true);
  for (auto I : TF)
    // typename is not in the symbol table so temporary conflict is ok.
    I->setName(obfs.obfuscate(I->getName()));

  // Obfuscate debug info strings
  MDSRecursiveModify Modify(M.getContext(),
                            [&obfs](StringRef S) {
                              return obfs.obfuscate(S, true);
                            });

  // llvm.dbg.cu and llvm.gcov  needs to be obfuscated
  if (auto DBG = M.getNamedMetadata("llvm.dbg.cu"))
    Modify.addEntryPoint(DBG);
  if (auto GCOV = M.getNamedMetadata("llvm.gcov"))
    Modify.addEntryPoint(GCOV);

  Modify.run();

  // llvm.dbg.value/declare's "variable" argument needs to be obfuscated
  auto addDbgIntrinsicCalls = [&](StringRef Name) {
    auto F = M.getFunction(Name);
    if (!F)
      return;
    for (auto Usr : F->users())
      Modify.addEntryPoint(cast<DbgInfoIntrinsic>(Usr));
  };
  addDbgIntrinsicCalls("llvm.dbg.declare");
  addDbgIntrinsicCalls("llvm.dbg.value");
  Modify.run();

  // Every insruction's DebugLoc needs to be obfuscated
  for (auto &F : M.functions())
    for (auto &BB : F)
      for (auto &I : BB) {
        // While we're here, go ahead and drop its name if it has one
        I.setName("");
        Modify.addEntryPoint(I.getDebugLoc().getAsMDNode());
      }
  Modify.run();

  // We only rename stuff, never really change IR.
  return false;
}

bool llvm::obfuscateModule(Module &M, const TargetMachine *TM) {
  IncrementObfuscator IncrObfuscate;
  StringSet<> Preserves;
  // Add PreserveSymbols from commandline.
  for (auto & sym : PreserveSymbols)
    Preserves.insert(sym);
  bool Changed = obfuscateModule(M, TM, IncrObfuscate, Preserves);
  // Output SymbolFile if needed.
  if (!SymbolFileOut.empty()) {
    // Create output file
    std::error_code EC;
    raw_fd_ostream Out(StringRef(SymbolFileOut), EC, sys::fs::F_None);
    if (EC) {
      // Just return.
      llvm::errs() << "could not open obfuscation symbol map: " <<
        SymbolFileOut << "\n";
      return Changed;
    }

    // Write reverse map
    IncrObfuscate.writeReverseMap(Out);
    Out.close();
  }
  return Changed;
}

bool llvm::obfuscateModule(Module &M, const TargetMachine *TM, Obfuscator &obfs,
                           StringSet<> &PreserveSymbols) {
  // Predicate to indicate if we should preserve the original name.
  TargetLibraryInfoImpl TLII(Triple(M.getTargetTriple()));
  TargetLibraryInfo TLI(TLII);
  auto isLibName = [&TLI](StringRef &S) {
    LibFunc::Func F;
    return TLI.getLibFunc(S, F);
  };

  // Handle llvm special symbols.
  static const char* const SpecialSymbols[] = {
#define COMPILER_SYMBOL(Name) #Name,
#include "llvm/Transforms/Utils/CompilerRTSymbols.def"
#undef COMPILER_SYMBOL
      "_objc_retain",
      "_objc_release",
      "_objc_autorelease",
      "_objc_retainAutoreleasedReturnValue",
      "_objc_retainBlock",
      "_objc_autoreleaseReturnValue",
      "_objc_autoreleasePoolPush",
      "_objc_loadWeakRetained",
      "_objc_loadWeak",
      "_objc_destroyWeak",
      "_objc_storeWeak",
      "_objc_initWeak",
      "_objc_moveWeak",
      "_objc_copyWeak",
      "_objc_retainedObject",
      "_objc_unretainedObject",
      "_objc_unretainedPointer",
      "___stack_chk_fail",
      "___stack_chk_guard"
  };
  static const unsigned NumSpecialSymbols = sizeof(SpecialSymbols) /
                                            sizeof(const char *);
  // Create and insert the symbols into StringSet.
  if (!SpecialSymbolSet.isConstructed()) {
    for (unsigned i = 0; i < NumSpecialSymbols; ++i)
      SpecialSymbolSet->insert(StringRef(SpecialSymbols[i]));
  }
  // Take the address of the StringSet so it can be captured.
  auto SymbolSet = &*SpecialSymbolSet;
  // Whether this is a special symbol that the compiler recognizes. This can
  // either be a compiler-internal symbol, or an external symbol that the
  // compiler special cases. Eitherway, checks based on name
  auto isSpecialSymbolName = [SymbolSet](StringRef Name) {
    // Special symbols supplied by ld64.
    if (Name.startswith("___dtrace") ||
        Name.startswith("section$start$") ||
        Name.startswith("section$end$") ||
        Name.startswith("segment$start$") ||
        Name.startswith("segment$end$"))
      return true;

    // Some special external names, which might of gotten dropped due to
    // optimizations
    if (SymbolSet->count(Name))
      return true;

    return false;
  };

  auto mustPreserve = [&PreserveSymbols, isLibName, isSpecialSymbolName](
      StringRef Name, StringRef MangledName) {
    return PreserveSymbols.count(MangledName) || isLibName(Name) ||
           isSpecialSymbolName(MangledName);
  };

  return obfuscateModuleModify(M, TM, obfs, mustPreserve);
}

char ObfuscateModule::ID = 0;
INITIALIZE_TM_PASS(ObfuscateModule, "obfuscate-module",
                   "Obfuscate all strings in the module", false, false)

ModulePass *llvm::createObfuscateModulePass(const llvm::TargetMachine *TM) {
  return new ObfuscateModule(TM);
}
