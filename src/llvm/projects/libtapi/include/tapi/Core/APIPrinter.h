//===- tapi/Core/APIPrinter.h - TAPI API Printer ----------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Declares the TAPI API Printer
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_APIPRINTER_H
#define TAPI_CORE_APIPRINTER_H

#include "tapi/Core/APIVisitor.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

class APIPrinter : public APIVisitor {
public:
  APIPrinter(raw_ostream &os, bool useColor = true);
  ~APIPrinter() override;

  void visitGlobalVariable(const API::GlobalRecord &) override;
  void visitFunction(const API::FunctionRecord &) override;
  void visitEnumConstant(const API::EnumConstantRecord &) override;
  void visitObjCInterface(const API::ObjCInterfaceRecord &) override;
  void visitObjCCategory(const API::ObjCCategoryRecord &) override;
  void visitObjCProtocol(const API::ObjCProtocolRecord &) override;
  void visitTypeDef(const API::GlobalRecord &) override;

private:
  void printMethod(const API::ObjCMethodRecord *method);
  void printProperty(const API::ObjCPropertyRecord *property);
  void printInstanceVariable(const API::ObjCInstanceVariableRecord *ivar);

  raw_ostream &os;
  bool hasColors = false;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_APIPRINTER_H
