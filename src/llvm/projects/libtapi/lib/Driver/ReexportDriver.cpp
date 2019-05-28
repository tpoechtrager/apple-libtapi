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

#include "API2XPIConverter.h"
#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/Registry.h"
#include "tapi/Defines.h"
#include "tapi/Diagnostics/Diagnostics.h"
#include "tapi/Driver/Driver.h"
#include "tapi/Driver/Options.h"
#include "tapi/Driver/Snapshot.h"
#include "tapi/Frontend/Frontend.h"
#include "clang/Driver/DriverDiagnostic.h"

using namespace llvm;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

/// \brief Parse the headers and generate an re-export file for the linker.
bool Driver::Reexport::run(DiagnosticsEngine &diag, Options &opts) {
  auto &fm = opts.getFileManager();

  // Handle targets.
  if (opts.frontendOptions.targets.empty()) {
    diag.report(diag::err_no_target);
    return false;
  }

  // Only allow one target.
  if (opts.frontendOptions.targets.size() > 1) {
    diag.report(diag::err_one_target);
    return false;
  }

  // Handle input files.
  if (opts.driverOptions.inputs.empty()) {
    diag.report(clang::diag::err_drv_no_input_files);
    return false;
  }

  HeaderSeq files;
  for (const auto &path : opts.driverOptions.inputs) {
    if (fm.exists(path))
      files.emplace_back(path, HeaderType::Public);
    else {
      diag.report(diag::err_cannot_open_file) << path;
      return false;
    }
  }

  FrontendJob job;
  job.language = opts.frontendOptions.language;
  job.language_std = opts.frontendOptions.language_std;
  job.useRTTI = opts.frontendOptions.useRTTI;
  job.visibility = opts.frontendOptions.visibility;
  job.isysroot = opts.frontendOptions.isysroot;
  job.macros = opts.frontendOptions.macros;
  job.systemFrameworkPaths = opts.frontendOptions.systemFrameworkPaths;
  job.systemIncludePaths = opts.frontendOptions.systemIncludePaths;
  job.frameworkPaths = opts.frontendOptions.frameworkPaths;
  job.includePaths = opts.frontendOptions.includePaths;
  job.clangExtraArgs = opts.frontendOptions.clangExtraArgs;
  job.headerFiles = std::move(files);
  job.clangResourcePath = opts.frontendOptions.clangResourcePath;
  job.useObjectiveCARC = opts.frontendOptions.useObjectiveCARC;
  job.useObjectiveCWeakARC = opts.frontendOptions.useObjectiveCWeakARC;
  job.type = HeaderType::Public;

  // Infer additional include paths.
  std::set<std::string> inferredIncludePaths;
  for (const auto &header : job.headerFiles)
    inferredIncludePaths.insert(sys::path::parent_path(header.fullPath));

  job.includePaths.insert(job.includePaths.end(), inferredIncludePaths.begin(),
                          inferredIncludePaths.end());

  std::vector<FrontendContext> frontendResults;
  for (auto &target : opts.frontendOptions.targets) {
    job.target = target;
    auto result = runFrontend(job);
    if (!result)
      return false;
    frontendResults.emplace_back(std::move(result.getValue()));
  }

  auto headerSymbols = make_unique<XPISet>();
  for (auto &result : frontendResults) {
    API2XPIConverter converter(headerSymbols.get(), result.target);
    result.visit(converter);
  }

  auto scanFile = make_unique<ExtendedInterfaceFile>(std::move(headerSymbols));
  scanFile->setFileType(FileType::ReexportFile);
  scanFile->setArchitectures(
      mapToArchitectureSet(opts.frontendOptions.targets));
  scanFile->setPlatform(mapToSinglePlatform(opts.frontendOptions.targets));

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
  auto result = registry.writeFile(scanFile.get(), outputPath.str());
  if (result) {
    diag.report(diag::err_cannot_write_file)
        << outputPath.str() << toString(std::move(result));
    return false;
  }
  globalSnapshot->recordFile(outputPath.str());

  return true;
}

TAPI_NAMESPACE_INTERNAL_END
