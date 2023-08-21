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

#include "tapi/Core/LLVM.h"
#include "tapi/Core/PackedVersion.h"
#include "tapi/Core/Path.h"
#include "tapi/Defines.h"
#include "clang/Frontend/FrontendOptions.h"
#include "llvm/TextAPI/MachO/Platform.h"
#include <string>
#include <utility>
#include <vector>


TAPI_NAMESPACE_INTERNAL_BEGIN

using Macro = std::pair<std::string, bool>;
static const clang::Language defaultLanguage = clang::Language::ObjC;

namespace configuration {
namespace v1 {

struct HeaderConfiguration {
  std::string umbrellaHeader;
  PathSeq preIncludes;
  PathSeq includes;
  PathSeq excludes;
};

struct FrameworkConfiguration {
  std::string name;
  std::string path;
  std::string installName;
  clang::Language language;
  PathSeq includePaths;
  PathSeq frameworkPaths;
  std::vector<Macro> macros;
  HeaderConfiguration publicHeaderConfiguration;
  HeaderConfiguration privateHeaderConfiguration;
  bool useOverlay = false;
};

struct ProjectConfiguration {
  std::string name;
  clang::Language language;
  PathSeq includePaths;
  PathSeq frameworkPaths;
  std::vector<Macro> macros;
  bool isiOSMac = false;
  bool useOverlay = false;
  bool useUmbrellaOnly = false;
  PathSeq maskPaths;
  HeaderConfiguration publicHeaderConfiguration;
  HeaderConfiguration privateHeaderConfiguration;
};

}
} // end namespace configuration::v1.

class ConfigurationFile {
public:
  PlatformKind platform;
  PackedVersion version;
  std::string isysroot;
  clang::Language language{defaultLanguage};
  PathSeq includePaths;
  PathSeq frameworkPaths;
  std::vector<Macro> macros;
  std::vector<std::string> publicDylibs;
  std::vector<configuration::v1::FrameworkConfiguration>
      frameworkConfigurations;
  std::vector<configuration::v1::ProjectConfiguration> projectConfigurations;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_CONFIGURATION_FILE_H
