//===- include/Core/HeaderFile.h - Header File ------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Header file enums and defines.
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_HEADER_FILE_H
#define TAPI_CORE_HEADER_FILE_H

#include "tapi/Defines.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

enum class HeaderType {
  Public,
  Private,
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_CORE_HEADER_FILE_H
