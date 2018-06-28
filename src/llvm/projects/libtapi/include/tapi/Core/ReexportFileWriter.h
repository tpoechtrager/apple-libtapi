//===- tapi/Core/ReexportFileWriter.h - Reexport File Writer ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the Reexport File Writer
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_REEXPORT_FILE_WRITER_H
#define TAPI_CORE_REEXPORT_FILE_WRITER_H

#include "tapi/Core/LLVM.h"
#include "tapi/Core/Registry.h"
#include "tapi/Defines.h"
#include "llvm/Support/Error.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

class ReexportFileWriter final : public Writer {
public:
  bool canWrite(const File *file) const override;
  Error writeFile(llvm::raw_ostream &os, const File *file) const override;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_REEXPORT_FILE_WRITER_H
