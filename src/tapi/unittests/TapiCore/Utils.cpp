//===- unittests/Utils/Utils.cpp - Utils Test -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "tapi/Core/Utils.h"
#include "llvm/ADT/SmallString.h"
#include "gtest/gtest.h"
#define DEBUG_TYPE "utils-test"

using namespace llvm;
using namespace tapi::internal;

static const char *publicSDKInstallNames[] = {
    "/usr/lib/libfoo.dylib",
    "/usr/lib/swift/libswiftCore.dylib",
    "/usr/lib/swift/subfolder/libswift_internal.dylib",
    "/System/Library/Frameworks/Foo.framework/Foo",
    "/System/Library/Frameworks/Foo.framework/Versions/A/Foo",
    "/System/Library/Frameworks/Public.framework/Versions/A/Public",
    "/System/iOSSupport/usr/lib/libfoo.dylib",
    "/System/iOSSupport/System/Library/Frameworks/Foo.framework/Foo",
    "/System/iOSSupport/System/Library/Frameworks/Foo.framework/Versions/A/Foo",
    "/System/iOSSupport/System/Library/Frameworks/Public.framework/Versions/"
    "A/Public",
    "/System/DriverKit/usr/lib/libfoo.dylib",
    "/Library/Apple/System/Library/Frameworks/Foo.framework/Foo",
    "/System/Cryptexes/OS/System/Library/Frameworks/WebKit.framework/WebKit",
    "/System/Cryptexes/OS/System/iOSSupport/System/Library/Frameworks/"
    "WebKit.framework/WebKit",
};

static const char *privateSDKInstallNames[] = {
    "/usr/lib/system/libsystem_foo.dylib",
    "/System/Library/Frameworks/Foo.framework/Resources/libBar.dylib",
    "/System/Library/Frameworks/Foo.framework/Frameworks/Bar.framework/Bar",
    "/System/Library/Frameworks/Foo.framework/Frameworks/XFoo.framework/XFoo",
    "/System/iOSSupport/usr/lib/system/libsystem_foo.dylib",
    "/System/iOSSupport/System/Library/Frameworks/Foo.framework/Resources/"
    "libBar.dylib",
    "/System/iOSSupport/System/Library/Frameworks/Foo.framework/Frameworks/"
    "Bar.framework/Bar",
    "/System/iOSSupport/System/Library/Frameworks/Foo.framework/Frameworks/"
    "XFoo.framework/XFoo",
    "/System/DriverKit/usr/local/lib/libfoo.dylib",
    "/Library/Apple/System/Library/PrivateFrameworks/Foo.framework/Foo",
    "/System/Cryptexes/OS/System/Library/PrivateFrameworks/Safari.framework/"
    "Safari",
    "/System/Cryptexes/OS/System/iOSSupport/System/Library/PrivateFrameworks/"
    "WebCore.framework/WebCore",
};

static const char *nonSDKInstallNames[] = {
    "@rpath/Foo.framework/Foo",
    "/System/Library/Frameworks/Internal.framework/Internal",
    "/dev/null",
};

static const char *publicSDKPaths[] = {
    "/usr/include/xcselect.h",
    "/usr/include/vfs/vfs_support.h",
    "/System/Library/Frameworks/Foo.framework/Headers/Foo.h",
    "/System/Cryptexes/OS/System/Library/Frameworks/Foo.framework/Headers/"
    "Foo.h",
    "/System/Cryptexes/OS/System/iOSSupport/usr/include/header.h",
    "/System/DriverKit/System/Library/Frameworks/Driver.framework/Modules/"
    "module.modulemap",
    "/Library/Apple/System/Library/Frameworks/Bar.framework/Resources/bar.dat",
};

static const char *nonPublicSDKPaths[] = {
    "/usr/local/include/xcselect_private.h",
    "/usr/local/include/private/private.h",
    "/System/Library/Frameworks/Internal.framework/Headers/"
    "Interhal.h",
    "/System/Library/PrivateFrameworks/Foo.framework/Headers/Foo.h",
    "/System/Library/Frameworks/Foo.framework/PrivateHeaders/Foo.h",
    "/System/Cryptexes/OS/System/Library/PrivateFrameworks/Foo.framework/"
    "Headers/Foo.h",
    "/System/Cryptexes/OS/System/iOSSupport/usr/local/include/private.h",
    "/System/DriverKit/System/Library/PrivateFrameworks/Driver.framework/"
    "Modules/module.modulemap",
};

TEST(Utils, isPublicDylib) {
  for (const char *installName : publicSDKInstallNames)
    EXPECT_TRUE(isPublicDylib(installName));

  for (const char *installName : privateSDKInstallNames)
    EXPECT_FALSE(isPublicDylib(installName));

  for (const char *installName : nonSDKInstallNames)
    EXPECT_FALSE(isPublicDylib(installName));
}

TEST(Utils, isSDKDylib) {
  for (const char *installName : publicSDKInstallNames)
    EXPECT_TRUE(isSDKDylib(installName));

  for (const char *installName : privateSDKInstallNames)
    EXPECT_TRUE(isSDKDylib(installName));

  for (const char *installName : nonSDKInstallNames)
    EXPECT_FALSE(isSDKDylib(installName));
}

TEST(Utils, isWithinPublicLocation) {
  for (const char *installName : publicSDKInstallNames)
    EXPECT_TRUE(isWithinPublicLocation(installName));

  for (const char *path : publicSDKPaths)
    EXPECT_TRUE(isWithinPublicLocation(path));

  for (const char *path : nonPublicSDKPaths)
    EXPECT_FALSE(isWithinPublicLocation(path));
}
