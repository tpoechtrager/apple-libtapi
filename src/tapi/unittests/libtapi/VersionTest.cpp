//===- unittests/libtapi/VersionTest.cpp - libtapi Version Interface Test -===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "gtest/gtest.h"
#include <tapi/tapi.h>
#define DEBUG_TYPE "libtapi-test"

#define MAKE_STRING2(X) #X
#define MAKE_STRING(X) MAKE_STRING2(X)

// Test the basic version query methods.
TEST(LibTapiTest, Version_getMajor) {
  EXPECT_EQ(tapi::Version::getMajor(), TAPI_VERSION_MAJOR);
}

TEST(LibTapiTest, Version_getMinor) {
  EXPECT_EQ(tapi::Version::getMinor(), TAPI_VERSION_MINOR);
}

TEST(LibTapiTest, Version_getPatch) {
  EXPECT_EQ(tapi::Version::getPatch(), TAPI_VERSION_PATCH);
}

TEST(LibTapiTest, Version_getAsString) {
  EXPECT_STREQ(tapi::Version::getAsString().c_str(), MAKE_STRING(TAPI_VERSION));
}

TEST(LibTapiTest, Version_getFullVersionAsString) {
  std::string vendor;
#ifdef TAPI_VENDOR
  vendor = TAPI_VENDOR;
#endif
#ifdef TAPI_REPOSITORY_STRING
  EXPECT_EQ(tapi::Version::getFullVersionAsString(),
            vendor + "TAPI version " MAKE_STRING(
                         TAPI_VERSION) " (" TAPI_REPOSITORY_STRING ")");
#else
  EXPECT_EQ(tapi::Version::getFullVersionAsString(),
            vendor + "TAPI version " MAKE_STRING(TAPI_VERSION));
#endif
}

// Test the version comparison method.
TEST(LibTapiTest, Version_isAtLeast) {
  EXPECT_TRUE(tapi::Version::isAtLeast(TAPI_VERSION_MAJOR));
  EXPECT_TRUE(tapi::Version::isAtLeast(TAPI_VERSION_MAJOR, TAPI_VERSION_MINOR));
  EXPECT_TRUE(tapi::Version::isAtLeast(TAPI_VERSION_MAJOR, TAPI_VERSION_MINOR,
                                       TAPI_VERSION_PATCH));

  EXPECT_FALSE(tapi::Version::isAtLeast(TAPI_VERSION_MAJOR + 1U));
  EXPECT_FALSE(
      tapi::Version::isAtLeast(TAPI_VERSION_MAJOR, TAPI_VERSION_MINOR + 1U));
  EXPECT_FALSE(tapi::Version::isAtLeast(TAPI_VERSION_MAJOR, TAPI_VERSION_MINOR,
                                        TAPI_VERSION_PATCH + 1U));

  if (TAPI_VERSION_MAJOR > 0) {
    EXPECT_TRUE(tapi::Version::isAtLeast(TAPI_VERSION_MAJOR - 1U));
    EXPECT_TRUE(tapi::Version::isAtLeast(TAPI_VERSION_MAJOR - 1U, ~0U));
    EXPECT_TRUE(tapi::Version::isAtLeast(TAPI_VERSION_MAJOR - 1U, ~0U, ~0U));
  }
  if (TAPI_VERSION_MINOR > 0) {
    EXPECT_TRUE(
        tapi::Version::isAtLeast(TAPI_VERSION_MAJOR, TAPI_VERSION_MINOR - 1U));
    EXPECT_TRUE(tapi::Version::isAtLeast(TAPI_VERSION_MAJOR,
                                         TAPI_VERSION_MINOR - 1U, ~0U));
  }
  if (TAPI_VERSION_PATCH > 0) {
    EXPECT_TRUE(tapi::Version::isAtLeast(TAPI_VERSION_MAJOR, TAPI_VERSION_MINOR,
                                         TAPI_VERSION_PATCH - 1U));
  }
}
