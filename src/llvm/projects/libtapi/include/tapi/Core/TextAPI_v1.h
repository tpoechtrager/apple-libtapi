//===- tapi/Core/TextAPI.h - Text API ---------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the content of a Text API file.
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_TEXT_API_V1_H
#define TAPI_CORE_TEXT_API_V1_H

#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/File.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Core/YAMLReaderWriter.h"
#include "tapi/Defines.h"

namespace llvm {
namespace yaml {
class IO;
} // namespace yaml
} // namespace llvm

TAPI_NAMESPACE_INTERNAL_BEGIN

struct Framework;

namespace api {
namespace v1 {

class YAMLDocumentHandler : public DocumentHandler {
  bool canRead(MemoryBufferRef memBufferRef,
               FileType types = FileType::All) const override;
  FileType getFileType(MemoryBufferRef memBufferRef) const override;
  bool canWrite(const File *file) const override;
  bool handleDocument(llvm::yaml::IO &io, const File *&file) const override;
};

std::unique_ptr<ExtendedInterfaceFile>
createTextAPIFile(const Framework &framework,
                  const ExtendedInterfaceFile *interface);

} // end namespace v1.
} // end namespace api.

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_TEXT_API_V1_H
