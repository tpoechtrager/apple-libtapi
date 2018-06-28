//===- lib/Core/Utils.cpp - TAPI Utility Methods ----------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Misc utility methods.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/Utils.h"
#include "tapi/Defines.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/Path.h"

using namespace llvm;

TAPI_NAMESPACE_INTERNAL_BEGIN

bool isPublicLocation(StringRef path) {
  // Only libraries directly in /usr/lib are public. All other libraries in
  // sub-directories (such as /usr/lib/system) are considered private.
  if (path.startswith("/usr/lib/") &&
      path.find('/', sizeof("/usr/lib/")) == StringRef::npos)
    return true;

  // /System/Library/Frameworks/ is a public location
  if (path.startswith("/System/Library/Frameworks/")) {
    StringRef name, rest;
    std::tie(name, rest) =
        path.drop_front(sizeof("/System/Library/Frameworks/")).split('.');

    // but only top level framework
    // /System/Library/Frameworks/Foo.framework/Foo ==> true
    // /System/Library/Frameworks/Foo.framework/Versions/A/Foo ==> true
    // /System/Library/Frameworks/Foo.framework/Resources/libBar.dylib ==> false
    // /System/Library/Frameworks/Foo.framework/Frameworks/Bar.framework/Bar
    // ==> false
    // /System/Library/Frameworks/Foo.framework/Frameworks/Xfoo.framework/XFoo
    // ==> false
    if (rest.startswith("framework/") && rest.endswith(name))
      return true;

    return false;
  }

  return false;
}

bool isHeaderFile(StringRef path) {
  return StringSwitch<bool>(sys::path::extension(path))
      .Cases(".h", ".H", ".hh", ".hpp", ".hxx", true)
      .Default(false);
}

TAPI_NAMESPACE_INTERNAL_END
