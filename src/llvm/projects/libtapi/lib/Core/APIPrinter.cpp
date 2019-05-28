//===- lib/Core/APIPrinter.cpp - TAPI API Printer ---------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "tapi/Core/APIPrinter.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

APIPrinter::APIPrinter(raw_ostream &os, bool useColor)
    : os(os), hasColors(useColor && os.has_colors()) {}

APIPrinter::~APIPrinter() {}

static void printGlobalRecord(raw_ostream &os, const API::GlobalRecord &var,
                              bool hasColors) {
  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "- name: ";
  if (hasColors)
    os.resetColor();
  os << var.name << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  loc: ";
  if (hasColors)
    os.resetColor();
  os << var.loc.getFilename() << ":" << var.loc.getLine() << ":"
     << var.loc.getColumn() << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  availability: ";
  if (hasColors)
    os.resetColor();
  os << var.availability << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  isWeakDefined: ";
  if (hasColors)
    os.resetColor();
  os << (var.isWeakDefined ? "true" : "false") << "\n";
}

void APIPrinter::visitGlobalVariable(const API::GlobalRecord &var) {
  static bool emittedHeader = false;
  if (!emittedHeader) {
    if (hasColors)
      os.changeColor(raw_ostream::GREEN);
    os << "global variables:\n";
    if (hasColors)
      os.resetColor();
    emittedHeader = true;
  }
  printGlobalRecord(os, var, hasColors);
}

void APIPrinter::visitFunction(const API::FunctionRecord &func) {
  static bool emittedHeader = false;
  if (!emittedHeader) {
    if (hasColors)
      os.changeColor(raw_ostream::GREEN);
    os << "functions:\n";
    if (hasColors)
      os.resetColor();
    emittedHeader = true;
  }
  printGlobalRecord(os, func, hasColors);
  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  inlined: ";
  if (hasColors)
    os.resetColor();
  os << (func.isInlined ? "true" : "false") << "\n";
}

void APIPrinter::visitEnumConstant(const API::EnumConstantRecord &var) {
  static bool emittedHeader = false;
  if (!emittedHeader) {
    if (hasColors)
      os.changeColor(raw_ostream::GREEN);
    os << "enum constants:\n";
    if (hasColors)
      os.resetColor();
    emittedHeader = true;
  }

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "- name: ";
  if (hasColors)
    os.resetColor();
  os << var.name << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  loc: ";
  if (hasColors)
    os.resetColor();
  os << var.loc.getFilename() << ":" << var.loc.getLine() << ":"
     << var.loc.getColumn() << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  availability: ";
  if (hasColors)
    os.resetColor();
  os << var.availability << "\n";
}

void APIPrinter::printMethod(const API::ObjCMethodRecord *method) {
  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  - name: ";
  if (hasColors)
    os.resetColor();
  os << method->name << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "    kind: ";
  if (hasColors)
    os.resetColor();
  os << (method->isInstanceMethod ? "instance" : "class") << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "    isOptional: ";
  if (hasColors)
    os.resetColor();
  os << (method->isOptional ? "true" : "false") << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "    isDynamic: ";
  if (hasColors)
    os.resetColor();
  os << (method->isDynamic ? "true" : "false") << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "    loc: ";
  if (hasColors)
    os.resetColor();
  os << method->loc.getFilename() << ":" << method->loc.getLine() << ":"
     << method->loc.getColumn() << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "    availability: ";
  if (hasColors)
    os.resetColor();
  os << method->availability << "\n";
}

void APIPrinter::printProperty(const API::ObjCPropertyRecord *property) {
  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  - name: ";
  if (hasColors)
    os.resetColor();
  os << property->name << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "    attributes:";
  if (hasColors)
    os.resetColor();
  auto attrs = property->attributes;
  if (attrs != ObjCPropertyDecl::OBJC_PR_noattr) {
    if (attrs & ObjCPropertyDecl::OBJC_PR_readonly)
      os << " readonly";
    if (attrs & ObjCPropertyDecl::OBJC_PR_assign)
      os << " assign";
    if (attrs & ObjCPropertyDecl::OBJC_PR_readwrite)
      os << " readwrite";
    if (attrs & ObjCPropertyDecl::OBJC_PR_retain)
      os << " retain";
    if (attrs & ObjCPropertyDecl::OBJC_PR_copy)
      os << " copy";
    if (attrs & ObjCPropertyDecl::OBJC_PR_nonatomic)
      os << " nonatomic";
    if (attrs & ObjCPropertyDecl::OBJC_PR_atomic)
      os << " atomic";
    if (attrs & ObjCPropertyDecl::OBJC_PR_weak)
      os << " weak";
    if (attrs & ObjCPropertyDecl::OBJC_PR_strong)
      os << " strong";
    if (attrs & ObjCPropertyDecl::OBJC_PR_unsafe_unretained)
      os << " unsafe_unretained";
    if (attrs & ObjCPropertyDecl::OBJC_PR_class)
      os << " class";
  }
  os << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "    isOptional: ";
  if (hasColors)
    os.resetColor();
  os << (property->isOptional ? "true" : "false") << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "    getter name: ";
  if (hasColors)
    os.resetColor();
  os << property->getterName << "\n";

  if (!(attrs & ObjCPropertyDecl::OBJC_PR_readonly)) {
    if (hasColors)
      os.changeColor(raw_ostream::BLUE);
    os << "    setter name: ";
    if (hasColors)
      os.resetColor();
    os << property->setterName << "\n";
  }

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "    loc: ";
  if (hasColors)
    os.resetColor();
  os << property->loc.getFilename() << ":" << property->loc.getLine() << ":"
     << property->loc.getColumn() << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "    availability: ";
  if (hasColors)
    os.resetColor();
  os << property->availability << "\n";
}

void APIPrinter::printInstanceVariable(
    const API::ObjCInstanceVariableRecord *ivar) {
  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  - name: ";
  if (hasColors)
    os.resetColor();
  os << ivar->name << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "    loc: ";
  if (hasColors)
    os.resetColor();
  os << ivar->loc.getFilename() << ":" << ivar->loc.getLine() << ":"
     << ivar->loc.getColumn() << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "    access: ";
  if (hasColors)
    os.resetColor();
  switch (ivar->accessControl) {
  case API::ObjCInstanceVariableRecord::AccessControl::Private:
    os << "private\n";
    break;
  case API::ObjCInstanceVariableRecord::AccessControl::Protected:
    os << "protected\n";
    break;
  case API::ObjCInstanceVariableRecord::AccessControl::Public:
    os << "public\n";
    break;
  case API::ObjCInstanceVariableRecord::AccessControl::Package:
    os << "package\n";
    break;
  case API::ObjCInstanceVariableRecord::AccessControl::None:
    llvm_unreachable("cannonical acccess doesn't have None");
  }

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "    fragile: ";
  if (hasColors)
    os.resetColor();
  os << (ivar->isFragile ? "true" : "false") << "\n";
}

void APIPrinter::visitObjCInterface(const API::ObjCInterfaceRecord &interface) {
  static bool emittedHeader = false;
  if (!emittedHeader) {
    if (hasColors)
      os.changeColor(raw_ostream::GREEN);
    os << "objective-c interfaces:\n";
    if (hasColors)
      os.resetColor();
    emittedHeader = true;
  }

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "- name: ";
  if (hasColors)
    os.resetColor();
  os << interface.name << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  superClassName: ";
  if (hasColors)
    os.resetColor();
  os << interface.superClassName << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  hasExceptionAttribute: ";
  if (hasColors)
    os.resetColor();
  os << (interface.hasExceptionAttribute ? "true" : "false") << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  loc: ";
  if (hasColors)
    os.resetColor();
  os << interface.loc.getFilename() << ":" << interface.loc.getLine() << ":"
     << interface.loc.getColumn() << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  availability: ";
  if (hasColors)
    os.resetColor();
  os << interface.availability << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  categories:";
  if (hasColors)
    os.resetColor();
  for (const auto *category : interface.categories)
    if (!category->name.empty())
      os << " " << category->name;
  os << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  protocols:";
  if (hasColors)
    os.resetColor();
  for (const auto &protocol : interface.protocols)
    os << " " << protocol;
  os << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  methods:\n";
  if (hasColors)
    os.resetColor();
  for (const auto *method : interface.methods)
    printMethod(method);

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  properties:\n";
  if (hasColors)
    os.resetColor();
  for (const auto *property : interface.properties)
    printProperty(property);

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  instance variables:\n";
  if (hasColors)
    os.resetColor();
  for (const auto *ivar : interface.ivars)
    printInstanceVariable(ivar);
}

void APIPrinter::visitObjCCategory(const API::ObjCCategoryRecord &category) {
  static bool emittedHeader = false;
  if (!emittedHeader) {
    if (hasColors)
      os.changeColor(raw_ostream::GREEN);
    os << "objective-c categories:\n";
    if (hasColors)
      os.resetColor();
    emittedHeader = true;
  }

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "- name: ";
  if (hasColors)
    os.resetColor();
  os << category.name << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  interfaceName: ";
  if (hasColors)
    os.resetColor();
  os << category.interfaceName << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  loc: ";
  if (hasColors)
    os.resetColor();
  os << category.loc.getFilename() << ":" << category.loc.getLine() << ":"
     << category.loc.getColumn() << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  availability: ";
  if (hasColors)
    os.resetColor();
  os << category.availability << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  protocols:";
  if (hasColors)
    os.resetColor();
  for (const auto &protocol : category.protocols)
    os << " " << protocol;
  os << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  methods:\n";
  if (hasColors)
    os.resetColor();
  for (const auto *method : category.methods)
    printMethod(method);

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  properties:\n";
  if (hasColors)
    os.resetColor();
  for (const auto *property : category.properties)
    printProperty(property);

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  instance variables:\n";
  if (hasColors)
    os.resetColor();
  for (const auto *ivar : category.ivars)
    printInstanceVariable(ivar);
}

void APIPrinter::visitObjCProtocol(const API::ObjCProtocolRecord &protocol) {
  static bool emittedHeader = false;
  if (!emittedHeader) {
    if (hasColors)
      os.changeColor(raw_ostream::GREEN);
    os << "objective-c protocols:\n";
    if (hasColors)
      os.resetColor();
    emittedHeader = true;
  }

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "- name: ";
  if (hasColors)
    os.resetColor();
  os << protocol.name << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  loc: ";
  if (hasColors)
    os.resetColor();
  os << protocol.loc.getFilename() << ":" << protocol.loc.getLine() << ":"
     << protocol.loc.getColumn() << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  availability: ";
  if (hasColors)
    os.resetColor();
  os << protocol.availability << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  protocols:";
  if (hasColors)
    os.resetColor();
  for (const auto &protocol : protocol.protocols)
    os << " " << protocol;
  os << "\n";

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  methods:\n";
  if (hasColors)
    os.resetColor();
  for (const auto *method : protocol.methods)
    printMethod(method);

  if (hasColors)
    os.changeColor(raw_ostream::BLUE);
  os << "  properties:\n";
  if (hasColors)
    os.resetColor();
  for (const auto *property : protocol.properties)
    printProperty(property);
}

void APIPrinter::visitTypeDef(const API::GlobalRecord &type) {
  static bool emittedHeader = false;
  if (!emittedHeader) {
    if (hasColors)
      os.changeColor(raw_ostream::GREEN);
    os << "type defs:\n";
    if (hasColors)
      os.resetColor();
    emittedHeader = true;
  }
  printGlobalRecord(os, type, hasColors);
}

TAPI_NAMESPACE_INTERNAL_END
