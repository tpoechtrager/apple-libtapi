//===- tapi/Core/ClangDiagnostics.h - TAPI Diagnostics Streams --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Stream Operator Declaration for llvm/TextAPI Types &
/// clang/Diagnostics
///
//===----------------------------------------------------------------------===//
//
#ifndef TAPI_CLANG_DIAGNOSTICS_H
#define TAPI_CLANG_DIAGNOSTICS_H

#include "tapi/Core/LLVM.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/Diagnostic.h"
#include "llvm/TextAPI/MachO/Architecture.h"
#include "llvm/TextAPI/MachO/ArchitectureSet.h"
#include "llvm/TextAPI/MachO/Platform.h"
namespace llvm {
namespace MachO {

const clang::DiagnosticBuilder &operator<<(const clang::DiagnosticBuilder &db,
                                           PlatformKind platform);

const clang::DiagnosticBuilder &operator<<(const clang::DiagnosticBuilder &db,
                                           PlatformSet platforms);

const clang::DiagnosticBuilder &operator<<(const clang::DiagnosticBuilder &db,
                                           Architecture arch);

const clang::DiagnosticBuilder &operator<<(const clang::DiagnosticBuilder &db,
                                           ArchitectureSet architectureSet);
} // namespace MachO
} // namespace llvm
#endif // TAPI_CLANG_DIAGNOSTICS_H
