//===- Statistic.cpp ----------------------------------------------*- C++
//-*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
#include <cstdlib>

namespace llvm {
namespace TrackingStatistic {

//TODO: determine different avenues to remove the static initializer from upstream Statistic.cpp.o
void RegisterStatistic() { abort(); }

} // namespace TrackingStatistic
} // namespace llvm
