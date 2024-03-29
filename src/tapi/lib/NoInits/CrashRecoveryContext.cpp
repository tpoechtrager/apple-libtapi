//===- CrashRecoveryContext.cpp ---------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "llvm/Support/CrashRecoveryContext.h"
#include <cstdlib>

namespace llvm {

bool CrashRecoveryContext::isRecoveringFromCrash() { abort(); }
CrashRecoveryContext *CrashRecoveryContext::GetCurrent() { abort(); }
void CrashRecoveryContext::HandleExit(int) { abort(); }

} // end namespace llvm.
