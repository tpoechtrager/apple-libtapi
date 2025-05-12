//===- unittests/HeaderFile/HeaderFile.cpp - HeaderFile Test --------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "tapi/Core/HeaderFile.h"
#include "gtest/gtest.h"
#define DEBUG_TYPE "headerfile-test"

using namespace llvm;
using namespace tapi::internal;

TEST(HeaderFile, frameworks_includeName) {
  const char *path = "/System/Library/Frameworks/Foo.framework/Headers/Foo.h";
  auto includeName = createIncludeHeaderName(path);
  EXPECT_TRUE(includeName.has_value());
  EXPECT_STREQ(includeName.value().c_str(), "Foo/Foo.h");

  path = "/System/Library/Frameworks/Foo.framework/Frameworks/Bar.framework/"
         "Headers/SimpleBar.h";
  includeName = createIncludeHeaderName(path);
  EXPECT_TRUE(includeName.has_value());
  EXPECT_STREQ(includeName.value().c_str(), "Bar/SimpleBar.h");

  path = "/tmp/Foo.framework/Versions/A/Headers/SimpleFoo.h";
  includeName = createIncludeHeaderName(path);
  EXPECT_TRUE(includeName.has_value());
  EXPECT_STREQ(includeName.value().c_str(), "Foo/SimpleFoo.h");

  path = "/System/Library/PrivateFrameworks/Foo.framework/Headers/Foo.h";
  includeName = createIncludeHeaderName(path);
  EXPECT_TRUE(includeName.has_value());
  EXPECT_STREQ(includeName.value().c_str(), "Foo/Foo.h");

  path = "/Developer/Library/Frameworks/"
         "HelloFramework.framework/Headers/HelloFramework.h";
  includeName = createIncludeHeaderName(path);
  EXPECT_TRUE(includeName.has_value());
  EXPECT_STREQ(includeName.value().c_str(), "HelloFramework/HelloFramework.h");

  path = "/tmp/BuildProducts/Foo.framework/Versions/A/"
         "PrivateHeaders/Foo+Private.h";
  includeName = createIncludeHeaderName(path);
  EXPECT_TRUE(includeName.has_value());
  EXPECT_STREQ(includeName.value().c_str(), "Foo/Foo+Private.h");

  path = "/Applications/Xcode.app/Contents/Developer/SDKS/MacOS.sdk/System/"
         "Library/Frameworks/Foo.framework/PrivateHeaders/Foo_Private.h";
  includeName = createIncludeHeaderName(path);
  EXPECT_TRUE(includeName.has_value());
  EXPECT_STREQ(includeName.value().c_str(), "Foo/Foo_Private.h");

  path =
      "/System/Library/PrivateFrameworks/Foo.framework/PrivateHeaders/Foo.hpp";
  includeName = createIncludeHeaderName(path);
  EXPECT_TRUE(includeName.has_value());
  EXPECT_STREQ(includeName.value().c_str(), "Foo/Foo.hpp");

  path = "/Applications/Xcode.app/Contents/Developer/SDKS/MacOS.sdk/System/"
         "Library/Frameworks/Foo.framework/Headers/BarDir/Bar.h";
  includeName = createIncludeHeaderName(path);
  EXPECT_TRUE(includeName.has_value());
  EXPECT_STREQ(includeName.value().c_str(), "Foo/BarDir/Bar.h");
}

TEST(HeaderFile, dylib_includeName) {
  const char *path = "/usr/include/foo.h";
  auto includeName = createIncludeHeaderName(path);
  EXPECT_TRUE(includeName.has_value());
  EXPECT_STREQ(includeName.value().c_str(), "foo.h");

  path = "/tmp/BuildProducts/usr/include/a/A.h";
  includeName = createIncludeHeaderName(path);
  EXPECT_TRUE(includeName.has_value());
  EXPECT_STREQ(includeName.value().c_str(), "a/A.h");

  path = "/Applications/Xcode.app/Contents/Developer/SDKS/MacOS.sdk/"
         "usr/include/simd/types.h";
  includeName = createIncludeHeaderName(path);
  EXPECT_TRUE(includeName.has_value());
  EXPECT_STREQ(includeName.value().c_str(), "simd/types.h");

  path = "/usr/local/include/hidden/A.h";
  includeName = createIncludeHeaderName(path);
  EXPECT_TRUE(includeName.has_value());
  EXPECT_STREQ(includeName.value().c_str(), "hidden/A.h");
}
