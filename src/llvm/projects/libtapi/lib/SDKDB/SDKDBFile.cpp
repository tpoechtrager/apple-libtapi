//===- tapi/Core/SDKDBFile.cpp - TAPI SDKDB File ----------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the SDKDB File
///
//===----------------------------------------------------------------------===//

#include "tapi/SDKDB/SDKDBFile.h"
#include "tapi/Core/AvailabilityInfo.h"
#include "llvm/Support/Error.h"

using namespace llvm;

TAPI_NAMESPACE_INTERNAL_BEGIN

static inline bool tryMergeAvailabilityInfo(AvailabilityInfo &A,
                                            const AvailabilityInfo &B) {
  if (A == B || B.isDefault())
    return true;

  if (A.isDefault()) {
    A = B;
    return true;
  }

  return false;
}

static inline Error mergeAvailabilityInfo(AvailabilityInfo &A,
                                          const AvailabilityInfo &B,
                                          StringRef context) {

  if (tryMergeAvailabilityInfo(A, B))
    return Error::success();

  std::string ErrStr;
  llvm::raw_string_ostream os(ErrStr);

  os << "Failed to combine availability " << A << " and " << B;

  if (!context.empty())
    os << " for " << context;

  return llvm::make_error<llvm::StringError>(
      os.str(),
      std::make_error_code(std::errc::not_supported));
}

static inline Expected<AvailabilityInfo> mergeAllAvailabilityInfo(
    const llvm::SmallVectorImpl<std::pair<Architecture, AvailabilityInfo>>
        &availability,
    const XPI *xpi) {
  AvailabilityInfo result;
  for (const auto &info : availability)
    if (auto error = mergeAvailabilityInfo(result, info.second,
                                           xpi->getAnnotatedName(false)))
      return std::move(error);
  return result;
}

void SDKDBFile::addGlobalSymbol(StringRef name, bool isPublic,
                                const AvailabilityInfo &availability) {
  auto entry = SymbolEntry(name, isPublic, availability);
  auto result = symbols.emplace(std::make_pair(name, entry));
  assert(result.second && "unexpected element in the global symbol map");
  (void)result;
}

SDKDBFile::ObjCClassEntry *
SDKDBFile::addObjectiveCClass(StringRef name, StringRef superClassName,
                              bool isPublic,
                              const AvailabilityInfo &availability) {
  auto entry = ObjCClassEntry(name, superClassName, isPublic, availability);
  auto result = classes.emplace(std::make_pair(name, entry));
  assert(result.second && "unexpected element in class map");
  return &result.first->second;
}

SDKDBFile::ObjCCategoryEntry *
SDKDBFile::addObjectiveCCategory(StringRef name, StringRef baseClassName,
                                 bool isPublic,
                                 const AvailabilityInfo &availability) {
  auto entry = ObjCCategoryEntry(name, baseClassName, isPublic, availability);
  CategoriesMapKey catKey(baseClassName, name);
  auto result = categories.emplace(std::make_pair(catKey, entry));
  assert(result.second && "unexpected element in category map");
  return &result.first->second;
}

SDKDBFile::ObjCProtocolEntry *
SDKDBFile::addObjectiveCProtocol(StringRef name, bool isPublic,
                                 const AvailabilityInfo &availability) {
  auto entry = ObjCProtocolEntry(name, isPublic, availability);
  auto result = protocols.emplace(std::make_pair(name, entry));
  assert(result.second && "unexpected element in protocol map");
  return &result.first->second;
}

void SDKDBFile::addObjectiveCMethod(
    SDKDBFile::ObjCContainerEntry *objcContainer, StringRef name,
    bool isInstanceMethod, bool isPublic,
    const AvailabilityInfo &availability) {
  auto entry = MethodEntry(name, isInstanceMethod, isPublic, availability);
  MethodMapKey methodKey(name, isInstanceMethod);
  auto result =
      objcContainer->methods.emplace(std::make_pair(methodKey, entry));
  assert(result.second && "unexpected element in selector map");
  (void)result;
}

void SDKDBFile::addXPISets(const XPISet *declarations,
                           const XPISet *definitions) {
  // Add global symbols.
  for (const auto *sym : definitions->exports()) {
    // Skip symbols that are not global symbols.
    bool isPublic = false;
    AvailabilityInfo availability;
    const auto *declaration = declarations->findSymbol(*sym);
    if (declaration) {
      isPublic = declaration->getAccess() == XPIAccess::Public;
      auto result = mergeAllAvailabilityInfo(
          declaration->getAvailabilityInfo(), declaration);
      if (!result)
        appendError(result.takeError());
      else
        availability = result.get();
    }
    if (const auto *globalSymbol = dyn_cast<GlobalSymbol>(sym)) {
      addGlobalSymbol(globalSymbol->getName(), isPublic, availability);
    } else if (const auto *objcClass = dyn_cast<ObjCClass>(sym)) {
      std::string clsSym = "_OBJC_CLASS_$_" + objcClass->getName().str();
      addGlobalSymbol(clsSym, isPublic, availability);
      std::string metaSym = "_OBJC_METACLASS_$_" + objcClass->getName().str();
      addGlobalSymbol(metaSym, isPublic, availability);
    }
  }

  // Add Objective-C classes.
  for (const auto *sym : definitions->symbols()) {
    const auto *objcClass = dyn_cast<ObjCClass>(sym);
    if (!objcClass)
      continue;

    StringRef superName;
    bool isPublic = false;
    AvailabilityInfo availability;
    if (const auto *superClass = objcClass->getSuperClass())
      superName = superClass->getName();

    const auto *objcClassDecl =
        cast_or_null<ObjCClass>(declarations->findSymbol(*objcClass));
    if (objcClassDecl) {
      isPublic = objcClassDecl->getAccess() == XPIAccess::Public;
      auto result = mergeAllAvailabilityInfo(
          objcClassDecl->getAvailabilityInfo(), objcClassDecl);
      if (!result)
        appendError(result.takeError());
      else
        availability = result.get();
    }

    auto *symbol = addObjectiveCClass(objcClass->getName(), superName, isPublic,
                                      availability);

    for (const auto *selector : objcClass->selectors()) {
      // Search for the declaration.
      const ObjCSelector *selDecl = nullptr;
      if (objcClassDecl)
        selDecl = objcClassDecl->findSelector(selector->getName(),
                                              selector->isInstanceMethod());

      bool isPublic = false;
      AvailabilityInfo availability;
      if (selDecl) {
        isPublic = selDecl->getAccess() == XPIAccess::Public;
        auto result =
            mergeAllAvailabilityInfo(selDecl->getAvailabilityInfo(), selDecl);
        if (!result)
          appendError(result.takeError());
        else
          availability = result.get();
      }

      addObjectiveCMethod(symbol, selector->getName(),
                          selector->isInstanceMethod(), isPublic, availability);
    }
  }

  // Add ObjCCategories.
  for (const auto &sym : definitions->categories()) {
    const auto *objcCategory = sym.second;

    bool isPublic = false;
    AvailabilityInfo availability;
    const auto *objcCategoryDecl =
        cast_or_null<ObjCCategory>(declarations->findCategory(objcCategory));
    if (objcCategoryDecl) {
      isPublic = objcCategoryDecl->getAccess() == XPIAccess::Public;
      auto result = mergeAllAvailabilityInfo(
          objcCategoryDecl->getAvailabilityInfo(), objcCategoryDecl);
      if (!result)
        appendError(result.takeError());
      else
        availability = result.get();
    }

    auto *symbol = addObjectiveCCategory(
        objcCategory->getName(), objcCategory->getBaseClass()->getName(),
        isPublic, availability);

    for (const auto *selector : objcCategory->selectors()) {
      // Search for the declaration.
      const ObjCSelector *selDecl = nullptr;
      if (objcCategoryDecl)
        selDecl = objcCategory->findSelector(selector->getName(),
                                             selector->isInstanceMethod());

      bool isPublic = false;
      AvailabilityInfo availability;
      if (selDecl) {
        isPublic = selDecl->getAccess() == XPIAccess::Public;
        auto result =
            mergeAllAvailabilityInfo(selDecl->getAvailabilityInfo(), selDecl);
        if (!result)
          appendError(result.takeError());
        else
          availability = result.get();
      }

      addObjectiveCMethod(symbol, selector->getName(),
                          selector->isInstanceMethod(), isPublic, availability);
    }
  }

  // Look through categories inside declarations and see if the methods are
  // merged into base class.
  for (const auto &sym : declarations->categories()) {
    auto baseClass = sym.second->getBaseClass();
    auto baseCls = classes.find(baseClass->getName());
    if (baseCls == classes.end())
      continue;

    for (const auto &sel : sym.second->selectors()) {
      auto s = baseCls->second.methods.find(
          {sel->getName(), sel->isInstanceMethod()});
      if (s == baseCls->second.methods.end())
        continue;
      if (sel->getAccess() == XPIAccess::Public)
        s->second.isPublic = true;
      auto availability =
          mergeAllAvailabilityInfo(sel->getAvailabilityInfo(), sel);
      if (!availability)
        appendError(availability.takeError());
      else {
        auto err = mergeAvailabilityInfo(s->second.availability, *availability,
                                         s->second.name);
        if (err)
          appendError(std::move(err));
      }
    }
  }

  // Add Objective-C protocols.
  for (auto &sym : definitions->protocols()) {
    auto objcProtocol = sym.second;
    bool isPublic = false;
    AvailabilityInfo availability;
    const auto *objcProtocolDecl =
        declarations->findProtocol(objcProtocol->getName());
    if (objcProtocolDecl) {
      isPublic = objcProtocolDecl->getAccess() == XPIAccess::Public;
      auto result = mergeAllAvailabilityInfo(
          objcProtocolDecl->getAvailabilityInfo(), objcProtocolDecl);
      if (!result)
        appendError(result.takeError());
      else
        availability = result.get();
    }

    auto *symbol =
        addObjectiveCProtocol(objcProtocol->getName(), isPublic, availability);

    for (const auto *selector : objcProtocol->selectors()) {
      // Search for the declaration.
      const ObjCSelector *selDecl = nullptr;
      if (objcProtocolDecl)
        selDecl = objcProtocolDecl->findSelector(selector->getName(),
                                                 selector->isInstanceMethod());

      bool isPublic = false;
      AvailabilityInfo availability;
      if (selDecl) {
        isPublic = selDecl->getAccess() == XPIAccess::Public;
        auto result =
            mergeAllAvailabilityInfo(selDecl->getAvailabilityInfo(), selDecl);
        if (!result)
          appendError(result.takeError());
        else
          availability = result.get();
      }

      addObjectiveCMethod(symbol, selector->getName(),
                          selector->isInstanceMethod(), isPublic, availability);
    }
  }

  // Find protocols only in declarations.
  for (auto &sym : declarations->protocols()) {
    if (protocols.find(sym.second->getName()) != protocols.end())
      continue;

    auto objcProtocol = sym.second;
    bool isPublic = false;
    AvailabilityInfo availability;
    isPublic = objcProtocol->getAccess() == XPIAccess::Public;
    auto result = mergeAllAvailabilityInfo(
        objcProtocol->getAvailabilityInfo(), objcProtocol);
    if (!result)
      appendError(result.takeError());
    else
      availability = result.get();

    auto *symbol =
        addObjectiveCProtocol(objcProtocol->getName(), isPublic, availability);

    for (const auto *selector : objcProtocol->selectors()) {
      bool isPublic = false;
      AvailabilityInfo availability;
      isPublic = selector->getAccess() == XPIAccess::Public;
      auto result =
          mergeAllAvailabilityInfo(selector->getAvailabilityInfo(), selector);
      if (!result)
        appendError(result.takeError());
      else
        availability = result.get();

      addObjectiveCMethod(symbol, selector->getName(),
                          selector->isInstanceMethod(), isPublic, availability);
    }
  }
}

void SDKDBFile::merge(SDKDBFile &&Other) {
  for (auto &sym : Other.symbols) {
    auto result = symbols.emplace(sym);

    if (!result.second) {
      if (sym.second.isPublic)
        result.first->second.isPublic = true;
    }
    auto err = mergeAvailabilityInfo(result.first->second.availability,
                                     sym.second.availability, sym.second.name);
    if (err)
      appendError(std::move(err));
  }

  for (auto &cls : Other.classes) {
    auto result = classes.emplace(cls);

    if (!result.second) {
      if (cls.second.isPublic)
        result.first->second.isPublic = true;
    }
    auto err = mergeAvailabilityInfo(result.first->second.availability,
                                     cls.second.availability, cls.second.name);
    if (err)
      appendError(std::move(err));

    for (auto &s : cls.second.methods) {
      auto s_result = result.first->second.methods.emplace(s);
      if (!s_result.second) {
        if (s.second.isPublic)
          s_result.first->second.isPublic = true;
      }

      auto err = mergeAvailabilityInfo(s_result.first->second.availability,
                                       s.second.availability, s.second.name);
      if (err)
        appendError(std::move(err));
    }
  }

  for (auto &proto : Other.protocols) {
    auto result = protocols.emplace(proto);

    if (!result.second) {
      if (proto.second.isPublic)
        result.first->second.isPublic = true;
    }
    auto err =
        mergeAvailabilityInfo(result.first->second.availability,
                              proto.second.availability, proto.second.name);
    if (err)
      appendError(std::move(err));

    for (auto &s : proto.second.methods) {
      auto s_result = result.first->second.methods.emplace(s);
      if (!s_result.second) {
        if (s.second.isPublic)
          s_result.first->second.isPublic = true;
      }

      auto err = mergeAvailabilityInfo(s_result.first->second.availability,
                                       s.second.availability, s.second.name);
      if (err)
        appendError(std::move(err));
    }
  }

  for (auto &cat : Other.categories) {
    auto result = categories.emplace(cat);

    if (!result.second) {
      if (cat.second.isPublic)
        result.first->second.isPublic = true;
    }
    auto err = mergeAvailabilityInfo(result.first->second.availability,
                                     cat.second.availability, cat.second.name);
    if (err)
      appendError(std::move(err));

    for (auto &s : cat.second.methods) {
      auto s_result = result.first->second.methods.emplace(s);
      if (!s_result.second) {
        if (s.second.isPublic)
          s_result.first->second.isPublic = true;
      }

      auto err = mergeAvailabilityInfo(s_result.first->second.availability,
                                       s.second.availability, s.second.name);
      if (err)
        appendError(std::move(err));
    }
  }

  appendError(std::move(Other.error));
}

std::string SDKDBFile::InfoEntry::toString() const {
  std::string str;
  llvm::raw_string_ostream os(str);
  os << name;
  if (isPublic) {
    os << " (Public) ";
    availability.print(os);
  } else
    os << " (Private) ";

  return str;
}

LLVM_DUMP_METHOD void SDKDBFile::dump(raw_ostream &os) const {
  os << "Install Name: " << installName << "\n";

  os << "Symbols:\n";
  for (const auto &sym : symbols)
    os << "\t" << sym.second.toString() << "\n";

  os << "ObjC Classes:\n";
  for (const auto &cls : classes) {
    os << "--> " << cls.second.toString() << "\n";
    os << "\tSuperClass: " << cls.second.superClassName << "\n";
    os << "\tInstanceMethods:\n";
    for (const auto &m : cls.second.methods) {
      if (!m.second.isInstanceMethod)
        continue;
      os << "\t\t" << m.second.toString() << "\n";
    }
    os << "\tClassMethods:\n";
    for (const auto &m : cls.second.methods) {
      if (m.second.isInstanceMethod)
        continue;
      os << "\t\t" << m.second.toString() << "\n";
    }
  }

  os << "ObjC Categories:\n";
  for (const auto &cat : categories) {
    os << "--> " << cat.second.toString() << "\n";
    os << "\tBaseClass: " << cat.second.baseClassName << "\n";
    os << "\tInstanceMethods:\n";
    for (const auto &m : cat.second.methods) {
      if (!m.second.isInstanceMethod)
        continue;
      os << "\t\t" << m.second.toString() << "\n";
    }
    os << "\tClassMethods:\n";
    for (const auto &m : cat.second.methods) {
      if (m.second.isInstanceMethod)
        continue;
      os << "\t\t" << m.second.toString() << "\n";
    }
  }

  os << "ObjC Protocols:\n";
  for (const auto &p : protocols) {
    os << "--> " << p.second.toString() << "\n";
    os << "\tInstanceMethods:\n";
    for (const auto &m : p.second.methods) {
      if (!m.second.isInstanceMethod)
        continue;
      os << "\t\t" << m.second.toString() << "\n";
    }
    os << "\tClassMethods:\n";
    for (const auto &m : p.second.methods) {
      if (m.second.isInstanceMethod)
        continue;
      os << "\t\t" << m.second.toString() << "\n";
    }
  }
}

Error SDKDBFile::verifySDKDBFile(const SDKDBFile *baseline) const {
  // Verify that no APIs are removed from baseline.
  Error errors = Error::success();
  auto addNewError = [&](Error &&Err) {
    errors = joinErrors(std::move(errors), std::move(Err));
  };
  auto diagnoseAvailability = [&](const AvailabilityInfo &A,
                                  const AvailabilityInfo &B, StringRef Name) {
    if (A == B)
      return;

    // If A has no availability, it is an improvement in SDK availability.
    if (A.isDefault())
      return;

    // Moving from unavailable to available is fine.
    if (A._unavailable)
      return;

    std::string ErrStr;
    llvm::raw_string_ostream os(ErrStr);

    os << "AvailabilityInfo is different for " << Name << ": " << A << " and "
       << B;

    addNewError(make_error<StringError>(
        os.str(),
        std::make_error_code(std::errc::not_supported)));
  };

  auto diagnoseInfoEntry = [&](const InfoEntry &A, const InfoEntry &B,
                               const InfoEntry *context = nullptr) {
    assert(A.name == B.name && "Name must be equal");
    std::string name = A.name;
    if (context)
      name += " (" + context->name + ")";
    if (A.isPublic && !B.isPublic) {
      addNewError(make_error<StringError>(
          "API " + name + " becomes SPI",
          std::make_error_code(std::errc::not_supported)));
      return;
    }
    diagnoseAvailability(A.availability, B.availability, name);
  };

  // Check C symbols.
  for (auto &sym : baseline->symbols) {
    if (!sym.second.isPublic)
      continue;
    // Check if the API is missing first.
    auto s = symbols.find(sym.first);
    if (s == symbols.end()) {
      // It is ok to remove unavailable symbols.
      if (sym.second.isAvailable())
        addNewError(make_error<StringError>(
            "missing C Symbol " + sym.first,
            std::make_error_code(std::errc::not_supported)));
      continue;
    }
    // Check if the API becomes SPI.
    diagnoseInfoEntry(sym.second, s->second);
  }

  // Check ObjC Class.
  for (auto &cls : baseline->classes) {
    if (!cls.second.isPublic)
      continue;
    auto c = classes.find(cls.first);
    if (c == classes.end()) {
      if (cls.second.isAvailable())
        addNewError(make_error<StringError>(
            "missing ObjC Class " + cls.first,
            std::make_error_code(std::errc::not_supported)));
      continue;
    }
    diagnoseInfoEntry(cls.second, c->second);
    // Check selectors.
    for (auto &sel : cls.second.methods) {
      if (!sel.second.isPublic)
        continue;
      auto s = c->second.methods.find(sel.first);
      if (s == c->second.methods.end()) {
        if (sel.second.isAvailable())
          addNewError(make_error<StringError>(
              "missing selector " + sel.second.name + " (" + cls.first + ")",
              std::make_error_code(std::errc::not_supported)));
        continue;
      }
      diagnoseInfoEntry(sel.second, s->second, &cls.second);
    }
  }

  // Check ObjC Category.
  for (auto &cat : baseline->categories) {
    if (!cat.second.isPublic)
      continue;
    auto c = categories.find(cat.first);
    if (c == categories.end()) {
      if (cat.second.isAvailable())
        addNewError(make_error<StringError>(
            "missing ObjC Category " + cat.second.name + " (" +
                cat.second.baseClassName + ")",
            std::make_error_code(std::errc::not_supported)));
      continue;
    }
    diagnoseInfoEntry(cat.second, c->second);
    // Check selectors.
    for (auto &sel : cat.second.methods) {
      if (!sel.second.isPublic)
        continue;
      auto s = c->second.methods.find(sel.first);
      if (s == c->second.methods.end()) {
        if (sel.second.isAvailable())
          addNewError(make_error<StringError>(
              "missing selector " + sel.second.name + " (" +
                  cat.second.name + ")",
              std::make_error_code(std::errc::not_supported)));
        continue;
      }
      diagnoseInfoEntry(sel.second, s->second, &cat.second);
    }
  }

  // Check ObjC Protocol.
  for (auto &protocol : baseline->protocols) {
    if (!protocol.second.isPublic)
      continue;
    auto p = protocols.find(protocol.first);
    if (p == protocols.end()) {
      if (protocol.second.isAvailable())
        addNewError(make_error<StringError>(
            "missing ObjC Protocol " + protocol.first,
            std::make_error_code(std::errc::not_supported)));
      continue;
    }
    diagnoseInfoEntry(protocol.second, p->second);
    // Check selectors.
    for (auto &sel : protocol.second.methods) {
      if (!sel.second.isPublic)
        continue;
      auto s = p->second.methods.find(sel.first);
      if (s == p->second.methods.end()) {
        if (sel.second.isAvailable())
          addNewError(make_error<StringError>(
              "missing selector " + sel.second.name + " (" + protocol.first + ")",
              std::make_error_code(std::errc::not_supported)));
        continue;
      }
      diagnoseInfoEntry(sel.second, s->second, &protocol.second);
    }
  }

  return errors;
}

void SDKDBFile::categoryMerge() {
  SmallVector<CategoriesMapKey, 4> MergedCategories;
  for (auto &cat : categories) {
    auto baseCls = classes.find(cat.second.baseClassName);
    if (baseCls == classes.end())
      continue;
    // Merge the category info with class info.
    for (auto &sel : cat.second.methods) {
      auto s = baseCls->second.methods.insert(sel);
      if (!s.second) {
        if (sel.second.isPublic)
          s.first->second.isPublic = true;
      }

      // If availability is defined in the base class, it can't be overritten.
      if (s.first->second.availability.isDefault()) {
        auto err =
            mergeAvailabilityInfo(s.first->second.availability,
                                  sel.second.availability, sel.second.name);
        if (err)
          appendError(std::move(err));
      }
    }
    MergedCategories.emplace_back(cat.first);
  }
  for (auto &cat : MergedCategories)
    categories.erase(cat);
}

TAPI_NAMESPACE_INTERNAL_END
