//===- lib/Core/Diagnostics.cpp - TAPI Diagnostic Handling ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the diagnostic handling for tapi.
///
//===----------------------------------------------------------------------===//

#include "tapi/Driver/Diagnostics.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Core/STLExtras.h"
#include "tapi/Defines.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Process.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

namespace {

// Diagnostic classes.
enum { CLASS_WARNING, CLASS_ERROR };

struct DiagInfoRec {
  const char *DescriptionStr;
  uint16_t id;
  uint16_t DescriptionLen;
  uint8_t DefaultSeverity;

  StringRef getDescription() const { return {DescriptionStr, DescriptionLen}; }

  bool operator<(const DiagInfoRec &rhs) const { return id < rhs.id; }
};

} // end anonymous namespace.

static constexpr DiagInfoRec diagInfo[] = {
#define DIAG(ENUM, CLASS, DEFAULT_SEVERITY, DESC, GROUP, SFINAE, NOWERROR,     \
             SHOWINSYSHEADER, CATEGORY)                                        \
  {DESC, diag::ENUM, sizeof(DESC) - 1, DEFAULT_SEVERITY},
#include "tapi/Driver/DiagnosticTAPIKinds.inc"
#undef DIAG
};

static constexpr unsigned diagInfoSize = llvm::array_lengthof(diagInfo);

static clang::DiagnosticOptions *createDiagnosticsEngineOpts() {
  static bool hasColors = llvm::sys::Process::StandardErrHasColors();

  auto *diagOpts = new clang::DiagnosticOptions();
  diagOpts->ShowColors = static_cast<unsigned int>(hasColors);
  return diagOpts;
}

DiagnosticsEngine::DiagnosticsEngine(raw_ostream &errorStream) {
  diagOpts = createDiagnosticsEngineOpts();
  diag = new clang::DiagnosticsEngine(
      new clang::DiagnosticIDs(), diagOpts.get(),
      new clang::TextDiagnosticPrinter(errorStream, diagOpts.get()));

  assert(is_sorted(diagInfo) && "diagnostic array should be sorted");
}

clang::DiagnosticIDs::Level
DiagnosticsEngine::getDiagnosticLevel(unsigned diagID) {
  // lookup the diagnostic descripton from the Tapi DiagInfo.
  auto index = diagID - clang::diag::DIAG_UPPER_LIMIT - 1;
  assert(index < diagInfoSize && "invalid Tapi DiagInfo index");
  auto &record = diagInfo[index];
  auto level = static_cast<clang::DiagnosticIDs::Level>(record.DefaultSeverity);

  if (level == clang::DiagnosticIDs::Warning && warningsAsErrors)
    level = clang::DiagnosticIDs::Error;

  return level;
}

clang::DiagnosticBuilder DiagnosticsEngine::report(unsigned diagID) {
  // pass through all clang diagnostics.
  if (diagID <= clang::diag::DIAG_UPPER_LIMIT)
    return diag->Report(diagID);

  // lookup the diagnostic descripton from the Tapi DiagInfo.
  auto index = diagID - clang::diag::DIAG_UPPER_LIMIT - 1;
  assert(index < diagInfoSize && "invalid Tapi DiagInfo index");
  (void)diagInfoSize;
  auto &record = diagInfo[index];
  auto diagIDs = diag->getDiagnosticIDs();
  auto level = getDiagnosticLevel(diagID);
  unsigned newID = diagIDs->getCustomDiagID(level, record.getDescription());
  return diag->Report(newID);
}

TAPI_NAMESPACE_INTERNAL_END
