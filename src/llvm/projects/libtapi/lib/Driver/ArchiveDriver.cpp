//===- lib/Driver/ArchiveDriver.cpp - TAPI Archive Driver -------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the archive driver for the tapi tool.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/Registry.h"
#include "tapi/Defines.h"
#include "tapi/Driver/Diagnostics.h"
#include "tapi/Driver/Driver.h"
#include "tapi/Driver/Options.h"
#include "tapi/Driver/Snapshot.h"
#include "clang/Driver/DriverDiagnostic.h"

using namespace llvm;

TAPI_NAMESPACE_INTERNAL_BEGIN

/// \brief Merge or thin text-based stub files.
bool Driver::Archive::run(DiagnosticsEngine &diag, Options &opts) {
  auto &fm = opts.getFileManager();

  // Handle input files.
  if (opts.driverOptions.inputs.empty()) {
    diag.report(clang::diag::err_drv_no_input_files);
    return false;
  }

  switch (opts.archiveOptions.action) {
  default:
    break;
  case ArchiveAction::ShowInfo:
  case ArchiveAction::ExtractArchitecture:
  case ArchiveAction::VerifyArchitecture:
  case ArchiveAction::ListSymbols:
    if (opts.driverOptions.inputs.size() != 1) {
      diag.report(diag::err_expected_one_input_file);
      return false;
    }
    break;
  }

  switch (opts.archiveOptions.action) {
  default:
    break;
  case ArchiveAction::ExtractArchitecture:
  case ArchiveAction::Merge:
    if (opts.driverOptions.outputPath.empty()) {
      diag.report(diag::err_no_output_file);
      return false;
    }
    break;
  }

  Registry registry;
  registry.addYAMLReaders();
  registry.addYAMLWriters();

  std::vector<std::unique_ptr<InterfaceFile>> inputs;
  for (const auto &path : opts.driverOptions.inputs) {
    auto bufferOr = fm.getBufferForFile(path);
    if (auto ec = bufferOr.getError()) {
      diag.report(diag::err_cannot_read_file) << path << ec.message();
      return false;
    }

    auto file = registry.readFile(std::move(bufferOr.get()));
    if (!file) {
      diag.report(diag::err_cannot_read_file) << path
                                              << toString(file.takeError());
      return false;
    }

    switch (file.get()->getFileType()) {
    default:
      diag.report(diag::err_unsupported_file_type);
      return false;
    case TBD_V1:
    case TBD_V2:
    case API_V1:
    case SPI_V1:
      break;
    }

    assert(isa<InterfaceFile>(file.get().get()) && "unexpected file type");
    inputs.emplace_back(cast<InterfaceFile>(file.get().release()));
  }

  std::unique_ptr<InterfaceFile> output;
  switch (opts.archiveOptions.action) {
  default:
    return false;

  case ArchiveAction::ShowInfo:
    assert(inputs.size() == 1 && "expecting exactly one input file");
    outs() << "Architectures: " << inputs.front()->getArchitectures() << '\n';
    break;

  case ArchiveAction::ExtractArchitecture: {
    assert(inputs.size() == 1 && "expecting exactly one input file");
    auto file = inputs.front()->extract(opts.archiveOptions.arch);
    if (!file) {
      diag.report(diag::err) << inputs.front()->getPath()
                             << toString(file.takeError());
      return false;
    }
    output = std::move(file.get());
    break;
  }

  case ArchiveAction::VerifyArchitecture:
    assert(inputs.size() == 1 && "expecting exactly one input file");
    return inputs.front()->getArchitectures().has(opts.archiveOptions.arch);

  case ArchiveAction::Merge: {
    assert(!inputs.empty() && "expecting at least one input file");
    for (auto &file : inputs) {
      if (!output) {
        output = std::move(file);
        continue;
      }

      auto result = output->merge(file.get());
      if (!result) {
        diag.report(diag::err) << file->getPath()
                               << toString(result.takeError());
        return false;
      }
      output = std::move(result.get());
    }
    break;
  }
  case ArchiveAction::ListSymbols: {
    assert(inputs.size() == 1 && "expecting exactly one input file");
    // Only allow one architecture.
    if (opts.linkerOptions.architectures.count() > 1) {
      diag.report(diag::err_one_architecture)
          << opts.linkerOptions.architectures;
      return false;
    }
    inputs.front()->printSymbols(opts.linkerOptions.architectures);
    break;
  }
  }

  if (output) {
    output->setPath(opts.driverOptions.outputPath);
    auto result = registry.writeFile(output.get());
    if (result) {
      diag.report(diag::err_cannot_write_file) << output->getPath()
                                               << toString(std::move(result));
      return false;
    }
  }
  if (output)
    globalSnapshot->recordFile(output->getPath());

  return true;
}

TAPI_NAMESPACE_INTERNAL_END
