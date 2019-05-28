//===- tapi-frontend/tapi-frontend.cpp - TAPI Frontend Tool -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
///
/// A tool to run the TAPI frontent for testing.
///
//===----------------------------------------------------------------------===//
#include "tapi/Core/APIPrinter.h"
#include "tapi/Diagnostics/Diagnostics.h"
#include "tapi/Frontend/Frontend.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Format.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Process.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace TAPI_INTERNAL;

static cl::OptionCategory tapiCategory("tapi-frontend options");

static cl::list<std::string> targets("target", cl::desc("target triple"),
                                     cl::value_desc("triple"),
                                     cl::cat(tapiCategory));

static cl::opt<std::string>
    isysroot("isysroot", cl::desc("Set the system root directory (usually /)"),
             cl::value_desc("dir"), cl::cat(tapiCategory));

static cl::opt<std::string> language_std("std",
                                         cl::desc("Set the language standard"),
                                         cl::value_desc("<lang>"),
                                         cl::cat(tapiCategory));

static cl::list<std::string> xparser("Xparser", cl::cat(tapiCategory));

static cl::opt<std::string> inputFilename(cl::Positional,
                                          cl::desc("<input file>"),
                                          cl::cat(tapiCategory));

static cl::opt<bool> verbose("v", cl::desc("verbose"), cl::cat(tapiCategory));
static cl::opt<bool> noColors("no-colors", cl::desc("don't use color output"),
                              cl::cat(tapiCategory));
static cl::opt<bool> noPrint("no-print", cl::desc("don't print the API"),
                             cl::cat(tapiCategory));

int main(int argc, const char *argv[]) {
  // Standard set up, so program fails gracefully.
  sys::PrintStackTraceOnErrorSignal(argv[0]);
  PrettyStackTraceProgram stackPrinter(argc, argv);
  llvm_shutdown_obj shutdown;

  if (sys::Process::FixupStandardFileDescriptors())
    return 1;

  cl::HideUnrelatedOptions(tapiCategory);
  cl::ParseCommandLineOptions(argc, argv, "TAPI Frontend Tool\n");

  if (inputFilename.empty()) {
    cl::PrintHelpMessage();
    return 0;
  }

  std::vector<FrontendContext> results;
  for (const auto &target : targets) {
    FrontendJob job;

    job.target = Triple(target);
    job.isysroot = isysroot;
    job.language_std = language_std;
    job.verbose = verbose;
    job.clangExtraArgs = xparser;
    auto result = runFrontend(job, inputFilename);
    if (!result)
      return -1;
    results.emplace_back(std::move(result.getValue()));
  }

  if (!noPrint) {
    for (auto &result : results) {
      APIPrinter printer(errs(), !noColors);
      result.visit(printer);
    }
  }

  return 0;
}
