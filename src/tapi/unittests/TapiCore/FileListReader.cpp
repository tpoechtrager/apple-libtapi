//===- unittests/TapiCore/FileListReader.cpp - File List Reader Tests -----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "tapi/Core/FileListReader.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"
#include "gtest/gtest.h"
#define DEBUG_TYPE "file-list-reader-test"

using namespace llvm;
using namespace tapi::internal;

TEST(FileListReader, version1) {
  static const auto input = "{\n"
                            "  \"version\" : \"1\",\n"
                            "  \"headers\" : [\n"
                            "    {\n"
                            "      \"type\" : \"public\",\n"
                            "      \"path\" : \"/usr/include/foo.h\"\n"
                            "    },\n"
                            "    {\n"
                            "      \"type\" : \"private\",\n"
                            "      \"path\" : \"/usr/local/include/bar.h\"\n"
                            "    }\n"
                            "  ]\n"
                            "}\n";
  auto inputMemBuf = MemoryBuffer::getMemBuffer(input);
  auto reader = FileListReader::get(std::move(inputMemBuf));
  ASSERT_TRUE(!!reader);

  class Visitor : public FileListReader::Visitor {
  public:
    std::map<std::string, HeaderType> headers;

    void visitHeaderFile(FileListReader::HeaderInfo &header) override {
      headers[header.path] = header.type;
    }
  };

  Visitor visitor;
  (*reader)->visit(visitor);

  EXPECT_EQ(2U, visitor.headers.size());
  EXPECT_EQ(HeaderType::Public, visitor.headers["/usr/include/foo.h"]);
  EXPECT_EQ(HeaderType::Private, visitor.headers["/usr/local/include/bar.h"]);
}

TEST(FileListReader, no_version) {
  static const auto *input = "{\n"
                             "  \"headers\" : [\n"
                             "    {\n"
                             "      \"type\" : \"public\",\n"
                             "      \"path\" : \"/usr/include/foo.h\"\n"
                             "    },\n"
                             "    {\n"
                             "      \"type\" : \"private\",\n"
                             "      \"path\" : \"/usr/local/include/bar.h\"\n"
                             "    }\n"
                             "  ]\n"
                             "}\n";
  auto inputMemBuf = MemoryBuffer::getMemBuffer(input);
  auto reader = FileListReader::get(std::move(inputMemBuf));
  EXPECT_FALSE(!!reader);
  consumeError(reader.takeError());
}

TEST(FileListReader, invalid_type) {
  static const auto input = "{\n"
                            "  \"version\" : \"1\",\n"
                            "  \"headers\" : [\n"
                            "    {\n"
                            "      \"type\" : \"project\",\n"
                            "      \"path\" : \"/usr/include/foo.h\"\n"
                            "    }\n"
                            "  ]\n"
                            "}\n";
  auto inputMemBuf = MemoryBuffer::getMemBuffer(input);
  auto reader = FileListReader::get(std::move(inputMemBuf));
  EXPECT_FALSE(!!reader);
  EXPECT_STREQ("unsupported header type", toString(reader.takeError()).c_str());
}

TEST(FileListReader, version2) {
  static const auto input = "{\n"
                            "  \"version\" : \"2\",\n"
                            "  \"headers\" : [\n"
                            "    {\n"
                            "      \"type\" : \"public\",\n"
                            "      \"path\" : \"/usr/include/foo.h\"\n"
                            "    },\n"
                            "    {\n"
                            "      \"type\" : \"project\",\n"
                            "      \"path\" : \"bar.h\"\n"
                            "    }\n"
                            "  ]\n"
                            "}\n";
  auto inputMemBuf = MemoryBuffer::getMemBuffer(input);
  auto reader = FileListReader::get(std::move(inputMemBuf));
  ASSERT_TRUE(!!reader);

  class Visitor : public FileListReader::Visitor {
  public:
    std::map<std::string, HeaderType> headers;

    void visitHeaderFile(FileListReader::HeaderInfo &header) override {
      headers[header.path] = header.type;
    }
  };

  Visitor visitor;
  (*reader)->visit(visitor);

  EXPECT_EQ(2U, visitor.headers.size());
  EXPECT_EQ(HeaderType::Public, visitor.headers["/usr/include/foo.h"]);
  EXPECT_EQ(HeaderType::Project, visitor.headers["bar.h"]);
}
