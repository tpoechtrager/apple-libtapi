//===- lib/Driver/SDKDBVerifier.cpp - TAPI SDKDB Verify Driver --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the SDKDB verify driver for the tapi tool.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/Registry.h"
#include "tapi/Core/YAMLReaderWriter.h"
#include "tapi/Defines.h"
#include "tapi/Driver/Diagnostics.h"
#include "tapi/Driver/Driver.h"
#include "tapi/Driver/Options.h"
#include "tapi/SDKDB/SDKDBFile.h"
#include "tapi/SDKDB/SDKDB_v1.h"

using namespace llvm;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

class FileManager;

// SDKDB Verifier Context.
namespace {
struct Context {
  // Make the context not copyable.
  Context(const Context &) = delete;
  Context &operator=(const Context &) = delete;

  Registry registry;
  FileManager &fm;
  DiagnosticsEngine &diag;

  Context(FileManager &fm, DiagnosticsEngine &diag) : fm(fm), diag(diag) {
    registry.addBinaryReaders();
    registry.addYAMLReaders();
    // Add SDKDB YAML Writer.
    auto writer = make_unique<YAMLWriter>();
    writer->add(make_unique<sdkdb::v1::YAMLDocumentHandler>());
    registry.add(std::move(writer));
    auto reader = make_unique<YAMLReader>();
    reader->add(make_unique<sdkdb::v1::YAMLDocumentHandler>());
    registry.add(std::move(reader));
  }
};
} // namespace


bool Driver::SDKDBVerifier::run(DiagnosticsEngine &diag, Options &opts) {
  Context context(opts.getFileManager(), diag);
  if (opts.driverOptions.inputs.size() != 1) {
    diag.report(diag::err_expected_one_input_file);
    return false;
  }

  if (opts.verifyOptions.baselinePath.empty()) {
    diag.report(diag::err_missing_baseline);
    return false;
  }

  const auto *file = context.fm.getFile(opts.driverOptions.inputs.front());
  if (!file) {
    diag.report(clang::diag::err_drv_no_such_file)
        << opts.driverOptions.inputs.front();
    return false;
  }

  auto bufferOrErr = context.fm.getBufferForFile(file);
  if (auto ec = bufferOrErr.getError()) {
    diag.report(diag::err_cannot_read_file) << file->getName()
                                            << ec.message();
    return false;
  }

  auto inputFile = context.registry.readFile(std::move(bufferOrErr.get()));
  if (!inputFile) {
    diag.report(diag::err_cannot_read_file)
        << file->getName() << toString(inputFile.takeError());
    return false;
  }
  auto inputDB = cast<SDKDBFile>(inputFile.get().get());
  inputDB->categoryMerge();
  if (auto Err = inputDB->takeError()) {
    diag.report(diag::err_cannot_read_file) << file->getName()
                                            << toString(std::move(Err));
    return false;
  }

  file = context.fm.getFile(opts.verifyOptions.baselinePath);
  if (!file) {
    diag.report(clang::diag::err_drv_no_such_file)
        << opts.verifyOptions.baselinePath;
    return false;
  }

  bufferOrErr = context.fm.getBufferForFile(file);
  if (auto ec = bufferOrErr.getError()) {
    diag.report(diag::err_cannot_read_file) << file->getName()
                                            << ec.message();
    return false;
  }

  auto baselineFile = context.registry.readFile(std::move(bufferOrErr.get()));
  if (!baselineFile) {
    diag.report(diag::err_cannot_read_file)
        << file->getName() << toString(baselineFile.takeError());
    return false;
  }
  auto baselineDB = cast<SDKDBFile>(baselineFile.get().get());
  baselineDB->categoryMerge();
  if (auto Err = baselineDB->takeError()) {
    logAllUnhandledErrors(std::move(Err), llvm::errs(), "");
    diag.report(diag::err_cannot_read_file) << file->getName()
                                            << toString(std::move(Err));
    return false;
  }

  auto errors = inputDB->verifySDKDBFile(baselineDB);
  if (errors) {
    logAllUnhandledErrors(std::move(errors), llvm::errs(), "");
    return false;
  }

  return true;
}

TAPI_NAMESPACE_INTERNAL_END
