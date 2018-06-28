//===- lib/Scanner/APIScanner.cpp - TAPI API Scanner ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the TAPI API Scanner.
///
//===----------------------------------------------------------------------===//

#include "tapi/Scanner/APIScanner.h"
#include "tapi/Core/AvailabilityInfo.h"
#include "tapi/Core/HeaderFile.h"
#include "tapi/Core/XPI.h"
#include "tapi/Core/XPISet.h"
#include "tapi/Defines.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclBase.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclObjC.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/Linkage.h"
#include "clang/Basic/ObjCRuntime.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/Specifiers.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/Visibility.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/iterator_range.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include <string>
#include <utility>

using namespace llvm;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

// \brief Check if the interface itself or any of its super classes have an
// exception attribute.
//
// We need to export an additonal symbol ("OBJC_EHTYPE_$CLASS_NAME") if any of
// the classes have the exception attribute.
static bool hasObjCExceptionAttribute(const ObjCInterfaceDecl *decl) {
  for (; decl != nullptr; decl = decl->getSuperClass())
    if (decl->hasAttr<ObjCExceptionAttr>())
      return true;

  return false;
}

// \brief Check if the functiom is inlined.
static bool isInlined(const ASTContext &context, const NamedDecl *decl) {
  auto *func = dyn_cast<FunctionDecl>(decl);
  if (func == nullptr)
    return false;

  // Check all redeclarations to find the inline attribute / keyword.
  bool hasInlineAttribute = false;
  for (const auto *decl : func->redecls()) {
    if (decl->isInlined()) {
      hasInlineAttribute = true;
      break;
    }
  }
  if (!hasInlineAttribute)
    return false;

  if ((!context.getLangOpts().CPlusPlus &&
       !context.getTargetInfo().getCXXABI().isMicrosoft() &&
       !func->hasAttr<DLLExportAttr>()) ||
      func->hasAttr<GNUInlineAttr>()) {
    if (func->doesThisDeclarationHaveABody() &&
        func->isInlineDefinitionExternallyVisible())
      return false;
  }

  return true;
}

static bool hasVTable(ASTContext &context, const CXXRecordDecl *decl) {
  // Check if we need need to emit the vtable symbol. Only dynamic classes need
  // vtables - except the class is abstract.
  if (!decl->hasDefinition() || !decl->isDynamicClass() || decl->isAbstract())
    return false;

  assert(decl->isExternallyVisible() && "should be externally visible");
  assert(decl->isCompleteDefinition() && "only work on complete definitions");

  const CXXMethodDecl *keyFunction = context.getCurrentKeyFunction(decl);
  // If this class has a key function, then we have a vtable (might be internal
  // only).
  if (keyFunction) {
    switch (keyFunction->getTemplateSpecializationKind()) {
    case TSK_Undeclared:
    case TSK_ExplicitSpecialization:
    case TSK_ImplicitInstantiation:
    case TSK_ExplicitInstantiationDefinition:
      return true;
    case TSK_ExplicitInstantiationDeclaration:
      llvm_unreachable("Should not have been asked to emit this");
    }
  }

  switch (decl->getTemplateSpecializationKind()) {
  case TSK_Undeclared:
  case TSK_ExplicitSpecialization:
  case TSK_ImplicitInstantiation:
    return false;

  case TSK_ExplicitInstantiationDeclaration:
  case TSK_ExplicitInstantiationDefinition:
    return true;
  }

  llvm_unreachable("Invalid TemplateSpecializationKind!");
}

static llvm::GlobalVariable::LinkageTypes
getVTableLinkage(ASTContext &context, const CXXRecordDecl *decl) {
  assert((decl->hasDefinition() && decl->isDynamicClass() &&
          !decl->isAbstract()) &&
         "no vtable");
  assert(decl->isExternallyVisible() && "should be externally visible");

  const CXXMethodDecl *keyFunction = context.getCurrentKeyFunction(decl);
  if (keyFunction) {
    // If this class has a key function, use that to determine the
    // linkage of the vtable.
    switch (keyFunction->getTemplateSpecializationKind()) {
    case TSK_Undeclared:
    case TSK_ExplicitSpecialization:
      if (isInlined(context, keyFunction))
        return llvm::GlobalVariable::LinkOnceODRLinkage;
      return llvm::GlobalVariable::ExternalLinkage;
    case TSK_ImplicitInstantiation:
      llvm_unreachable("no external vtable for implicit instantiation");
    case TSK_ExplicitInstantiationDefinition:
      return llvm::GlobalVariable::WeakODRLinkage;
    case TSK_ExplicitInstantiationDeclaration:
      llvm_unreachable("Should not have been asked to emit this");
    }
  }

  switch (decl->getTemplateSpecializationKind()) {
  case TSK_Undeclared:
  case TSK_ExplicitSpecialization:
  case TSK_ImplicitInstantiation:
    return llvm::GlobalValue::LinkOnceODRLinkage;
  case TSK_ExplicitInstantiationDeclaration:
  case TSK_ExplicitInstantiationDefinition:
    return llvm::GlobalValue::WeakODRLinkage;
  }

  llvm_unreachable("Invalid TemplateSpecializationKind!");
}

static bool isRTTIWeakDef(ASTContext &context, const CXXRecordDecl *decl) {
  if (decl->hasAttr<WeakAttr>())
    return true;

  if (decl->isAbstract())
    return true;

  if (decl->isDynamicClass())
    return getVTableLinkage(context, decl) !=
           llvm::GlobalVariable::ExternalLinkage;

  return false;
}

static bool hasRTTI(ASTContext &context, const CXXRecordDecl *decl) {
  if (!context.getLangOpts().RTTI)
    return false;

  if (!decl->hasDefinition())
    return false;

  if (!decl->isDynamicClass())
    return false;

  // Don't emit weak-def RTTI information. We cannot reliably determine if the
  // final binary will have those weak defined RTTI symbols. This depends on the
  // optimization level and if the class has been instantiated and used.
  //
  // Luckily the static linker doesn't need those weak defined RTTI symbols for
  // linking. They are only needed by the runtime linker. That means we can
  // safely drop all of them.
  if (isRTTIWeakDef(context, decl))
    return false;

  return true;
}

static bool isExported(const NamedDecl *decl) {
  auto li = decl->getLinkageAndVisibility();
  return isExternallyVisible(li.getLinkage()) &&
         (li.getVisibility() == DefaultVisibility);
}

APIScanner::APIScanner(ASTContext &context, ParseContext &parse)
    : _context(context), _sourceManager(context.getSourceManager()),
      _mc(ItaniumMangleContext::create(context, context.getDiagnostics())),
      _dl(context.getTargetInfo().getDataLayout()), _parse(parse),
      _isObjCFragile(context.getLangOpts().ObjCRuntime.isFragile()) {}

void APIScanner::HandleTranslationUnit(ASTContext &context) {
  if (context.getDiagnostics().hasErrorOccurred())
    return;

  auto *d = context.getTranslationUnitDecl();
  TraverseDecl(d);
}

AvailabilityInfo APIScanner::getAvailabilityInfo(const NamedDecl *decl) const {
  auto platformName = _context.getTargetInfo().getPlatformName();

  AvailabilityInfo availability;
  for (const auto *A : decl->specific_attrs<AvailabilityAttr>()) {
    if (A->getPlatform()->getName() != platformName)
      continue;

    availability = AvailabilityInfo(A->getIntroduced(), A->getObsoleted(),
                                    A->getUnavailable());
    break;
  }

  if (const auto *attr = decl->getAttr<UnavailableAttr>())
    if (!attr->isImplicit())
      availability._unavailable = true;

  // Return default availability.
  return availability;
}

std::pair<XPIAccess, PresumedLoc>
APIScanner::getFileAttributesForDecl(const NamedDecl *decl) const {
  auto access = XPIAccess::Unknown;
  auto presumedLoc = PresumedLoc();

  auto loc = decl->getLocation();
  if (loc.isInvalid())
    return std::make_pair(access, presumedLoc);

  // If the loc refers to a macro expansion we need to first get the file
  // location of the expansion.
  auto fileLoc = _sourceManager.getFileLoc(loc);
  FileID ID = _sourceManager.getFileID(fileLoc);
  if (ID.isInvalid())
    return std::make_pair(access, presumedLoc);

  const auto *file = _sourceManager.getFileEntryForID(ID);
  if (!file)
    return std::make_pair(access, presumedLoc);

  presumedLoc = _sourceManager.getPresumedLoc(loc);

  auto H = _parse.files.find(file);
  if (H == _parse.files.end())
    return std::make_pair(access, presumedLoc);

  access = (H->second == HeaderType::Public) ? XPIAccess::Public
                                             : XPIAccess::Private;

  return std::make_pair(access, presumedLoc);
}

void APIScanner::recordObjCSelectors(
    ObjCContainer *objcContainer, const ObjCContainerDecl::method_range methods,
    bool isDynamic) {
  for (const auto *method : methods) {
    XPIAccess access;
    PresumedLoc loc;
    std::tie(access, loc) = getFileAttributesForDecl(method);
    assert(access != XPIAccess::Unknown && "Unexpected access value");

    auto name = method->getSelector().getAsString();
    auto info = getAvailabilityInfo(method);
    _parse.xpi->addObjCSelector(objcContainer, name, method->isInstanceMethod(),
                                isDynamic, loc, access, _parse.arch, info);
  }
}

void APIScanner::recordObjCInstanceVariables(
    const iterator_range<DeclContext::specific_decl_iterator<ObjCIvarDecl>>
        ivars) {
  for (const auto *ivar : ivars) {
    // ObjC has an additional mechanism to specify if an ivar is exported or
    // not.
    if (ivar->getCanonicalAccessControl() == ObjCIvarDecl::Private ||
        ivar->getCanonicalAccessControl() == ObjCIvarDecl::Package)
      return;

    XPIAccess access;
    PresumedLoc loc;
    std::tie(access, loc) = getFileAttributesForDecl(ivar);
    assert(access != XPIAccess::Unknown && "Unexpected access value");

    auto name = getMangledName(ivar);
    auto info = getAvailabilityInfo(ivar);
    _parse.xpi->addObjCInstanceVariable(name, loc, access, _parse.arch, info);
  }
}

/// \brief Visit all Objective-C Interface declarations.
///
/// Every Objective-C class has an interface declaration that lists all the
/// ivars, properties, and methods of the class.
///
bool APIScanner::VisitObjCInterfaceDecl(ObjCInterfaceDecl *interface) {
  // Skip forward declaration for classes (@class)
  if (!interface->isThisDeclarationADefinition())
    return true;

  // Check if the interface is defined in one of the header files we care about.
  XPIAccess access;
  PresumedLoc loc;
  std::tie(access, loc) = getFileAttributesForDecl(interface);
  if (access == XPIAccess::Unknown)
    return true;

  if (loc.isInvalid())
    return true;

  // When the interface is not exported, then there are no linkable symbols
  // exported from the library. The Objective-C metadata for the class and
  // selectors on the other hand are always recorded.
  if (!isExported(interface))
    access = XPIAccess::Internal;

  ///
  /// 1. Get super class.
  ///
  ObjCClass *objcSuperClass = nullptr;
  auto *superClass = interface->getSuperClass();
  if (superClass) {
    PresumedLoc superClassLoc;
    std::tie(std::ignore, superClassLoc) = getFileAttributesForDecl(superClass);
    if (superClassLoc.isInvalid())
      return true;

    auto superClassName = getMangledName(superClass);
    auto superClassInfo = getAvailabilityInfo(superClass);
    objcSuperClass = _parse.xpi->addObjCClass(superClassName, superClassLoc,
                                              XPIAccess::Unknown, _parse.arch,
                                              superClassInfo);
  }

  ///
  /// 2. Record the ObjC Class
  ///
  auto name = getMangledName(interface);
  auto info = getAvailabilityInfo(interface);
  auto *objcClass = _parse.xpi->addObjCClass(name, loc, access, _parse.arch,
                                             info, objcSuperClass);

  // Check if we need to record an additional objc exception class.
  if (!_isObjCFragile && hasObjCExceptionAttribute(interface)) {
    auto name = getMangledName(interface);
    _parse.xpi->addObjCClassEHType(name, loc, access, _parse.arch, info);
  }

  ///
  /// 3. Record all methods (selectors). This includes automatically synthesized
  ///    property methods.
  ///
  recordObjCSelectors(objcClass, interface->methods());

  ///
  /// 4. Record all ivars.
  ///

  // The old ObjC ABI doesn't record the ivar symbols.
  if (_isObjCFragile)
    return true;

  recordObjCInstanceVariables(interface->ivars());

  // We don't record the categories here. This will be done by
  // VisitObjCCategoryDecl.

  return true;
}

/// \brief Visit all Objective-C Category/Extension declarations.
///
/// Objective-C classes may have category or extension declarations that list
/// additional ivars, properties, and methods for the class.
///
/// The class that is being extended might come from a different framework and
/// is therefore itself not recorded.
///
bool APIScanner::VisitObjCCategoryDecl(ObjCCategoryDecl *category) {
  // Check if the category is defined in one of the header files we care about.
  XPIAccess access;
  PresumedLoc loc, interfaceLoc;
  std::tie(access, loc) = getFileAttributesForDecl(category);
  if (access == XPIAccess::Unknown)
    return true;

  auto name = category->getName();

  // Get the interface decl.
  auto *interface = category->getClassInterface();
  std::tie(std::ignore, interfaceLoc) = getFileAttributesForDecl(interface);
  if (interfaceLoc.isInvalid())
    return true;

  auto interfaceName = getMangledName(interface);
  auto interfaceInfo = getAvailabilityInfo(interface);
  auto *baseClass =
      _parse.xpi->addObjCClass(interfaceName, interfaceLoc, XPIAccess::Unknown,
                               _parse.arch, interfaceInfo);

  auto *objcCategory = _parse.xpi->addObjCCategory(baseClass, name, loc, access,
                                                   _parse.arch, interfaceInfo);

  ///
  /// 1. Record all methods (selectors). This includes automatically synthesized
  ///    property methods.
  ///
  bool isDynamic = name == "CoreDataGeneratedAccessors";
  recordObjCSelectors(objcCategory, category->methods(), isDynamic);

  ///
  /// 2. Record all ivars.
  ///
  // The old ObjC ABI doesn't record the ivar symbols.
  if (_isObjCFragile)
    return true;

  recordObjCInstanceVariables(category->ivars());

  return true;
}

/// \brief Visit all Objective-C Protocol declarations.
bool APIScanner::VisitObjCProtocolDecl(ObjCProtocolDecl *protocol) {
  // Skip forward declaration for protocols (@protocol).
  if (!protocol->isThisDeclarationADefinition())
    return true;

  // Check if the protocol is defined in one of the header files we care about.
  XPIAccess access;
  PresumedLoc loc;
  std::tie(access, loc) = getFileAttributesForDecl(protocol);
  if (access == XPIAccess::Unknown)
    return true;

  auto name = protocol->getName();
  auto info = getAvailabilityInfo(protocol);
  auto *objcProtocol =
      _parse.xpi->addObjCProtocol(name, loc, access, _parse.arch, info);

  ///
  /// Record all methods (selectors).
  ///
  recordObjCSelectors(objcProtocol, protocol->methods());

  return true;
}

bool APIScanner::VisitCXXRecordDecl(CXXRecordDecl *record) {
  if (!record->isCompleteDefinition())
    return true;

  if (!isExported(record))
    return true;

  // Skip templated classes.
  if (record->getDescribedClassTemplate() != nullptr)
    return true;

  // Skip partial templated classes too.
  if (isa<ClassTemplatePartialSpecializationDecl>(record))
    return true;

  XPIAccess access;
  PresumedLoc loc;
  std::tie(access, loc) = getFileAttributesForDecl(record);
  if (access == XPIAccess::Unknown)
    return true;

  auto info = getAvailabilityInfo(record);

  // Check if we need to emit the vtable/rtti symbols.
  emitVTableSymbols(record, loc, access, info);

  auto classSpecializationKind = TSK_Undeclared;
  bool keepInlineAsWeak = false;
  if (auto *decl = dyn_cast<ClassTemplateSpecializationDecl>(record)) {
    classSpecializationKind = decl->getTemplateSpecializationKind();
    if (classSpecializationKind == TSK_ExplicitSpecialization ||
        classSpecializationKind == TSK_ExplicitInstantiationDeclaration)
      keepInlineAsWeak = true;
  }

  // Record the class methods.
  for (const auto *method : record->methods()) {
    // Inlined methods are usually not emitted - except it comes from a
    // specialized template.
    bool isWeakDef = false;
    if (isInlined(_context, method)) {
      if (!keepInlineAsWeak)
        continue;

      isWeakDef = true;
    }

    switch (method->getTemplateSpecializationKind()) {
    case TSK_Undeclared:
      break;
    case TSK_ImplicitInstantiation:
      continue;
    case TSK_ExplicitSpecialization:
      isWeakDef = true;
      break;
    case TSK_ExplicitInstantiationDeclaration:
      if (classSpecializationKind == TSK_ExplicitInstantiationDeclaration)
        isWeakDef = true;
      break;
    case TSK_ExplicitInstantiationDefinition:
      isWeakDef = true;
      break;
    }

    if (!method->isUserProvided())
      continue;

    // Methods that are deleted are not exported.
    if (method->isDeleted())
      continue;

    // Abstract methods aren't exported either.
    if (method->isPure())
      continue;

    XPIAccess access;
    PresumedLoc loc;
    std::tie(access, loc) = getFileAttributesForDecl(method);
    if (access == XPIAccess::Unknown)
      continue;

    auto info = getAvailabilityInfo(method);

    if (const auto *ctor = dyn_cast<CXXConstructorDecl>(method)) {
      // Defaulted constructors are not exported.
      if (ctor->isDefaulted())
        continue;

      auto name = getMangledName(method, Ctor_Base);
      _parse.xpi->addGlobalSymbol(name, loc, access, _parse.arch, info,
                                  isWeakDef);

      if (!record->isAbstract()) {
        auto name = getMangledName(method, Ctor_Complete);
        _parse.xpi->addGlobalSymbol(name, loc, access, _parse.arch, info,
                                    isWeakDef);
      }

      continue;
    }

    if (const auto *dtor = dyn_cast<CXXDestructorDecl>(method)) {
      // Defaulted destructors are not exported.
      if (dtor->isDefaulted())
        continue;

      auto name = getMangledName(method, Dtor_Base);
      _parse.xpi->addGlobalSymbol(name, loc, access, _parse.arch, info,
                                  isWeakDef);

      name = getMangledName(method, Dtor_Complete);
      _parse.xpi->addGlobalSymbol(name, loc, access, _parse.arch, info,
                                  isWeakDef);

      if (dtor->isVirtual()) {
        auto name = getMangledName(method, Dtor_Deleting);
        _parse.xpi->addGlobalSymbol(name, loc, access, _parse.arch, info,
                                    isWeakDef);
      }

      continue;
    }

    auto name = getMangledName(method);
    _parse.xpi->addGlobalSymbol(name, loc, access, _parse.arch, info,
                                isWeakDef);
  }

  if (auto *decl = dyn_cast<ClassTemplateSpecializationDecl>(record)) {
    if (!decl->isExplicitInstantiationOrSpecialization())
      return true;
  }

  using var_iter = CXXRecordDecl::specific_decl_iterator<VarDecl>;
  using var_range = llvm::iterator_range<var_iter>;
  for (auto *var : var_range(record->decls())) {
    // Skip const static member variables.
    // \code
    // struct S {
    //   static const int x = 0;
    // };
    // \endcode
    if (var->isStaticDataMember() && var->hasInit())
      continue;

    // Check if the interface is defined in one of the header files we care
    // about.
    XPIAccess access;
    PresumedLoc loc;
    std::tie(access, loc) = getFileAttributesForDecl(var);
    if (access == XPIAccess::Unknown)
      return true;

    auto name = getMangledName(var);
    auto info = getAvailabilityInfo(var);
    bool isWeakDef = var->hasAttr<WeakAttr>() || keepInlineAsWeak;
    _parse.xpi->addGlobalSymbol(name, loc, access, _parse.arch, info,
                                isWeakDef);
  }

  return true;
}

bool APIScanner::VisitFunctionDecl(FunctionDecl *func) {
  // Skip methods in records. They are already handled in VisitCXXRecordDecl.
  if (isa<CXXMethodDecl>(func))
    return true;

  if (!isExported(func))
    return true;

  // Skip templated functions.
  switch (func->getTemplatedKind()) {
  case FunctionDecl::TK_NonTemplate:
  case FunctionDecl::TK_FunctionTemplateSpecialization:
    break;
  case FunctionDecl::TK_FunctionTemplate:
  case FunctionDecl::TK_MemberSpecialization:
  case FunctionDecl::TK_DependentFunctionTemplateSpecialization:
    return true;
  }

  // Skip inlined functions.
  if (isInlined(_context, func))
    return true;

  // Check if the interface is defined in one of the header files we care about.
  XPIAccess access;
  PresumedLoc loc;
  std::tie(access, loc) = getFileAttributesForDecl(func);
  if (access == XPIAccess::Unknown)
    return true;

  auto name = getMangledName(func);
  auto info = getAvailabilityInfo(func);
  bool isExplicitInstantiation = func->getTemplateSpecializationKind() ==
                                 TSK_ExplicitInstantiationDeclaration;
  bool isWeakDef = isExplicitInstantiation || func->hasAttr<WeakAttr>();
  _parse.xpi->addGlobalSymbol(name, loc, access, _parse.arch, info, isWeakDef);

  return true;
}

bool APIScanner::VisitVarDecl(VarDecl *var) {
  // Skip variables in records. They are already handled in VisitCXXRecordDecl.
  if (var->getDeclContext()->isRecord())
    return true;

  if (!isExported(var))
    return true;

  // Skip const static function variables.
  if (var->isStaticLocal() && var->hasInit())
    return true;

  // Check if the interface is defined in one of the header files we care about.
  XPIAccess access;
  PresumedLoc loc;
  std::tie(access, loc) = getFileAttributesForDecl(var);
  if (access == XPIAccess::Unknown)
    return true;

  auto name = getMangledName(var);
  auto info = getAvailabilityInfo(var);
  bool isWeakDef = var->hasAttr<WeakAttr>();
  _parse.xpi->addGlobalSymbol(name, loc, access, _parse.arch, info, isWeakDef);

  return true;
}

void APIScanner::emitVTableSymbols(const CXXRecordDecl *record, PresumedLoc loc,
                                   XPIAccess access, AvailabilityInfo info,
                                   bool emittedVTable) {
  if (hasVTable(_context, record)) {
    emittedVTable = true;
    auto vtableLinkage = getVTableLinkage(_context, record);
    if (vtableLinkage == llvm::GlobalVariable::ExternalLinkage ||
        vtableLinkage == llvm::GlobalVariable::WeakODRLinkage) {
      auto name = getMangledCXXVTableName(record);
      bool isWeakDef = vtableLinkage == llvm::GlobalVariable::WeakODRLinkage;
      _parse.xpi->addGlobalSymbol(name, loc, access, _parse.arch, info,
                                  isWeakDef);

      if (!record->getDescribedClassTemplate() && !record->isInvalidDecl()) {
        auto vtable = _context.getVTableContext();
        for (auto *method : record->methods()) {
          if (isa<CXXConstructorDecl>(method) || isa<CXXDestructorDecl>(method))
            continue;

          if (!method->isVirtual())
            continue;

          auto *thunks = vtable->getThunkInfo(method);
          if (!thunks)
            continue;

          auto info = getAvailabilityInfo(method);
          for (auto &thunk : *thunks) {
            auto name = getMangledCXXThunk(method, thunk);
            _parse.xpi->addGlobalSymbol(name, loc, access, _parse.arch, info);
          }
        }
      }
    }
  }

  if (!emittedVTable)
    return;

  if (hasRTTI(_context, record)) {
    auto name = getMangledCXXRTTI(record);
    _parse.xpi->addGlobalSymbol(name, loc, access, _parse.arch, info);

    name = getMangledCXXRTTIName(record);
    _parse.xpi->addGlobalSymbol(name, loc, access, _parse.arch, info);
  }

  for (auto &I : record->bases()) {
    const CXXRecordDecl *base =
        cast<CXXRecordDecl>(I.getType()->castAs<RecordType>()->getDecl());

    XPIAccess baseAccess;
    PresumedLoc baseLoc;
    std::tie(baseAccess, baseLoc) = getFileAttributesForDecl(base);
    if (baseAccess == XPIAccess::Unknown)
      continue;
    auto baseInfo = getAvailabilityInfo(base);

    emitVTableSymbols(base, baseLoc, baseAccess, baseInfo, true);
  }
}

std::string APIScanner::getMangledName(const NamedDecl *decl, Flags flags) {
  assert((isa<FunctionDecl>(decl) || isa<VarDecl>(decl) ||
          isa<ObjCIvarDecl>(decl) || isa<ObjCInterfaceDecl>(decl)) &&
         "Unexpected decl type.");

  SmallString<128> name;
  if (auto *ivar = dyn_cast<ObjCIvarDecl>(decl)) {
    const auto *container = ivar->getContainingInterface();
    name += container->getObjCRuntimeNameAsString();
    name += ".";
    name += ivar->getName();
    return name.str();
  }

  if (auto *interface = dyn_cast<ObjCInterfaceDecl>(decl))
    return interface->getObjCRuntimeNameAsString();

  if (_mc->shouldMangleDeclName(decl)) {
    raw_svector_ostream nameStream(name);
    if (const auto *ctor = dyn_cast<CXXConstructorDecl>(decl))
      _mc->mangleCXXCtor(ctor, flags.getCtorType(), nameStream);
    else if (const auto *dtor = dyn_cast<CXXDestructorDecl>(decl))
      _mc->mangleCXXDtor(dtor, flags.getDtorType(), nameStream);
    else
      _mc->mangleName(decl, nameStream);
  } else
    name += decl->getNameAsString();

  return getBackendMangledName(name);
}

TAPI_NAMESPACE_INTERNAL_END
