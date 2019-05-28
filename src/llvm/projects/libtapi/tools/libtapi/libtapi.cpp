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
// static initializers.
raw_ostream &dbgs() { return errs(); }

#ifndef NDEBUG
#undef isCurrentDebugType
bool isCurrentDebugType(const char *Type) { abort(); }

bool DebugFlag = false;
#endif

void report_bad_alloc_error(const char *, bool) { abort(); }

void report_fatal_error(const char *, bool) { abort(); }

void report_fatal_error(const std::string &, bool) { abort(); }

void report_fatal_error(StringRef, bool) { abort(); }

void report_fatal_error(const Twine &, bool) { abort(); }

void llvm_unreachable_internal(const char *, const char *, unsigned) {
  abort();
}

namespace sys {

#ifdef __APPLE__

bool RemoveFileOnSignal(StringRef Filename, std::string *ErrMsg) { abort(); }

void DontRemoveFileOnSignal(StringRef Filename) { abort(); }

void AddSignalHandler(void (*)(void *), void *) { abort(); }

#endif

} // end namespace sys.

} // end namespace llvm.
