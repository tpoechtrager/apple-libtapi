//===- tapi/Core/Configuration.h - Configuration ----------------*- C++ -*-===//
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
#include "tapi/Core/ConfigurationFile.h"
#include "tapi/Defines.h"
#include "clang/Frontend/FrontendOptions.h"
#include "llvm/ADT/StringRef.h"
#include <map>
#include <string>

TAPI_NAMESPACE_INTERNAL_BEGIN

enum class HeaderType;
class DiagnosticsEngine;
class FileManager;

struct CommandLineConfiguration {
  clang::InputKind::Language language = clang::InputKind::Unknown;
  std::string std;
  std::string isysroot;
  std::string publicUmbrellaHeaderPath;
  std::string privateUmbrellaHeaderPath;
  std::string moduleCachePath;
  std::string clangResourcePath;
  std::vector<std::string> includePaths;
  std::vector<std::string> frameworkPaths;
  std::vector<std::pair<std::string, bool /*isUndef*/>> macros;
  std::vector<std::string> clangExtraArgs;
  std::vector<std::string> extraPublicHeaders;
  std::vector<std::string> extraPrivateHeaders;
  std::vector<std::string> excludePublicHeaders;
  std::vector<std::string> excludePrivateHeaders;
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
  std::map<std::string, const configuration::v1::DylibConfiguration *>
      pathToDylibConfig;

  void setConfiguration(ConfigurationFile &&configFile, FileManager &fm,
                        StringRef basePath,
                        tapi::internal::DiagnosticsEngine &diag);

  StringRef getSysRoot() const;
  clang::InputKind::Language getLanguage(StringRef path) const;
  const std::vector<Macro> &getMacros(StringRef path) const;
  const std::vector<std::string> &getIncludePaths(StringRef path) const;
  const std::vector<std::string> &getFrameworkPaths(StringRef path) const;
  std::vector<std::string> getExtraHeaders(StringRef path,
                                           HeaderType type) const;
  std::vector<std::string> getPreIncludedHeaders(StringRef path,
                                                 HeaderType type) const;
  std::vector<std::string> getExcludedHeaders(StringRef path,
                                              HeaderType type) const;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_CONFIGURATION_H
