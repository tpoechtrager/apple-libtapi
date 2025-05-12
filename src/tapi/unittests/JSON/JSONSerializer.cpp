//===- unittests/JSONSerializer/JSONSerializer.cpp - JSON Serializer Test -===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "tapi/Core/APIJSONSerializer.h"

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"
#include "gtest/gtest.h"
#define DEBUG_TYPE "json-test"

using namespace llvm;
using namespace tapi::internal;

namespace {

// Round trip serialization test.
// Make sure the input and output are exactly the same.
TEST(JSON, Serializer) {
  SmallVector<char, PATH_MAX> inputPath;
  llvm::sys::path::append(inputPath, INPUT_PATH, "binary.json");
  auto inputBuf = MemoryBuffer::getFile(inputPath);
  EXPECT_TRUE(inputBuf);

  auto api = APIJSONSerializer::parse((*inputBuf)->getBuffer());
  EXPECT_FALSE(!api);

  std::string output;
  raw_string_ostream os(output);
  APIJSONOption options{};
  APIJSONSerializer serializer(*api, options);
  serializer.serialize(os);

  EXPECT_STREQ((*inputBuf)->getBuffer().str().c_str(), os.str().c_str());
}

TEST(JSON, Serializer2) {
  SmallVector<char, PATH_MAX> inputPath;
  llvm::sys::path::append(inputPath, INPUT_PATH, "frontend.json");
  auto inputBuf = MemoryBuffer::getFile(inputPath);
  EXPECT_TRUE(inputBuf);

  auto api = APIJSONSerializer::parse((*inputBuf)->getBuffer());
  EXPECT_FALSE(!api);

  std::string output;
  raw_string_ostream os(output);
  APIJSONOption options{};
  APIJSONSerializer serializer(*api, options);
  serializer.serialize(os);

  EXPECT_STREQ((*inputBuf)->getBuffer().str().c_str(), os.str().c_str());
}

} // end anonymous namespace.
