//===- tapi/Driver/Configuration.h - Configuration --------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines the configuration.
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_CONFIGURATION_H
#define TAPI_CORE_CONFIGURATION_H

#include "tapi/Core/ArchitectureSet.h"
#include "tapi/Core/ArchitectureSupport.h"
#include "tapi/Core/Path.h"
#include "tapi/Defines.h"
#include "tapi/Driver/ConfigurationFile.h"
#include "clang/Frontend/FrontendOptions.h"
#include "llvm/ADT/StringRef.h"
#include <map>
#include <string>

TAPI_NAMESPACE_INTERNAL_BEGIN

enum class HeaderType;
class DiagnosticsEngine;
class FileManager;
class Context;

struct CommandLineConfiguration {
  clang::InputKind::Language language = clang::InputKind::Unknown;
  std::string std;
  std::string isysroot;
  std::string publicUmbrellaHeaderPath;
  std::string privateUmbrellaHeaderPath;
  std::string moduleCachePath;
  std::string clangResourcePath;
  PathSeq includePaths;
  PathSeq frameworkPaths;
  std::vector<std::pair<std::string, bool /*isUndef*/>> macros;
  std::vector<std::string> clangExtraArgs;
  PathSeq extraPublicHeaders;
  PathSeq extraPrivateHeaders;
  PathSeq excludePublicHeaders;
  PathSeq excludePrivateHeaders;
  std::string visibility;
  bool useRTTI = true;
  bool scanPublicHeaders = true;
  bool scanPrivateHeaders = true;
  bool enableModules = false;
  bool validateSystemHeaders = false;
};

struct Configuration {
  ArchitectureSet arches;
  CommandLineConfiguration commandLine;
  ConfigurationFile file;
  std::map<std::string, const configuration::v1::FrameworkConfiguration *>
      pathToConfig;
  std::vector<const configuration::v1::DylibConfiguration *> dylibConfigs;

  void setConfiguration(ConfigurationFile &&configFile, StringRef basePath,
                        Context &context);

  StringRef getSysRoot() const;
  clang::InputKind::Language getLanguage(StringRef path) const;
  const std::vector<Macro> &getMacros(StringRef path) const;
  const PathSeq &getIncludePaths(StringRef path) const;
  const PathSeq &getFrameworkPaths(StringRef path) const;
  PathSeq getExtraHeaders(StringRef path, HeaderType type) const;
  PathSeq getPreIncludedHeaders(StringRef path, HeaderType type) const;
  PathSeq getExcludedHeaders(StringRef path, HeaderType type) const;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_CONFIGURATION_H
