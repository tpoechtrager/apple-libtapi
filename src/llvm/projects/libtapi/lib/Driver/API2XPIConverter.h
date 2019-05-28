//===- API2XPIConverter.h - API2XPI Converter -------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief API2XPI Converter
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/APIVisitor.h"
#include "tapi/Core/XPISet.h"
#include "tapi/Defines.h"

#ifndef TAPI_DRIVER_API2XPICONVERTER_H
#define TAPI_DRIVER_API2XPICONVERTER_H

TAPI_NAMESPACE_INTERNAL_BEGIN

class API2XPIConverter : public APIVisitor {
public:
  API2XPIConverter(XPISet *xpiSet, const llvm::Triple &target,
                   bool visitAllObjC = false)
      : xpiSet(xpiSet), architecture(getArchType(target.getArchName())),
        visitAllObjC(visitAllObjC) {}
  void visitGlobalVariable(const API::GlobalRecord &) override;
  void visitFunction(const API::FunctionRecord &) override;
  void visitObjCInterface(const API::ObjCInterfaceRecord &) override;
  void visitObjCCategory(const API::ObjCCategoryRecord &) override;
  void visitObjCProtocol(const API::ObjCProtocolRecord &) override;

private:
  XPISet *xpiSet;
  Architecture architecture;
  const bool visitAllObjC;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_DRIVER_API2XPICONVERTER_H
