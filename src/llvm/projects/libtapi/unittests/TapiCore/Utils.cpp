//===- unittests/Utils/Utils.cpp - Utils Test -----------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "tapi/Core/Utils.h"
#include "llvm/ADT/SmallString.h"
#include "gtest/gtest.h"
#define DEBUG_TYPE "utils-test"

using namespace llvm;
using namespace tapi::internal;

TEST(Utils, isPublicLocation) {
  static const char *publicPaths[] = {
      "/usr/lib/libfoo.dylib",
      "/System/Library/Frameworks/Foo.framework/Foo",
      "/System/Library/Frameworks/Foo.framework/Versions/A/Foo",
      "/System/Library/Frameworks/Public.framework/Versions/A/Public",
  };

  static const char *privatePaths[] = {
      "/usr/lib/system/libsystem_foo.dylib",
      "/System/Library/Frameworks/Foo.framework/Resources/libBar.dylib",
      "/System/Library/Frameworks/Foo.framework/Frameworks/Bar.framework/Bar",
      "/System/Library/Frameworks/Foo.framework/Frameworks/XFoo.framework/XFoo",
  };

  for (const char *path : publicPaths)
    EXPECT_TRUE(isPublicLocation(path));

  for (const char *path : privatePaths)
    EXPECT_FALSE(isPublicLocation(path));
}
