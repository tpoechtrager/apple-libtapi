//===- tapi/Core/APIVisitor.h - TAPI API Visitor ----------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the API Visitor.
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_APIVISITOR_H
#define TAPI_CORE_APIVISITOR_H

#include "tapi/Core/API.h"
#include "tapi/Defines.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

class APIVisitor {
public:
  virtual ~APIVisitor();

  virtual void visitGlobalVariable(const API::GlobalRecord &);
  virtual void visitFunction(const API::FunctionRecord &);
  virtual void visitEnumConstant(const API::EnumConstantRecord &);
  virtual void visitObjCInterface(const API::ObjCInterfaceRecord &);
  virtual void visitObjCCategory(const API::ObjCCategoryRecord &);
  virtual void visitObjCProtocol(const API::ObjCProtocolRecord &);
  virtual void visitTypeDef(const API::GlobalRecord &);
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_APIVISITOR_H
