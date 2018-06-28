//===- lib/Core/JSONFile.cpp - JSON File ------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the JSON File. For now this only contains a header list.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/JSONFile.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/YAMLTraits.h"

using namespace llvm;
using namespace llvm::yaml;
using namespace TAPI_INTERNAL;
using namespace TAPI_INTERNAL::json_file::v1;

LLVM_YAML_IS_SEQUENCE_VECTOR(Header)

namespace llvm {
namespace yaml {

template <> struct ScalarEnumerationTraits<HeaderType> {
  static void enumeration(IO &io, HeaderType &type) {
    io.enumCase(type, "public", HeaderType::Public);
    io.enumCase(type, "private", HeaderType::Private);
  }
};

template <> struct MappingTraits<Header> {
  static void mapping(IO &io, Header &header) {
    io.mapRequired("type", header.type);
    io.mapRequired("path", header.path);
  }
};

template <> struct MappingTraits<const JSONFile *> {
  struct NormalizedJSONFile : JSONFile {
    explicit NormalizedJSONFile(IO &io) {}
    NormalizedJSONFile(IO &io, const JSONFile *&file) {
      llvm_unreachable("Writing is not supported!");
    }

    const NormalizedJSONFile *denormalize(IO &io) {
      auto ctx = reinterpret_cast<YAMLContext *>(io.getContext());
      assert(ctx);

      this->setPath(ctx->path);
      this->setFileType(TAPI_INTERNAL::FileType::JSON_V1);

      return this;
    }

    unsigned version{};
  };

  static void mappingJSON(IO &io, const JSONFile *&file) {
    MappingNormalizationHeap<NormalizedJSONFile, const JSONFile *> keys(
        io, file, nullptr);

    io.mapRequired("version", keys->version);
    io.mapOptional("headers", keys->headers);
  }
};
} // namespace yaml
} // namespace llvm

TAPI_NAMESPACE_INTERNAL_BEGIN

namespace json_file {
namespace v1 {

bool YAMLDocumentHandler::canRead(MemoryBufferRef memBufferRef,
                                  FileType types) const {
  if (!(types & FileType::JSON_V1))
    return false;

  return true;
}

FileType YAMLDocumentHandler::getFileType(MemoryBufferRef memBufferRef) const {
  if (canRead(memBufferRef))
    return FileType::JSON_V1;

  return FileType::Invalid;
}

bool YAMLDocumentHandler::canWrite(const File *file) const { return false; }

bool YAMLDocumentHandler::handleDocument(IO &io, const File *&file) const {
  const auto *json = dyn_cast_or_null<JSONFile>(file);
  MappingTraits<const JSONFile *>::mappingJSON(io, json);
  file = json;

  return true;
}
} // namespace v1
} // namespace json_file

TAPI_NAMESPACE_INTERNAL_END
