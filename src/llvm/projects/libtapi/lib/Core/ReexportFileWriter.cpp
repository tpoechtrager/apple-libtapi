//===- lib/Core/ReexportFileWriter.cpp - Reexport File Writer ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the LD64 re-export file writer
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/ReexportFileWriter.h"
#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/LLVM.h"
#include "llvm/Support/ErrorHandling.h"
#include <system_error>

using namespace llvm;

TAPI_NAMESPACE_INTERNAL_BEGIN

bool ReexportFileWriter::canWrite(const File *file) const {
  auto *interface = dyn_cast<ExtendedInterfaceFile>(file);
  if (interface == nullptr)
    return false;

  if (interface->getFileType() != FileType::ReexportFile)
    return false;

  if (interface->getArchitectures().count() != 1)
    return false;

  return true;
}

Error ReexportFileWriter::writeFile(raw_ostream &os, const File *file) const {
  assert(canWrite(file) && "Cannot write provided file type");

  if (file == nullptr)
    return errorCodeToError(std::make_error_code(std::errc::invalid_argument));

  const auto *interface = cast<ExtendedInterfaceFile>(file);

  auto platform = interface->getPlatform();
  auto arch = *(interface->getArchitectures().begin());
  for (const auto *symbol : interface->exports()) {
    assert(symbol->hasArch(arch) &&
           "Symbol is not defined for expected architecture");

    switch (symbol->getKind()) {
    case XPIKind::GlobalSymbol:
      os << symbol->getName() << "\n";
      break;
    case XPIKind::ObjectiveCClass:
      if (platform == Platform::macOS && arch == Architecture::i386)
        os << ".objc_class_name_" << symbol->getName() << "\n";
      else {
        os << "_OBJC_CLASS_$_" << symbol->getName() << "\n";
        os << "_OBJC_METACLASS_$_" << symbol->getName() << "\n";
      }
      break;
    case XPIKind::ObjectiveCClassEHType:
      os << "_OBJC_EHTYPE_$_" << symbol->getName() << "\n";
      break;
    case XPIKind::ObjectiveCInstanceVariable:
      os << "_OBJC_IVAR_$_" + symbol->getName() << "\n";
      break;
    default:
      llvm_unreachable("Unexpected symbol kind for exported symbols");
    }
  }

  return Error::success();
}

TAPI_NAMESPACE_INTERNAL_END
