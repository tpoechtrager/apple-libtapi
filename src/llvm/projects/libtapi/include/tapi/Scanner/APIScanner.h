//===- tapi/Scanner/APIScanner.h - TAPI API Scanner -------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the TAPI API Scanner.
///
//===----------------------------------------------------------------------===//
#ifndef TAPI_SCANNER_API_SCANNER_H
#define TAPI_SCANNER_API_SCANNER_H

#include "tapi/Core/XPI.h"
#include "tapi/Defines.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Mangle.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/VTableBuilder.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Mangler.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

using clang::ASTContext;
using clang::ASTConsumer;
using clang::CXXRecordDecl;
using clang::FunctionDecl;
using clang::ObjCContainerDecl;
using clang::ObjCInterfaceDecl;
using clang::ObjCIvarDecl;
using clang::ObjCCategoryDecl;
using clang::ObjCProtocolDecl;
using clang::VarDecl;
using clang::NamedDecl;
using clang::CXXRecordDecl;
using clang::DeclContext;
using clang::PresumedLoc;
using llvm::iterator_range;
using clang::CompilerInstance;
using clang::ASTFrontendAction;
using clang::RecursiveASTVisitor;
using clang::CXXMethodDecl;
using clang::ThunkInfo;
using clang::ItaniumMangleContext;

enum class HeaderType;

struct ParseContext {
  std::map<const clang::FileEntry *, HeaderType> files;
  XPISet *xpi;
  Architecture arch;
  int ReturnValue;
};

class APIScanner : public ASTConsumer, public RecursiveASTVisitor<APIScanner> {
  class Flags {
    struct S {
      unsigned _ctor : 8;
      unsigned _dtor : 8;
      S() : _ctor(0), _dtor(0) {}
      S(clang::CXXCtorType ctor) : _ctor(ctor), _dtor(0) {}
      S(clang::CXXDtorType dtor) : _ctor(0), _dtor(dtor) {}
    } _s;

  public:
    Flags() : _s() {}
    Flags(clang::CXXCtorType ctor) : _s(ctor) {}
    Flags(clang::CXXDtorType dtor) : _s(dtor) {}

    clang::CXXCtorType getCtorType() const {
      return (clang::CXXCtorType)_s._ctor;
    }
    clang::CXXDtorType getDtorType() const {
      return (clang::CXXDtorType)_s._dtor;
    }
  };

public:
  explicit APIScanner(ASTContext &context, ParseContext &parse);
  void HandleTranslationUnit(ASTContext &context) override;
  bool VisitCXXRecordDecl(CXXRecordDecl *record);
  bool VisitFunctionDecl(FunctionDecl *func);
  bool VisitObjCInterfaceDecl(ObjCInterfaceDecl *interface);
  bool VisitObjCCategoryDecl(ObjCCategoryDecl *category);
  bool VisitObjCProtocolDecl(ObjCProtocolDecl *protocol);
  bool VisitVarDecl(VarDecl *var);

  bool shouldVisitTemplateInstantiations() const { return true; }

private:
  AvailabilityInfo getAvailabilityInfo(const NamedDecl *decl) const;
  std::pair<XPIAccess, PresumedLoc>
  getFileAttributesForDecl(const NamedDecl *decl) const;
  void recordObjCInstanceVariables(
      const iterator_range<DeclContext::specific_decl_iterator<ObjCIvarDecl>>
          ivars);
  void recordObjCSelectors(ObjCContainer *objcContainer,
                           const ObjCContainerDecl::method_range methods,
                           bool isDynamic = false);
  void emitVTableSymbols(const CXXRecordDecl *record, PresumedLoc loc,
                         XPIAccess access, AvailabilityInfo info,
                         bool emittedVTable = false);

  std::string getMangledName(const NamedDecl *decl, Flags flags = Flags());

  std::string getBackendMangledName(Twine name) {
    SmallString<128> finalName;
    llvm::Mangler::getNameWithPrefix(finalName, name, _dl);
    return finalName.str();
  }

  std::string getMangledCXXVTableName(const CXXRecordDecl *decl) {
    SmallString<128> name;
    llvm::raw_svector_ostream nameStream(name);
    _mc->mangleCXXVTable(decl, nameStream);

    return getBackendMangledName(name);
  }

  std::string getMangledCXXRTTI(const CXXRecordDecl *decl) {
    SmallString<128> name;
    llvm::raw_svector_ostream nameStream(name);
    _mc->mangleCXXRTTI(clang::QualType(decl->getTypeForDecl(), 0), nameStream);

    return getBackendMangledName(name);
  }

  std::string getMangledCXXRTTIName(const CXXRecordDecl *decl) {
    SmallString<128> name;
    llvm::raw_svector_ostream nameStream(name);
    _mc->mangleCXXRTTIName(clang::QualType(decl->getTypeForDecl(), 0),
                           nameStream);

    return getBackendMangledName(name);
  }

  std::string getMangledCXXThunk(const CXXMethodDecl *decl,
                                 const ThunkInfo &thunk) {
    SmallString<128> name;
    llvm::raw_svector_ostream nameStream(name);
    _mc->mangleThunk(decl, thunk, nameStream);

    return getBackendMangledName(name);
  }

private:
  clang::ASTContext &_context;
  clang::SourceManager &_sourceManager;
  std::unique_ptr<ItaniumMangleContext> _mc;
  llvm::DataLayout _dl;
  ParseContext &_parse;
  bool _isObjCFragile;
};

class APIScannerAction : public ASTFrontendAction {
public:
  explicit APIScannerAction(ParseContext &parse) : _parse(parse) {}

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &compiler,
                                                 StringRef inFile) override {
    return llvm::make_unique<APIScanner>(compiler.getASTContext(), _parse);
  }

private:
  ParseContext &_parse;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_SCANNER_API_SCANNER_H
