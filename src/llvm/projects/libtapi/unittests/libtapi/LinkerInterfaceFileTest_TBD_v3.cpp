//===-- LinkerInterfaceFileTest_TBD_v2.cpp - Linker Interface File Test ---===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "tapi/Core/ArchitectureConfig.h"
#include "gtest/gtest.h"
#include <mach/machine.h>
#include <tapi/tapi.h>

using namespace tapi;

#define DEBUG_TYPE "libtapi-test"

static const char tbd_v3_file[] =
    "--- !tapi-tbd-v3\n"
    "archs: [ arm64 ]\n"
    "platform: ios\n"
    "install-name: /System/Library/Frameworks/Umbrella.framework/Umbrella\n"
    "exports:\n"
    "  - archs: [ arm64 ]\n"
    "    re-exports: [ /System/Library/PrivateFrameworks/Sub1.framework/Sub1, "
    "\n"
    "                  /System/Library/PrivateFrameworks/Sub2.framework/Sub2 "
    "]\n"
    "--- !tapi-tbd-v3\n"
    "archs: [ arm64 ]\n"
    "platform: ios\n"
    "install-name: /System/Library/PrivateFrameworks/Sub1.framework/Sub1\n"
    "exports:\n"
    "  - archs: [ arm64 ]\n"
    "    symbols: [ _sym1 ]\n"
    "--- !tapi-tbd-v3\n"
    "archs: [ arm64 ]\n"
    "platform: ios\n"
    "install-name: /System/Library/PrivateFrameworks/Sub2.framework/Sub2\n"
    "exports:\n"
    "  - archs: [ arm64 ]\n"
    "    symbols: [ _sym2 ]\n"
    "...\n";

using ExportedSymbol = std::tuple<std::string, bool, bool>;
using ExportedSymbolSeq = std::vector<ExportedSymbol>;

inline bool operator<(const ExportedSymbol &lhs, const ExportedSymbol &rhs) {
  return std::get<0>(lhs) < std::get<0>(rhs);
}

namespace TBDv3 {

TEST(libtapiTBDv3, LIF_isSupported) {
  llvm::StringRef buffer(tbd_v3_file);
  bool isSupported1 = LinkerInterfaceFile::isSupported(
      "Test.tbd", reinterpret_cast<const uint8_t *>(buffer.data()),
      buffer.size());
  EXPECT_TRUE(isSupported1);
}

// Test parsing a .tbd file from a memory buffer/nmapped file
TEST(libtapiTBDv3, LIF_Load_ARM64) {
  static const std::vector<std::string> expectedInlinedFrameworkNames = {
      "/System/Library/PrivateFrameworks/Sub1.framework/Sub1",
      "/System/Library/PrivateFrameworks/Sub2.framework/Sub2"};

  llvm::StringRef buffer(tbd_v3_file);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      "/System/Library/Frameworks/Umbrella.framework/Umbrella.tbd",
      reinterpret_cast<const uint8_t *>(buffer.data()), buffer.size(),
      CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);
  EXPECT_EQ(FileType::TBD_V3, file->getFileType());
  EXPECT_EQ(Platform::iOS, file->getPlatform());
  EXPECT_EQ(
      std::string("/System/Library/Frameworks/Umbrella.framework/Umbrella"),
      file->getInstallName());
  EXPECT_TRUE(file->isApplicationExtensionSafe());
  EXPECT_TRUE(file->hasTwoLevelNamespace());
  EXPECT_TRUE(file->hasReexportedLibraries());
  EXPECT_TRUE(file->exports().empty());
  EXPECT_EQ(2U, file->inlinedFrameworkNames().size());
  std::vector<std::string> inlinedFrameworkNames;
  for (auto &name : file->inlinedFrameworkNames())
    inlinedFrameworkNames.emplace_back(name);
  std::sort(inlinedFrameworkNames.begin(), inlinedFrameworkNames.end());
  ASSERT_EQ(expectedInlinedFrameworkNames.size(), inlinedFrameworkNames.size());
  EXPECT_TRUE(std::equal(inlinedFrameworkNames.begin(),
                         inlinedFrameworkNames.end(),
                         expectedInlinedFrameworkNames.begin()));

  for (auto &name : file->inlinedFrameworkNames()) {
    auto framework =
        std::unique_ptr<LinkerInterfaceFile>(file->getInlinedFramework(
            name, CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL, ParsingFlags::None,
            PackedVersion32(9, 0, 0), errorMessage));
    ASSERT_TRUE(errorMessage.empty());
    ASSERT_NE(nullptr, framework);
    EXPECT_EQ(FileType::TBD_V3, framework->getFileType());
    EXPECT_EQ(Platform::iOS, framework->getPlatform());
    EXPECT_TRUE(framework->isApplicationExtensionSafe());
    EXPECT_TRUE(framework->hasTwoLevelNamespace());
    EXPECT_FALSE(framework->hasReexportedLibraries());
    EXPECT_EQ(1U, framework->exports().size());
    EXPECT_TRUE(framework->inlinedFrameworkNames().empty());
  }
}

} // end namespace TBDv3
