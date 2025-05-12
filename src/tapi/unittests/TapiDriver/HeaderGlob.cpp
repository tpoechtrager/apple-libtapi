//===- unittests/TapiDriver/HeaderGlob.cpp - Glob Tests -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "tapi/Driver/HeaderGlob.h"
#include "gtest/gtest.h"
#define DEBUG_TYPE "glob-tests"

using namespace llvm;
using namespace tapi::internal;

TEST(HeaderGlob, matchWildcard) {
  auto glob = HeaderGlob::create("*.h", HeaderType::Public);
  EXPECT_TRUE(!!glob);
  EXPECT_TRUE(glob.get()->match({"foo.h", HeaderType::Public}));
  EXPECT_TRUE(glob.get()->match({"bar.h", HeaderType::Public}));
  EXPECT_FALSE(glob.get()->match({"foo.h", HeaderType::Private}));
  EXPECT_FALSE(glob.get()->match({"bar.hpp", HeaderType::Public}));
  EXPECT_FALSE(glob.get()->match({"bar.c", HeaderType::Public}));
  EXPECT_FALSE(glob.get()->match({"/baz/bar.h", HeaderType::Public}));
}

TEST(HeaderGlob, matchGlob) {
  auto glob = HeaderGlob::create("**/*.h", HeaderType::Public);
  EXPECT_TRUE(!!glob);
  EXPECT_TRUE(glob.get()->match({"/foo.h", HeaderType::Public}));
  EXPECT_TRUE(glob.get()->match({"/bar.h", HeaderType::Public}));
  EXPECT_TRUE(glob.get()->match({"/baz/bar.h", HeaderType::Public}));
  EXPECT_FALSE(glob.get()->match({"/bar.c", HeaderType::Public}));
  EXPECT_FALSE(glob.get()->match({"/baz/bar.hpp", HeaderType::Public}));
}
