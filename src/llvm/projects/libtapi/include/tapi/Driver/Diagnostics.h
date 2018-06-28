//===--- Diagnostic.h - TAPI Diagnostics Handling ---------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_DIAGNOSTIC_H
#define TAPI_CORE_DIAGNOSTIC_H

#include "tapi/Core/LLVM.h"
#include "tapi/Defines.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticIDs.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"

namespace llvm {
raw_ostream &errs();
}

TAPI_NAMESPACE_INTERNAL_BEGIN

namespace diag {
using Severity = clang::diag::Severity;

enum {
  DIAG_START_TAPI = clang::diag::DIAG_UPPER_LIMIT,
#define DIAG(ENUM, FLAGS, DEFAULT_MAPPING, DESC, GROUP, SFINAE, CATEGORY,      \
             NOWERROR, SHOWINSYSHEADER)                                        \
  ENUM,
#include "tapi/Driver/DiagnosticTAPIKinds.inc"
#undef DIAG
};
} // end namespace diag

/// \brief The Tapi diagnostic engine.
///
/// This wraps the Clang diagnostic engine to augment it with additonal Tapi
/// specific diagnostics.
class DiagnosticsEngine : public llvm::RefCountedBase<DiagnosticsEngine> {
public:
  explicit DiagnosticsEngine(raw_ostream &errorStream = llvm::errs());
  DiagnosticsEngine(const DiagnosticsEngine &) = delete;
  void operator=(const DiagnosticsEngine &) = delete;

  clang::DiagnosticBuilder report(unsigned diagID);
  void setWarningsAsErrors(bool value) { warningsAsErrors = value; }
  void setErrorLimit(unsigned value) { diag->setErrorLimit(value); }
  bool hasErrorOccurred() const { return diag->hasErrorOccurred(); }

private:
  clang::DiagnosticIDs::Level getDiagnosticLevel(unsigned diagID);
  IntrusiveRefCntPtr<clang::DiagnosticOptions> diagOpts;
  IntrusiveRefCntPtr<clang::DiagnosticsEngine> diag;
  bool warningsAsErrors = false;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_DIAGNOSTIC_H
