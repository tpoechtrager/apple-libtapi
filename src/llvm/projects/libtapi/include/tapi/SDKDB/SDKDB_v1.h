//===- tapi/Core/SDKDB_v1.h - SDKDB v1 --------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the content of SDKDB v1
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_SDKDB_V1_H
#define TAPI_CORE_SDKDB_V1_H

#include "tapi/Core/LLVM.h"
#include "tapi/Core/YAMLReaderWriter.h"
#include "tapi/Defines.h"
#include "llvm/Support/YAMLTraits.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

namespace sdkdb {
namespace v1 {

class YAMLDocumentHandler : public DocumentHandler {
  bool canRead(MemoryBufferRef memBufferRef,
               FileType types = FileType::All) const override;
  FileType getFileType(MemoryBufferRef memBufferRef) const override;
  bool canWrite(const File *file) const override;
  bool handleDocument(llvm::yaml::IO &io, const File *&file) const override;
};

} // end namespace v1.
} // end namespace sdkdb.

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_SDKDB_V1_H
