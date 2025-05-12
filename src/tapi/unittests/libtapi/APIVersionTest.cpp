//===- /u/libtapi/APIVersionTest.cpp - libtapi API Version Interface Test -===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "gtest/gtest.h"
#include <tapi/tapi.h>
#define DEBUG_TYPE "libtapi-test"

// Test the basic API Version query methods.
TEST(LibTapiTest, APIVersion_getMajor) {
  EXPECT_EQ(tapi::APIVersion::getMajor(), TAPI_API_VERSION_MAJOR);
}

TEST(LibTapiTest, APIVersion_getMinor) {
  EXPECT_EQ(tapi::APIVersion::getMinor(), TAPI_API_VERSION_MINOR);
}

TEST(LibTapiTest, APIVersion_getPatch) {
  EXPECT_EQ(tapi::APIVersion::getPatch(), TAPI_API_VERSION_PATCH);
}

// Test the APIVersion comparison method.
TEST(LibTapiTest, APIVersion_isAtLeast) {
  EXPECT_TRUE(tapi::APIVersion::isAtLeast(TAPI_API_VERSION_MAJOR));
  EXPECT_TRUE(tapi::APIVersion::isAtLeast(TAPI_API_VERSION_MAJOR,
                                          TAPI_API_VERSION_MINOR));
  EXPECT_TRUE(tapi::APIVersion::isAtLeast(
      TAPI_API_VERSION_MAJOR, TAPI_API_VERSION_MINOR, TAPI_API_VERSION_PATCH));

  EXPECT_FALSE(tapi::APIVersion::isAtLeast(TAPI_API_VERSION_MAJOR + 1U));
  EXPECT_FALSE(tapi::APIVersion::isAtLeast(TAPI_API_VERSION_MAJOR,
                                           TAPI_API_VERSION_MINOR + 1U));
  EXPECT_FALSE(tapi::APIVersion::isAtLeast(TAPI_API_VERSION_MAJOR,
                                           TAPI_API_VERSION_MINOR,
                                           TAPI_API_VERSION_PATCH + 1U));

  if (TAPI_API_VERSION_MAJOR > 0) {
    EXPECT_TRUE(tapi::APIVersion::isAtLeast(TAPI_API_VERSION_MAJOR - 1U));
    EXPECT_TRUE(tapi::APIVersion::isAtLeast(TAPI_API_VERSION_MAJOR - 1U, ~0U));
    EXPECT_TRUE(
        tapi::APIVersion::isAtLeast(TAPI_API_VERSION_MAJOR - 1U, ~0U, ~0U));
  }
  if (TAPI_API_VERSION_MINOR > 0) {
    EXPECT_TRUE(tapi::APIVersion::isAtLeast(TAPI_API_VERSION_MAJOR,
                                            TAPI_API_VERSION_MINOR - 1U));
    EXPECT_TRUE(tapi::APIVersion::isAtLeast(TAPI_API_VERSION_MAJOR,
                                            TAPI_API_VERSION_MINOR - 1U, ~0U));
  }
  if (TAPI_API_VERSION_PATCH > 0) {
    EXPECT_TRUE(tapi::APIVersion::isAtLeast(TAPI_API_VERSION_MAJOR,
                                            TAPI_API_VERSION_MINOR,
                                            TAPI_API_VERSION_PATCH - 1U));
  }
}

// Test the ABI method.
TEST(LibTapiTest, APIVersion_hasABI) {
  // There never has been an ABI v0.
  EXPECT_FALSE(tapi::APIVersion::hasABI(0));

  // Currently we only have ABI v1.
  EXPECT_TRUE(tapi::APIVersion::hasABI(1));

  EXPECT_FALSE(tapi::APIVersion::hasABI(2));
}
