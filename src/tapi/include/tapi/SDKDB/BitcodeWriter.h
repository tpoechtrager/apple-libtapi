//===- tapi/SDKDB/BitcodeWriter.h - TAPI SDKDB Bitcode Writer ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief SDKDB bitcode writer.
///
//===----------------------------------------------------------------------===//
#ifndef TAPI_SDKDB_BITCODE_WRTIER_H
#define TAPI_SDKDB_BITCODE_WRTIER_H

#include "tapi/Core/API.h"
#include "tapi/Defines.h"
#include "tapi/SDKDB/SDKDB.h"
#include "llvm/Support/raw_ostream.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

class SDKDBBitcodeWriter {
public:
  SDKDBBitcodeWriter() = default;

  void writeSDKDBToStream(const SDKDBBuilder &builder, raw_ostream &os);
};

TAPI_NAMESPACE_INTERNAL_END

#endif /* TAPI_SDKDB_BITCODE_WRTIER_H */
