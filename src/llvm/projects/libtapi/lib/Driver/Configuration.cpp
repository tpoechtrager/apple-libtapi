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
#include "tapi/Core/Context.h"
#include "tapi/Core/FileManager.h"
#include "tapi/Core/HeaderFile.h"
#include "tapi/Core/Path.h"
#include "tapi/Core/Registry.h"
#include "tapi/Diagnostics/Diagnostics.h"
#include "tapi/Driver/ConfigurationFile.h"
#include "llvm/BinaryFormat/Magic.h"
#include "llvm/Support/FileSystem.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

static std::string getCanonicalPath(StringRef path) {
  SmallVector<char, PATH_MAX> fullPath(path.begin(), path.end());
  llvm::sys::path::remove_dots(fullPath, /*remove_dot_dot=*/true);
  return std::string(fullPath.begin(), fullPath.end());
}

static std::string getFullPath(StringRef path, StringRef base) {
  SmallString<PATH_MAX> temp(base);
  llvm::sys::path::append(temp, path);
  llvm::sys::fs::make_absolute(temp);
  return getCanonicalPath(temp);
}

static void
updateHeaderConfigurations(configuration::v1::HeaderConfiguration &HC,
                           StringRef base, FileManager &fm,
                           tapi::internal::DiagnosticsEngine &diag) {
  auto updateHeaderFiles = [&fm, &diag, &base](PathSeq &paths) {
    PathSeq headers;
    for (auto &path : paths) {
      path = getFullPath(path, base);
      if (fm.isDirectory(path, /*CacheFailure=*/false)) {
        auto result = enumerateHeaderFiles(fm, path);
        if (!result) {
          diag.report(diag::err) << path << toString(result.takeError());
          return;
        }
        for (auto &path : *result)
          headers.emplace_back(path);
      } else
        headers.emplace_back(path);
    }

    std::swap(paths, headers);
  };

  updateHeaderFiles(HC.preIncludes);
  updateHeaderFiles(HC.includes);
  updateHeaderFiles(HC.excludes);
}

static bool isMachOBinary(StringRef path) {
  llvm::file_magic magic;
  auto ec = identify_magic(path, magic);
  if (ec)
    return false;

  switch (magic) {
  case llvm::file_magic::macho_dynamically_linked_shared_lib:
  case llvm::file_magic::macho_dynamically_linked_shared_lib_stub:
  case llvm::file_magic::macho_universal_binary:
    return true;
  default:
    return false;
  }
}

void Configuration::setConfiguration(ConfigurationFile &&configFile,
                                     StringRef basePath, Context &context) {
  file = std::move(configFile);
  pathToConfig.clear();

  for (auto &conf : file.frameworkConfigurations) {
    conf.path = getFullPath(conf.path, basePath);

    pathToConfig.emplace(conf.path, &conf);
    for (auto &inc : conf.includePaths) {
      inc = getFullPath(inc, basePath);
    }

    conf.frameworkPaths.insert(conf.frameworkPaths.end(),
                               file.frameworkPaths.begin(),
                               file.frameworkPaths.end());

    conf.macros.insert(conf.macros.end(), file.macros.begin(),
                       file.macros.end());

    // Upgrade paths in the configuration file.
    updateHeaderConfigurations(conf.publicHeaderConfiguration, basePath,
                               context.getFileManager(), context.getDiag());
    updateHeaderConfigurations(conf.privateHeaderConfiguration, basePath,
                               context.getFileManager(), context.getDiag());
  }

  for (auto &conf : file.dylibConfigurations) {
    dylibConfigs.emplace_back(&conf);
    for (auto &inc : conf.includePaths) {
      conf.includePaths.emplace_back(getFullPath(inc, basePath));
    }

    conf.macros.insert(conf.macros.end(), file.macros.begin(),
                       file.macros.end());

    // Upgrade paths in the configuration file.
    updateHeaderConfigurations(conf.publicHeaderConfiguration, basePath,
                               context.getFileManager(), context.getDiag());
    updateHeaderConfigurations(conf.privateHeaderConfiguration, basePath,
                               context.getFileManager(), context.getDiag());

    // Upgrade paths for binary files.
    PathSeq binaries;
    for (auto &f : conf.binaries) {
      f = getFullPath(f, basePath);
      if (context.getFileManager().isDirectory(f, /*CacheFailure=*/false)) {
        auto result =
            enumerateFiles(context.getFileManager(), f, isMachOBinary);
        if (!result) {
          context.getDiag().report(diag::err)
              << f << toString(result.takeError());
          return;
        }
        for (auto &path : *result)
          binaries.emplace_back(path);
      } else
        binaries.emplace_back(f);
    }
    std::swap(binaries, conf.binaries);
  }
}

StringRef Configuration::getSysRoot() const {
  if (!commandLine.isysroot.empty())
    return commandLine.isysroot;

  return file.isysroot;
}

clang::InputKind::Language Configuration::getLanguage(StringRef path) const {
  if (commandLine.language != clang::InputKind::Unknown)
    return commandLine.language;

  auto it = pathToConfig.find(getCanonicalPath(path));
  if (it != pathToConfig.end())
    return it->second->language;

  return file.language;
}

const std::vector<Macro> &Configuration::getMacros(StringRef path) const {
  if (!commandLine.macros.empty())
    return commandLine.macros;

  auto it = pathToConfig.find(getCanonicalPath(path));
  if (it != pathToConfig.end())
    return it->second->macros;

  return file.macros;
}

const PathSeq &Configuration::getIncludePaths(StringRef path) const {
  if (!commandLine.includePaths.empty())
    return commandLine.includePaths;

  auto it = pathToConfig.find(getCanonicalPath(path));
  if (it != pathToConfig.end())
    return it->second->includePaths;

  return file.includePaths;
}

const PathSeq &Configuration::getFrameworkPaths(StringRef path) const {
  if (!commandLine.frameworkPaths.empty())
    return commandLine.frameworkPaths;

  auto it = pathToConfig.find(getCanonicalPath(path));
  if (it != pathToConfig.end())
    return it->second->frameworkPaths;

  return file.frameworkPaths;
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

  auto it = pathToConfig.find(getCanonicalPath(path));
  if (it == pathToConfig.end())
    return {};

  if (type == HeaderType::Public)
    return it->second->publicHeaderConfiguration.includes;

  return it->second->privateHeaderConfiguration.includes;
}

PathSeq Configuration::getPreIncludedHeaders(StringRef path,
                                             HeaderType type) const {
  auto it = pathToConfig.find(getCanonicalPath(path));
  if (it == pathToConfig.end())
    return {};

  if (type == HeaderType::Public)
    return it->second->publicHeaderConfiguration.preIncludes;

  assert(type == HeaderType::Private && "Unexpected header type.");
  return it->second->privateHeaderConfiguration.preIncludes;
}

PathSeq Configuration::getExcludedHeaders(StringRef path,
                                          HeaderType type) const {
  if (type == HeaderType::Public) {
    if (!commandLine.excludePublicHeaders.empty())
      return commandLine.excludePublicHeaders;
  } else {
    assert(type == HeaderType::Private && "Unexpected header type.");
    if (!commandLine.excludePrivateHeaders.empty())
      return commandLine.excludePrivateHeaders;
  }

  auto it = pathToConfig.find(getCanonicalPath(path));
  if (it == pathToConfig.end())
    return {};

  if (type == HeaderType::Public)
    return it->second->publicHeaderConfiguration.excludes;

  return it->second->privateHeaderConfiguration.excludes;
}

TAPI_NAMESPACE_INTERNAL_END
