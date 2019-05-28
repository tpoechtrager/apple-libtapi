//===- tapi/Driver/ConfigurationFile.h - Configuration File -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the configuration file.
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_CONFIGURATION_FILE_H
#define TAPI_CORE_CONFIGURATION_FILE_H

#include "tapi/Core/ArchitectureSupport.h"
#include "tapi/Core/File.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Core/Path.h"
#include "tapi/Core/Platform.h"
#include "tapi/Core/Registry.h"
#include "tapi/Core/YAMLReaderWriter.h"
#include "tapi/Defines.h"
#include "clang/Frontend/FrontendOptions.h"
#include "llvm/Support/MemoryBuffer.h"
#include <string>
#include <utility>
#include <vector>

namespace llvm {
namespace yaml {
class IO;
}
} // namespace llvm

TAPI_NAMESPACE_INTERNAL_BEGIN

using Macro = std::pair<std::string, bool>;
static const clang::InputKind::Language defaultLanguage =
    clang::InputKind::ObjC;

namespace configuration {
namespace v1 {

struct HeaderConfiguration {
  PathSeq preIncludes;
  PathSeq includes;
  PathSeq excludes;
};

struct FrameworkConfiguration {
  std::string name;
  std::string path;
  std::string installName;
  clang::InputKind::Language language;
  PathSeq includePaths;
  PathSeq frameworkPaths;
  std::vector<Macro> macros;
  HeaderConfiguration publicHeaderConfiguration;
  HeaderConfiguration privateHeaderConfiguration;
};

struct DylibConfiguration {
  std::string name;
  std::string installName;
  clang::InputKind::Language language;
  PathSeq includePaths;
  PathSeq binaries;
  std::vector<Macro> macros;
  HeaderConfiguration publicHeaderConfiguration;
  HeaderConfiguration privateHeaderConfiguration;
};

struct Configuration {
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

class YAMLDocumentHandler : public DocumentHandler {
  bool canRead(MemoryBufferRef memBufferRef,
               FileType types = FileType::All) const override;
  FileType getFileType(MemoryBufferRef memBufferRef) const override;
  bool canWrite(const File *file) const override;
  bool handleDocument(llvm::yaml::IO &io, const File *&file) const override;
};
} // namespace v1
} // namespace configuration

class ConfigurationFile : public File {
public:
  static bool classof(const File *file) {
    return file->kind() == File::Kind::Configuration;
  }

  ConfigurationFile()
      : File(File::Kind::Configuration), language(defaultLanguage) {}

  Platform platform;
  PackedVersion version;
  std::string isysroot;
  clang::InputKind::Language language;
  PathSeq includePaths;
  PathSeq frameworkPaths;
  std::vector<Macro> macros;
  std::vector<configuration::v1::FrameworkConfiguration>
      frameworkConfigurations;
  std::vector<configuration::v1::DylibConfiguration> dylibConfigurations;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_CONFIGURATION_FILE_H
