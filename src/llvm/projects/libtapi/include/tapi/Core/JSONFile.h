//===- tapi/Core/JSONFile.h - TAPI JSON File --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief The JSON File is used to communicate additional information to
///        InstallAPI. For now this only includes a header list.
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_JSON_FILE_H
#define TAPI_CORE_JSON_FILE_H

#include "tapi/Core/File.h"
#include "tapi/Core/HeaderFile.h"
#include "tapi/Core/Registry.h"
#include "tapi/Core/STLExtras.h"
#include "tapi/Core/YAMLReaderWriter.h"
#include "tapi/Defines.h"
#include "llvm/Support/MemoryBuffer.h"
#include <string>

namespace llvm {
namespace yaml {
class IO;
}
} // namespace llvm

TAPI_NAMESPACE_INTERNAL_BEGIN

namespace json_file {
namespace v1 {

struct Header {
  HeaderType type;
  std::string path;
};
} // namespace v1
} // namespace json_file

class JSONFile : public File {
public:
  static bool classof(const File *file) {
    return file->kind() == File::Kind::JSONFile;
  }

  JSONFile() : File(File::Kind::JSONFile) {}

  std::vector<json_file::v1::Header> headers;
};

namespace json_file {
namespace v1 {

class YAMLDocumentHandler : public DocumentHandler {
  bool canRead(MemoryBufferRef memBufferRef,
               FileType types = FileType::All) const override;
  FileType getFileType(MemoryBufferRef memBufferRef) const override;
  bool canWrite(const File *file) const override;
  bool handleDocument(llvm::yaml::IO &io, const File *&file) const override;
};
} // namespace v1
} // namespace json_file

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_JSON_FILE_H
