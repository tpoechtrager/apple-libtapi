//===- lib/Driver/ReexportDriver.cpp - TAPI Reexport Driver -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the driver for the tapi tool.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/Registry.h"
#include "tapi/Defines.h"
#include "tapi/Driver/Diagnostics.h"
#include "tapi/Driver/Driver.h"
#include "tapi/Driver/Options.h"
#include "tapi/Driver/Snapshot.h"
#include "tapi/Scanner/Scanner.h"
#include "clang/Driver/DriverDiagnostic.h"

using namespace llvm;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

/// \brief Parse the headers and generate an re-export file for the linker.
bool Driver::Reexport::run(DiagnosticsEngine &diag, Options &opts) {
  auto &fm = opts.getFileManager();

  // Handle architecture.
  if (opts.linkerOptions.architectures.empty()) {
    diag.report(diag::err_no_architecture);
    return false;
  }

  // Only allow one architecture.
  if (opts.linkerOptions.architectures.count() > 1) {
    diag.report(diag::err_one_architecture) << opts.linkerOptions.architectures;
    return false;
  }

  if (opts.frontendOptions.platform == Platform::Unknown) {
    diag.report(diag::err_no_deployment_target);
    return false;
  }


  // Handle input files.
  if (opts.driverOptions.inputs.empty()) {
    diag.report(clang::diag::err_drv_no_input_files);
    return false;
  }

  std::vector<const FileEntry *> files;
  for (const auto &path : opts.driverOptions.inputs) {
    if (const auto *file = fm.getFile(path)) {
      files.emplace_back(file);
    } else {
      diag.report(diag::err_cannot_open_file) << path;
      return false;
    }
  }

  auto job = make_unique<ParsingJob>();
  job->fileManager = &fm;
  job->architectures = opts.linkerOptions.architectures;
  job->platform = opts.frontendOptions.platform;
  job->osVersion = opts.frontendOptions.osVersion;
  job->language = opts.frontendOptions.language;
  job->language_std = opts.frontendOptions.language_std;
  job->useRTTI = opts.frontendOptions.useRTTI;
  job->visibility = opts.frontendOptions.visibility;
  job->isysroot = opts.frontendOptions.isysroot;
  job->macros = opts.frontendOptions.macros;
  job->systemFrameworkPaths = opts.frontendOptions.systemFrameworkPaths;
  job->systemIncludePaths = opts.frontendOptions.systemIncludePaths;
  job->frameworkPaths = opts.frontendOptions.frameworkPaths;
  job->includePaths = opts.frontendOptions.includePaths;
  job->clangExtraArgs = opts.frontendOptions.clangExtraArgs;
  job->publicHeaderFiles = std::move(files);
  job->clangResourcePath = opts.frontendOptions.clangResourcePath;
  job->useObjectiveCARC = opts.frontendOptions.useObjectiveCARC;
  job->useObjectiveCWeakARC = opts.frontendOptions.useObjectiveCWeakARC;

  // Infer additional include paths.
  std::set<std::string> inferredIncludePaths;
  for (const auto *file : job->publicHeaderFiles)
    inferredIncludePaths.insert(file->getDir()->getName());

  job->includePaths.insert(job->includePaths.end(),
                           inferredIncludePaths.begin(),
                           inferredIncludePaths.end());

  auto headerSymbols = Scanner::run(std::move(job));
  if (!headerSymbols)
    return false;

  auto scanFile = make_unique<ExtendedInterfaceFile>(std::move(headerSymbols));
  scanFile->setFileType(FileType::ReexportFile);
  scanFile->setArchitectures(opts.linkerOptions.architectures);
  scanFile->setPlatform(opts.frontendOptions.platform);

  SmallString<PATH_MAX> outputPath(opts.driverOptions.outputPath);
  if (outputPath.empty()) {
    auto ec = sys::fs::current_path(outputPath);
    if (ec) {
      diag.report(diag::err) << outputPath << ec.message();
      return false;
    }
    sys::path::append(outputPath, "linker.reexport");
  }

  SmallString<PATH_MAX> outputDir(outputPath);
  sys::path::remove_filename(outputDir);
  auto ec = sys::fs::create_directories(outputDir);
  if (ec) {
    diag.report(diag::err_cannot_create_directory) << outputDir << ec.message();
    return false;
  }

  Registry registry;
  registry.addReexportWriters();
  scanFile->setPath(outputPath.str());
  auto result = registry.writeFile(scanFile.get());
  if (result) {
    diag.report(diag::err_cannot_write_file) << scanFile->getPath()
                                             << toString(std::move(result));
    return false;
  }
  globalSnapshot->recordFile(scanFile->getPath());

  return true;
}

TAPI_NAMESPACE_INTERNAL_END
