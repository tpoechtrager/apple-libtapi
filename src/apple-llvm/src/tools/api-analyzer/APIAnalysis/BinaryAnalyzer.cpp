
#include "BinaryAnalyzer.h"
#include "XarAnalyzer.h"
#include "ObjCStructs.h"

#include "llvm/ObjCMetadata/ObjCMetadata.h"
#include "llvm/ObjCMetadata/ObjCMachOBinary.h"
#include "llvm/Object/MachO.h"
#include "llvm/Object/MachOUniversal.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace object;

// Assume that __cxa_demangle is provided by libcxxabi (except for Windows).
extern "C" char *__cxa_demangle(const char *mangled_name, char *output_buffer,
                                size_t *length, int *status);

std::string DemangleName(const std::string &Name) {
  // We can spoil names of symbols with C linkage, so use an heuristic
  // approach to check if the name should be demangled.
  if (Name.substr(0, 3) != "__Z") {
    if (Name.substr(0, 1) == "_")
      return Name.substr(1);
    else
      return Name;
  }
  int status = 0;
  char *DemangledName =
      __cxa_demangle(Name.c_str() + 1, nullptr, nullptr, &status);
  if (status != 0)
    return Name;
  std::string Result = DemangledName;
  free(DemangledName);
  return Result;
}

static inline void InsertExternalObjCClass(StringRef SymName, bool mangled,
                                           StringMap<bool> &classes) {
  auto insertClassName = [&](StringRef Sym) {
    if (classes.find(Sym) == classes.end())
      classes.insert({Sym, false});
  };

  if (mangled) {
    if (SymName.startswith("_OBJC_CLASS_$_"))
      insertClassName(SymName.drop_front(strlen("_OBJC_CLASS_$_")));
    else if (SymName.startswith(".objc_class_name_"))
      insertClassName(SymName.drop_front(strlen(".objc_class_name_")));
  } else {
    if (SymName.startswith("OBJC_CLASS_$_"))
      insertClassName(SymName.drop_front(strlen("OBJC_CLASS_$_")));
    else if (SymName.startswith("\01.objc_class_name_"))
      insertClassName(SymName.drop_front(strlen("\01.objc_class_name_")));
  }
}

int AnalyzeBinary(StringRef filePath, APIAnalysisIntermediateResult &result,
                  const APIAnalysisOptions &options) {
  ErrorOr<OwningBinary<Binary>> BinaryOrErr = createBinary(filePath);
  if (std::error_code EC = BinaryOrErr.getError()) {
    errs() << "unable to read input: '" << EC.message() << "'\n";
    return 1;
  }
  Binary &binary = *BinaryOrErr.get().getBinary();
  return AnalyzeBinaryImpl(binary, result, options);
}

int AnalyzeBinary(llvm::MemoryBufferRef fileData,
                  APIAnalysisIntermediateResult &result,
                  const APIAnalysisOptions &options) {
  ErrorOr<std::unique_ptr<Binary>> BinaryOrErr = createBinary(fileData);
  if (std::error_code EC = BinaryOrErr.getError()) {
    errs() << "unable to read input: '" << EC.message() << "'\n";
    return 1;
  }
  Binary &binary = *BinaryOrErr.get();
  return AnalyzeBinaryImpl(binary, result, options);
}

int AnalyzeBinaryImpl(const MachOObjectFile *InputObject,
                      APIAnalysisIntermediateResult &result,
                      const APIAnalysisOptions &options) {
  // check to see if there is a bitcode section
  bool hasBitcode = false;
  for (auto s : InputObject->sections()) {
    StringRef SegmentName =
        InputObject->getSectionFinalSegmentName(s.getRawDataRefImpl());
    StringRef SectName;
    s.getName(SectName);
    if (SegmentName == "__LLVM" && SectName == "__bundle") {
      if (options.bitcodeOnly) {
        StringRef contents;
        if (std::error_code EC = s.getContents(contents)) {
          errs() << "Could not get section data: '" << EC.message() << "'";
          return 1;
        }
        auto xarBuf = MemoryBuffer::getMemBuffer(
            contents, "", /*RequiresNullTerminator=*/false);
        AnalyzeXar(xarBuf->getMemBufferRef(), result, options);
      }
      hasBitcode = true;
      break;
    }
  }

  // if there is no bitcode section, this may be from an assembly file that was
  // hand coded, and since we support that we need to ignore bitcodeOnly
  bool bitcodeOnly = options.bitcodeOnly && hasBitcode;
  if (!bitcodeOnly) {
    for (auto s : InputObject->symbols()) {
      section_iterator sec = s.getSection().get();

      // If the symbol has a section, and the symbol has no flags -- skip it.
      if (sec != InputObject->section_end() &&
          BasicSymbolRef::SF_None == s.getFlags())
        continue;

      // If the symbol is format specific, skip it
      if (BasicSymbolRef::SF_FormatSpecific == s.getFlags())
        continue;

      auto type = s.getType();
      std::string name = s.getName()->str();
      if (options.demangle)
        name = DemangleName(name);
      bool declared = !(s.getFlags() & BasicSymbolRef::SF_Undefined);

      if (SymbolRef::ST_Function & type) {
        auto entry = result.functionNames.find(name);
        if (entry != result.functionNames.end())
          entry->second = entry->second || declared;
        else
          result.functionNames.insert(std::make_pair(name, declared));
      }

      if (SymbolRef::ST_Data & type) {
        auto entry = result.globals.find(name);
        if (entry != result.globals.end())
          entry->second = entry->second || declared;
        else
          result.globals.insert(std::make_pair(name, declared));
      }

      if (!declared)
        InsertExternalObjCClass(name, !options.demangle, result.classNames);
    }
  }

  // If only looking at bitcode skip everything else.
  if (bitcodeOnly)
    return 0;

  // Parse ObjC Metadata.
  MachOMetadata ObjCInfo(InputObject);
  Error hasError;
  auto recordError = [&](Error&& NewError) {
    hasError = joinErrors(std::move(hasError), std::move(NewError));
  };

  // Handle ObjC Classes.
  if (auto ObjCClasses = ObjCInfo.classes()) {
    for (auto c : *ObjCClasses) {
      auto ObjCClass = *c;
      if (!ObjCClass) {
        recordError(ObjCClass.takeError());
        continue;
      }
      auto name = ObjCClass->getName();
      if (!name) {
        recordError(name.takeError());
        continue;
      }
      result.classNames.insert({*name, true});
      // Instance Methods.
      if (auto instanceMethods = ObjCClass->instanceMethods()) {
        for (auto m : *instanceMethods) {
          if (auto mName = m.getName())
            result.instanceMethods[*name].insert({*mName, true});
          else
            recordError(mName.takeError());
        }
      } else
        recordError(instanceMethods.takeError());
      // Class Methods.
      if (auto classMethods = ObjCClass->classMethods()) {
        for (auto m : *classMethods) {
          if (auto mName = m.getName())
            result.classMethods[*name].insert({*mName, true});
          else
            recordError(mName.takeError());
        }
      } else
        recordError(classMethods.takeError());
      // Properties.
      if (auto Properties = ObjCClass->properties()) {
        for (auto m : *Properties) {
          if (auto getter = m.getGetter()) {
            if (!getter->empty())
              result.instanceMethods[*name].insert({*getter, true});
          } else
            recordError(getter.takeError());
          if (auto setter = m.getSetter()) {
            if (!setter->empty())
              result.instanceMethods[*name].insert({*setter, true});
          } else
            recordError(setter.takeError());
        }
      } else
        recordError(Properties.takeError());
      // Superclass.
      if (auto superclass = ObjCClass->getSuperClassName())
        result.superClasses[*name] = *superclass;
      else
        recordError(superclass.takeError());
    }
  } else
    recordError(ObjCClasses.takeError());

  // Handle ObjC Protocols.
  if (auto ObjCProtocols = ObjCInfo.protocols()) {
    for (auto p : *ObjCProtocols) {
      auto ObjCProtocol = *p;
      if (!ObjCProtocol) {
        recordError(ObjCProtocol.takeError());
        continue;
      }
      auto name = ObjCProtocol->getName();
      if (!name) {
        recordError(name.takeError());
        continue;
      }
      // Instance Methods.
      if (auto instanceMethods = ObjCProtocol->instanceMethods()) {
        for (auto m : *instanceMethods) {
          if (auto mName = m.getName())
            result.protocolInstanceMethods[*name].insert({*mName, true});
          else
            recordError(mName.takeError());
        }
      } else
        recordError(instanceMethods.takeError());
      if (auto instanceMethods = ObjCProtocol->optionalInstanceMethods()) {
        for (auto m : *instanceMethods) {
          if (auto mName = m.getName())
            result.protocolInstanceMethods[*name].insert({*mName, true});
          else
            recordError(mName.takeError());
        }
      } else
        recordError(instanceMethods.takeError());
      // Class Methods.
      if (auto classMethods = ObjCProtocol->classMethods()) {
        for (auto m : *classMethods) {
          if (auto mName = m.getName())
            result.protocolClassMethods[*name].insert({*mName, true});
          else
            recordError(mName.takeError());
        }
      } else
        recordError(classMethods.takeError());
      if (auto classMethods = ObjCProtocol->optionalClassMethods()) {
        for (auto m : *classMethods) {
          if (auto mName = m.getName())
            result.protocolClassMethods[*name].insert({*mName, true});
          else
            recordError(mName.takeError());
        }
      } else
        recordError(classMethods.takeError());
      // Properties.
      if (auto Properties = ObjCProtocol->properties()) {
        for (auto m : *Properties) {
          if (auto getter = m.getGetter()) {
            if (!getter->empty())
              result.protocolInstanceMethods[*name].insert({*getter, true});
          } else
            recordError(getter.takeError());
          if (auto setter = m.getSetter()) {
            if (!setter->empty())
              result.protocolInstanceMethods[*name].insert({*setter, true});
          } else
            recordError(setter.takeError());
        }
      } else
        recordError(Properties.takeError());
    }
  } else
    recordError(ObjCProtocols.takeError());

  // Handle ObjC Categories.
  if (auto ObjCCategories = ObjCInfo.categories()) {
    for (auto c : *ObjCCategories) {
      auto ObjCCategory = *c;
      if (!ObjCCategory) {
        recordError(ObjCCategory.takeError());
        continue;
      }
      auto name = ObjCCategory->getName();
      if (!name) {
        recordError(name.takeError());
        continue;
      }
      auto baseName = ObjCCategory->getBaseClassName();
      if (!baseName) {
        recordError(baseName.takeError());
        continue;
      }
      // Instance Methods.
      if (auto instanceMethods = ObjCCategory->instanceMethods()) {
        for (auto m : *instanceMethods) {
          if (auto mName = m.getName())
            result.categoryInstanceMethods[*baseName][*name].insert(
                {*mName, true});
          else
            recordError(mName.takeError());
        }
      } else
        recordError(instanceMethods.takeError());
      // Class Methods.
      if (auto classMethods = ObjCCategory->classMethods()) {
        for (auto m : *classMethods) {
          if (auto mName = m.getName())
            result.categoryClassMethods[*baseName][*name].insert(
                {*mName, true});
          else
            recordError(mName.takeError());
        }
      } else
        recordError(classMethods.takeError());
      // Properties.
      if (auto Properties = ObjCCategory->properties()) {
        for (auto m : *Properties) {
          if (auto getter = m.getGetter()) {
            if (!getter->empty())
              result.categoryInstanceMethods[*baseName][*name].insert(
                  {*getter, true});
          } else
            recordError(getter.takeError());
          if (auto setter = m.getSetter()) {
            if (!setter->empty())
              result.categoryInstanceMethods[*baseName][*name].insert(
                  {*setter, true});
          } else
            recordError(setter.takeError());
        }
      } else
        recordError(Properties.takeError());
    }
  } else
    recordError(ObjCCategories.takeError());

  // Handle Selector References.
  if (auto ObjCSelectRefs = ObjCInfo.referencedSelectors()) {
    for (auto s : *ObjCSelectRefs) {
      if (auto name = s.getSelector())
        result.messageNames.insert(*name);
      else
        recordError(name.takeError());
    }
  } else
    recordError(ObjCSelectRefs.takeError());

  // Potentially Defined Selectors;
  ObjCInfo.getAllPotentiallyDefinedSelectors(
      result.potentiallyDefinedSelectors);
  // Log all errors.
  logAllUnhandledErrors(std::move(hasError), errs(),
                        "All binary scanning errors:\n");

  for (size_t i = 0; i < InputObject->getNumLibraries(); ++i) {
    StringRef name;
    if (std::error_code EC = InputObject->getLibraryNameByIndex(i, name)) {
      errs() << "Failed to read library name: " << EC.message();
      continue;
    }
    StringRef Suffix;
    bool isFramework;
    StringRef shortName =
        InputObject->guessLibraryShortName(name, isFramework, Suffix);
    if (name.endswith(".dylib"))
      Suffix = ".dylib";
    result.linkedLibraries.insert((shortName + Suffix).str());
  }
  return 0;
}

int AnalyzeBinaryImpl(Binary &binary, APIAnalysisIntermediateResult &result,
                      const APIAnalysisOptions &options) {
  const MachOUniversalBinary *UniversalBinary =
      dyn_cast<MachOUniversalBinary>(&binary);
  if (UniversalBinary) {
    int retCode = 0;
    for (auto &obj : UniversalBinary->objects()) {
      ErrorOr<std::unique_ptr<MachOObjectFile>> errorOrSlice =
          obj.getAsObjectFile();
      if (std::error_code EC = errorOrSlice.getError()) {
        outs() << "Could not read slice: '" << EC.message() << "'\n";
        retCode &= 1;
        continue;
      }
      retCode &= AnalyzeBinaryImpl(errorOrSlice.get().get(), result, options);
    }
    return retCode;
  }

  const MachOObjectFile *InputObject = dyn_cast<MachOObjectFile>(&binary);
  if (InputObject) {
    return AnalyzeBinaryImpl(InputObject, result, options);
  }

  errs() << "Unrecognized binary format!\n";
  return 1;
}
