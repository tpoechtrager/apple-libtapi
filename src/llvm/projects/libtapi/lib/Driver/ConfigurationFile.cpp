//===- tapi/Core/ConfigurationFile.cpp - Configuration File -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the configuration file reader.
///
//===----------------------------------------------------------------------===//

#include "tapi/Driver/ConfigurationFile.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Core/YAML.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/YAMLTraits.h"

using namespace llvm;
using namespace llvm::yaml;
using namespace TAPI_INTERNAL;
using namespace TAPI_INTERNAL::configuration::v1;

LLVM_YAML_IS_FLOW_SEQUENCE_VECTOR(Macro)
LLVM_YAML_IS_SEQUENCE_VECTOR(FrameworkConfiguration)
LLVM_YAML_IS_SEQUENCE_VECTOR(DylibConfiguration)

namespace llvm {
namespace yaml {

template <> struct ScalarTraits<Macro> {
  static void output(const Macro &macro, void * /*unused*/, raw_ostream &out) {
    out << (macro.second ? "-U" : "-D") << macro.first;
  }

  static StringRef input(StringRef scalar, void * /*unused*/, Macro &value) {
    if (scalar.startswith("-D")) {
      value = std::make_pair(scalar.drop_front(2), false);
      return {};
    }

    if (scalar.startswith("-U")) {
      value = std::make_pair(scalar.drop_front(2), true);
      return {};
    }

    return {"invalid macro"};
  }

  static QuotingType mustQuote(StringRef /*unused*/) {
    return QuotingType::None;
  }
};

template <> struct MappingTraits<HeaderConfiguration> {
  static void mapping(IO &io, HeaderConfiguration &config) {
    io.mapOptional("pre-includes", config.preIncludes);
    io.mapOptional("includes", config.includes);
    io.mapOptional("excludes", config.excludes);
  }
};

template <> struct MappingTraits<FrameworkConfiguration> {
  static void mapping(IO &io, FrameworkConfiguration &config) {
    io.mapRequired("name", config.name);
    io.mapRequired("path", config.path);
    io.mapOptional("install-name", config.installName);
    io.mapOptional("language", config.language, defaultLanguage);
    io.mapOptional("include-paths", config.includePaths);
    io.mapOptional("framework-paths", config.frameworkPaths);
    io.mapOptional("macros", config.macros);
    io.mapOptional("public-header", config.publicHeaderConfiguration);
    io.mapOptional("private-header", config.privateHeaderConfiguration);
  }
};

template <> struct MappingTraits<DylibConfiguration> {
  static void mapping(IO &io, DylibConfiguration &config) {
    io.mapRequired("name", config.name);
    io.mapOptional("install-name", config.installName);
    io.mapOptional("language", config.language, defaultLanguage);
    io.mapOptional("include-paths", config.includePaths);
    io.mapOptional("binaries", config.binaries);
    io.mapOptional("macros", config.macros);
    io.mapOptional("public-header", config.publicHeaderConfiguration);
    io.mapOptional("private-header", config.privateHeaderConfiguration);
  }
};

template <> struct MappingTraits<const ConfigurationFile *> {
  struct NormalizedTAPIConfV1 {
    explicit NormalizedTAPIConfV1(IO &io) {}
    NormalizedTAPIConfV1(IO &io, const ConfigurationFile *&file) {
      llvm_unreachable("Writting is not supported!");
    }

    const ConfigurationFile *denormalize(IO &io) {
      auto ctx = reinterpret_cast<YAMLContext *>(io.getContext());
      assert(ctx);

      auto *file = new ConfigurationFile;
      file->setPath(ctx->path);
      file->setFileType(TAPI_INTERNAL::FileType::TAPI_Configuration_V1);
      file->platform = platform;
      file->version = version;
      file->isysroot = std::move(isysroot);
      file->language = language;
      file->includePaths = std::move(includePaths);
      file->frameworkPaths = std::move(frameworkPaths);
      file->macros = std::move(macros);
      file->frameworkConfigurations = std::move(frameworkConfigurations);
      file->dylibConfigurations = std::move(dylibConfigurations);

      return file;
    }

    Platform platform;
    PackedVersion version;
    std::string isysroot;
    clang::InputKind::Language language;
    PathSeq includePaths;
    PathSeq frameworkPaths;
    std::vector<Macro> macros;
    std::vector<FrameworkConfiguration> frameworkConfigurations;
    std::vector<DylibConfiguration> dylibConfigurations;
  };

  static void mappingTAPIConfV1(IO &io, const ConfigurationFile *&file) {
    MappingNormalization<NormalizedTAPIConfV1, const ConfigurationFile *> keys(
        io, file);
    io.mapTag("tapi-configuration-v1", true);
    io.mapRequired("sdk-platform", keys->platform);
    io.mapRequired("sdk-version", keys->version);
    io.mapRequired("sdk-root", keys->isysroot);
    io.mapOptional("language", keys->language, clang::InputKind::ObjC);
    io.mapOptional("include-paths", keys->includePaths);
    io.mapOptional("framework-paths", keys->frameworkPaths);
    io.mapOptional("macros", keys->macros);
    io.mapOptional("frameworks", keys->frameworkConfigurations);
    io.mapOptional("dylibs", keys->dylibConfigurations);
  }
};

} // end namespace yaml.
} // end namespace llvm.

TAPI_NAMESPACE_INTERNAL_BEGIN

namespace configuration {
namespace v1 {

bool YAMLDocumentHandler::canRead(MemoryBufferRef memBufferRef,
                                  FileType types) const {
  if (!(types & FileType::TAPI_Configuration_V1))
    return false;

  auto str = memBufferRef.getBuffer().trim();
  if (!(str.startswith("---\n") ||
        str.startswith("--- !tapi-configuration-v1\n")) ||
      !str.endswith("..."))
    return false;

  return true;
}

FileType YAMLDocumentHandler::getFileType(MemoryBufferRef memBufferRef) const {
  if (canRead(memBufferRef))
    return FileType::TAPI_Configuration_V1;

  return FileType::Invalid;
}

bool YAMLDocumentHandler::canWrite(const File *file) const { return false; }

bool YAMLDocumentHandler::handleDocument(IO &io, const File *&file) const {
  if (io.outputting() && file->getFileType() != FileType::TAPI_Configuration_V1)
    return false;

  if (!io.outputting() && !io.mapTag("!tapi-configuration-v1") &&
      !io.mapTag("tag:yaml.org,2002:map"))
    return false;

  auto *ctx = reinterpret_cast<YAMLContext *>(io.getContext());
  ctx->fileType = FileType::TAPI_Configuration_V1;

  const auto *configuration = dyn_cast_or_null<ConfigurationFile>(file);
  MappingTraits<const ConfigurationFile *>::mappingTAPIConfV1(io,
                                                              configuration);
  file = configuration;

  return true;
}
} // end namespace v1.
} // end namespace configuration.

TAPI_NAMESPACE_INTERNAL_END
