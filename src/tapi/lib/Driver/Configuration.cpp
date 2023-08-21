//===- tapi/Core/Configuration.cpp - Configuration --------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the configuration query functions.
///
//===----------------------------------------------------------------------===//

#include "tapi/Driver/Configuration.h"
#include "tapi/Driver/ConfigurationFile.h"
#include "tapi/Driver/HeaderGlob.h"
#include "tapi/Core/Context.h"
#include "tapi/Core/FileManager.h"
#include "tapi/Core/HeaderFile.h"
#include "tapi/Core/Path.h"
#include "tapi/Diagnostics/Diagnostics.h"
#include "llvm/BinaryFormat/Magic.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Process.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

static std::string getCanonicalPath(StringRef path) {
  SmallVector<char, PATH_MAX> fullPath(path.begin(), path.end());
  llvm::sys::path::remove_dots(fullPath, /*remove_dot_dot=*/true);
  return std::string(fullPath.begin(), fullPath.end());
}

static std::string getFullPath(StringRef path,
                               StringRef base) {
  SmallString<PATH_MAX> temp(base);
  llvm::sys::path::append(temp, path);
  llvm::sys::fs::make_absolute(temp);
  return getCanonicalPath(temp);
}

static PathSeq updatePathSeq(const PathSeq &paths, StringRef root) {
  PathSeq headers;
  for (auto &p : paths) {
    auto path = getFullPath(p, root);
    headers.emplace_back(path);
  }
  return headers;
}

PathSeq Configuration::updateDirectories(StringRef frameworkPath,
                                         const PathSeq &paths) const {
  PathSeq headers;
  if (!useOverlay(frameworkPath))
    headers = updatePathSeq(paths, rootPath);
  PathSeq headersFromSDK = updatePathSeq(paths, getSysRoot());
  headers.insert(headers.end(), headersFromSDK.begin(), headersFromSDK.end());
  return headers;
}

PathSeq Configuration::updateSDKHeaderFiles(const PathSeq &paths) const {
  PathSeq headers;
  for (auto &p : paths) {
    auto path = getFullPath(p, getSysRoot());
    if (context.getFileManager().isDirectory(path, /*CacheFailure=*/false)) {
      auto result = enumerateHeaderFiles(context.getFileManager(), path);
      if (!result) {
        context.getDiag().report(diag::err)
            << path << toString(result.takeError());
        return headers;
      }
      for (auto &path : *result)
        headers.emplace_back(path);
    } else
      headers.emplace_back(path);
  }
  return headers;
}

static bool isMachOBinary(StringRef path) {
  llvm::file_magic magic;
  auto ec = identify_magic(path, magic);
  if (ec)
    return false;

  switch(magic) {
  case llvm::file_magic::macho_dynamically_linked_shared_lib:
  case llvm::file_magic::macho_dynamically_linked_shared_lib_stub:
  case llvm::file_magic::macho_universal_binary:
    return true;
  default:
    return false;
  }
}

PathSeq Configuration::updateBinaryFiles(const PathSeq &paths) const {
  PathSeq binaries;
  for (auto &p : paths) {
    auto path = getFullPath(p, rootPath);
    if (context.getFileManager().isDirectory(path, /*CacheFailure=*/false)) {
      auto result =
          enumerateFiles(context.getFileManager(), path, isMachOBinary);
      if (!result) {
        context.getDiag().report(diag::err)
            << path << toString(result.takeError());
        return binaries;
      }
      for (auto &path : *result)
        binaries.emplace_back(path);
    } else
      binaries.emplace_back(path);
  }
  return binaries;
}

void Configuration::setConfiguration(ConfigurationFile &&configFile,
                                     Context &context) {
  file = std::move(configFile);
  pathToConfig.clear();

  for (auto &conf : file.frameworkConfigurations) {
    pathToConfig.emplace(conf.path, &conf);
    conf.frameworkPaths.insert(conf.frameworkPaths.end(),
                               file.frameworkPaths.begin(),
                               file.frameworkPaths.end());

    conf.macros.insert(conf.macros.end(), file.macros.begin(),
                       file.macros.end());
  }

  // Get the project name from environment.
  if (projectName.empty())
    return;

  StringRef projName(projectName);
  // If the project name ends with _iosmac, set the default to iosmac.
  isiOSMac = projName.endswith("_iosmac");
  isDriverKit = projName.endswith("_driverkit");

  // Find the project setting from configuration file.
  // If there is setting for the project, update them as commandline options.
  for (auto &conf : file.projectConfigurations) {
    if (projName == conf.name) {
      projectConfig.reset(new configuration::v1::ProjectConfiguration(conf));
      break;
    }
    // Handle the forked version.
    auto altName = projName.split('_');
    if (altName.second == conf.name) {
      projectConfig.reset(new configuration::v1::ProjectConfiguration(conf));
      break;
    }
  }

  // For projects end with "_Sim", try search without suffix.
  if (!projectConfig && projName.consume_back("_Sim")) {
    for (auto &conf : file.projectConfigurations) {
      if (projName.endswith(conf.name)) {
        projectConfig.reset(new configuration::v1::ProjectConfiguration(conf));
        break;
      }
    }
  }
}

std::string Configuration::getSysRoot() const {
  return !commandLine.isysroot.empty() ? commandLine.isysroot : file.isysroot;
}

clang::Language Configuration::getLanguage(StringRef path) const {
  if (commandLine.language != clang::Language::Unknown)
    return commandLine.language;

  if (projectConfig)
    return projectConfig->language;

  auto it = pathToConfig.find(path.str());
  if (it != pathToConfig.end())
    return it->second->language;

  // DriverKit is c++.
  if (isDriverKitProject())
    return clang::Language::CXX;

  return file.language;
}

std::vector<Macro> Configuration::getMacros(StringRef path) const {
  if (!commandLine.macros.empty())
    return commandLine.macros;

  if (projectConfig)
    return projectConfig->macros;

  auto it = pathToConfig.find(path.str());
  if (it != pathToConfig.end())
    return it->second->macros;

  return file.macros;
}

PathSeq Configuration::getIncludePaths(StringRef path) const {
  PathSeq includePaths;
  if (!commandLine.includePaths.empty())
    includePaths.insert(includePaths.end(), commandLine.includePaths.begin(),
                        commandLine.includePaths.end());

  if (projectConfig) {
    auto projectIncludes = updateDirectories(path, projectConfig->includePaths);
    includePaths.insert(includePaths.end(), projectIncludes.begin(),
                        projectIncludes.end());
  }

  auto it = pathToConfig.find(path.str());
  if (it != pathToConfig.end()) {
    auto frameworkIncludes = updateDirectories(path, it->second->includePaths);
    includePaths.insert(includePaths.end(), frameworkIncludes.begin(),
                        frameworkIncludes.end());
  }

  auto globalIncludes = updateDirectories(path, file.includePaths);
  includePaths.insert(includePaths.end(), globalIncludes.begin(),
                      globalIncludes.end());

  return includePaths;
}

PathSeq Configuration::getFrameworkPaths(StringRef path) const {
  PathSeq frameworkPaths;
  if (!commandLine.frameworkPaths.empty())
    frameworkPaths.insert(frameworkPaths.end(),
                          commandLine.frameworkPaths.begin(),
                          commandLine.frameworkPaths.end());

  if (projectConfig) {
    auto projectFrameworks =
        updateDirectories(path, projectConfig->frameworkPaths);
    frameworkPaths.insert(frameworkPaths.end(), projectFrameworks.begin(),
                          projectFrameworks.end());
  }

  auto it = pathToConfig.find(path.str());
  if (it != pathToConfig.end()) {
    auto frameworkFrameworks =
        updateDirectories(path, it->second->frameworkPaths);
    frameworkPaths.insert(frameworkPaths.end(), frameworkFrameworks.begin(),
                          frameworkFrameworks.end());
  }

  auto globalFrameworks = updateDirectories(path, file.frameworkPaths);
  frameworkPaths.insert(frameworkPaths.end(), globalFrameworks.begin(),
                        globalFrameworks.end());

  return frameworkPaths;
}

PathSeq Configuration::getExtraHeaders(StringRef path, HeaderType type) const {
  if (type == HeaderType::Public) {
    if (!commandLine.extraPublicHeaders.empty())
      return commandLine.extraPublicHeaders;
  } else {
    assert(type == HeaderType::Private && "Unexpected header type.");
    if (!commandLine.extraPrivateHeaders.empty())
      return commandLine.extraPrivateHeaders;
  }
  
  if (projectConfig) {
    if (type == HeaderType::Public)
      return updateSDKHeaderFiles(
          projectConfig->publicHeaderConfiguration.includes);
    else
      return updateSDKHeaderFiles(
          projectConfig->privateHeaderConfiguration.includes);
  }

  auto it = pathToConfig.find(path.str());
  if (it == pathToConfig.end())
    return {};

  if (type == HeaderType::Public)
    return updateSDKHeaderFiles(it->second->publicHeaderConfiguration.includes);

  return updateSDKHeaderFiles(it->second->privateHeaderConfiguration.includes);
}

PathSeq Configuration::getPreIncludedHeaders(StringRef path,
                                             HeaderType type) const {
  if (projectConfig) {
    if (type == HeaderType::Public)
      return projectConfig->publicHeaderConfiguration.preIncludes;
    else
      return projectConfig->privateHeaderConfiguration.preIncludes;
  }

  auto it = pathToConfig.find(path.str());
  if (it == pathToConfig.end())
    return {};

  if (type == HeaderType::Public)
    return it->second->publicHeaderConfiguration.preIncludes;

  assert(type == HeaderType::Private && "Unexpected header type.");
  return it->second->privateHeaderConfiguration.preIncludes;
}

PathSeq Configuration::getExcludedHeaders(StringRef path,
                                          HeaderType type) const {
  PathSeq excludePaths;
  if (type == HeaderType::Public) {
    excludePaths.insert(excludePaths.end(),
                        commandLine.excludePublicHeaders.begin(),
                        commandLine.excludePublicHeaders.end());
  } else {
    assert(type == HeaderType::Private && "Unexpected header type.");
    excludePaths.insert(excludePaths.end(),
                        commandLine.excludePrivateHeaders.begin(),
                        commandLine.excludePrivateHeaders.end());
  }

  if (projectConfig) {
    if (type == HeaderType::Public)
      excludePaths.insert(
          excludePaths.end(),
          projectConfig->publicHeaderConfiguration.excludes.begin(),
          projectConfig->publicHeaderConfiguration.excludes.end());
    else
      excludePaths.insert(
          excludePaths.end(),
          projectConfig->privateHeaderConfiguration.excludes.begin(),
          projectConfig->privateHeaderConfiguration.excludes.end());
  }

  auto it = pathToConfig.find(path.str());
  if (it != pathToConfig.end()) {
    if (type == HeaderType::Public)
      excludePaths.insert(
          excludePaths.end(),
          it->second->publicHeaderConfiguration.excludes.begin(),
          it->second->publicHeaderConfiguration.excludes.end());
    else
      excludePaths.insert(
          excludePaths.end(),
          it->second->privateHeaderConfiguration.excludes.begin(),
          it->second->privateHeaderConfiguration.excludes.end());
  }

  return excludePaths;
}

std::string Configuration::getUmbrellaHeader(StringRef path,
                                             HeaderType type) const {
  if (type == HeaderType::Public) {
    if (!commandLine.publicUmbrellaHeaderPath.empty())
      return commandLine.publicUmbrellaHeaderPath;
  } else {
    assert(type == HeaderType::Private && "Unexpected header type.");
    if (!commandLine.privateUmbrellaHeaderPath.empty())
      return commandLine.privateUmbrellaHeaderPath;
  }

  if (projectConfig) {
    if (type == HeaderType::Public)
      return projectConfig->publicHeaderConfiguration.umbrellaHeader;
    else
      return projectConfig->privateHeaderConfiguration.umbrellaHeader;
  }

  auto it = pathToConfig.find(path.str());
  if (it == pathToConfig.end())
    return {};

  if (type == HeaderType::Public)
    return it->second->publicHeaderConfiguration.umbrellaHeader;

  return it->second->privateHeaderConfiguration.umbrellaHeader;
}

bool Configuration::isiOSMacProject() const {
  return isiOSMac || (projectConfig && projectConfig->isiOSMac);
}

bool Configuration::useOverlay(StringRef path) const {
  if (projectConfig)
    return projectConfig->useOverlay;

  auto it = pathToConfig.find(path.str());
  if (it != pathToConfig.end())
    return it->second->useOverlay;

  return true; // true by default.
}

bool Configuration::useUmbrellaOnly() const {
  return projectConfig && projectConfig->useUmbrellaOnly;
}

PathSeq Configuration::getMaskPaths() const {
  PathSeq paths;
  if (projectConfig)
    paths = projectConfig->maskPaths;
  return paths;
}

bool Configuration::isPromotedToPublicDylib(StringRef installName) const {
  auto result = llvm::find_if(file.publicDylibs, [&](StringRef glob) {
    if (installName == glob)
      return true;
    auto regex = createRegexFromGlob(glob);
    if (!regex) {
      consumeError(regex.takeError());
      return false;
    }
    return regex->match(installName);
  });
  return result != file.publicDylibs.end();
}

TAPI_NAMESPACE_INTERNAL_END
