//===- lib/Core/APIVisitor.cpp - TAPI API Visitor ---------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "tapi/Core/APIVisitor.h"

using namespace llvm;

TAPI_NAMESPACE_INTERNAL_BEGIN

APIVisitor::~APIVisitor() {}

void APIVisitor::visitGlobalVariable(const API::GlobalRecord &) {}
void APIVisitor::visitFunction(const API::FunctionRecord &) {}
void APIVisitor::visitEnumConstant(const API::EnumConstantRecord &) {}
void APIVisitor::visitObjCInterface(const API::ObjCInterfaceRecord &) {}
void APIVisitor::visitObjCCategory(const API::ObjCCategoryRecord &) {}
void APIVisitor::visitObjCProtocol(const API::ObjCProtocolRecord &) {}
void APIVisitor::visitTypeDef(const API::GlobalRecord &) {}

TAPI_NAMESPACE_INTERNAL_END
