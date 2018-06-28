//===- libtapi/libtapi.cpp - TAPI library code ------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements error handling for library.
///
//===----------------------------------------------------------------------===//

#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/Twine.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdlib>
#include <string>

namespace llvm {

// Prevent the inclusion of lib/Support/Debug.cpp, because we don't want the
// static initializers. Only do this for release mode.
#ifdef NDEBUG
raw_ostream &dbgs() { return errs(); }
#endif

void report_bad_alloc_error(const char *, bool) { abort(); }

void report_fatal_error(const char *, bool) { abort(); }

void report_fatal_error(const std::string &, bool) { abort(); }

void report_fatal_error(StringRef, bool) { abort(); }

void report_fatal_error(const Twine &, bool) { abort(); }

void llvm_unreachable_internal(const char *, const char *, unsigned) {
  abort();
}

} // end namespace llvm.
