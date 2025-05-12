//===-- LinkerInterfaceFileTest_TBD_v4.cpp - Linker Interface File Test ---===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "LibTapiTest.h"
#include "tapi/Core/ArchitectureConfig.h"
#include "gtest/gtest.h"
#include <mach-o/loader.h>
#include <mach/machine.h>
#include <tapi/tapi.h>

using namespace tapi;

#define DEBUG_TYPE "libtapi-test"

// Guard against old SDKs in CI missing these macro defs.
#ifndef PLATFORM_MACCATALYST
#define PLATFORM_MACCATALYST 6
#endif

#ifndef PLATFORM_DRIVERKIT
#define PLATFORM_DRIVERKIT 10
#endif


namespace {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

class LibTapiTest_TBDv4 : public LibTapiTest {};

static const char tbd_v4_file[] =
    "--- !tapi-tbd\n"
    "tbd-version: 4\n"
    "targets:  [ i386-macos, x86_64-macos, x86_64-maccatalyst ]\n"
    "uuids:\n"
    "  - target: i386-macos\n"
    "    value:  00000000-0000-0000-0000-000000000000\n"
    "  - target: x86_64-macos\n"
    "    value:  11111111-1111-1111-1111-111111111111\n"
    "  - target: x86_64-maccatalyst\n"
    "    value: 11111111-1111-1111-1111-111111111111\n"
    "flags: [ flat_namespace, installapi ]\n"
    "install-name: /System/Library/Frameworks/Umbrella.framework/Umbrella\n"
    "current-version: 1.2.3\n"
    "compatibility-version: 1.2\n"
    "swift-abi-version: 5\n"
    "parent-umbrella:\n"
    "  - targets: [ i386-macos, x86_64-macos, x86_64-maccatalyst ]\n"
    "    umbrella: System\n"
    "allowable-clients:\n"
    "  - targets: [ i386-macos, x86_64-macos, x86_64-maccatalyst ]\n"
    "    clients: [ ClientA, ClientB ]\n"
    "reexported-libraries:\n"
    "  - targets: [ i386-macos ]\n"
    "    libraries: [ /System/Library/Frameworks/A.framework/A ]\n"
    "  - targets: [ x86_64-macos, x86_64-maccatalyst ]\n"
    "    libraries: [ /System/Library/Frameworks/B.framework/B,\n"
    "                 /System/Library/Frameworks/C.framework/C ]\n"
    "exports:\n"
    "  - targets: [ i386-macos ]\n"
    "    symbols: [ _symA ]\n"
    "    objc-classes: []\n"
    "    objc-eh-types: []\n"
    "    objc-ivars: []\n"
    "    weak-symbols: [_weak5]\n"
    "    thread-local-symbols: []\n"
    "  - targets: [ x86_64-maccatalyst]\n"
    "    symbols: [_symB]\n"
    "    weak-symbols: [_weak0]\n"
    "  - targets: [ x86_64-macos, x86_64-maccatalyst ]\n"
    "    symbols: [_symAB]\n"
    "    weak-symbols: [_weak1, _weak2]\n"
    "reexports:\n"
    "  - targets: [i386-macos]\n"
    "    symbols: [_symC]\n"
    "    objc-classes: []\n"
    "    objc-eh-types: []\n"
    "    objc-ivars: []\n"
    "    weak-symbols: []\n"
    "    thread-local-symbols: []\n"
    "undefineds:\n"
    "  - targets: [ i386-macos ]\n"
    "    symbols: [ _symD ]\n"
    "    objc-classes: []\n"
    "    objc-eh-types: []\n"
    "    objc-ivars: []\n"
    "    weak-symbols: [ _weak6 ]\n"
    "    thread-local-symbols: []\n"
    "  - targets: [ x86_64-maccatalyst ]\n"
    "    symbols: []\n"
    "    objc-classes: []\n"
    "    objc-eh-types: []\n"
    "    objc-ivars: []\n"
    "    weak-symbols: [_weak3, _weak4]\n"
    "    thread-local-symbols: []\n"
    "...\n";

// fields: name, isWeakDefined, isThreadLocal, isWeakReferenced
using ExportedSymbol = std::tuple<std::string, bool, bool, bool>;
using ExportedSymbolSeq = std::vector<ExportedSymbol>;

static auto checkSyms = [](auto &base, auto &testSyms) {
  ExportedSymbolSeq test;
  for (const auto &sym : testSyms)
    test.push_back({sym.getName(), sym.isWeakDefined(),
                    sym.isThreadLocalValue(), sym.isWeakReferenced()});
  std::sort(test.begin(), test.end());

  ASSERT_EQ(base.size(), test.size());
  EXPECT_TRUE(std::equal(test.begin(), test.end(), base.begin()));
};

TEST_F(LibTapiTest_TBDv4, LIF_isSupported) {
  llvm::StringRef buffer(tbd_v4_file);
  bool isSupported = LinkerInterfaceFile::isSupported(
      "Test.tbd", reinterpret_cast<const uint8_t *>(buffer.data()),
      buffer.size());
  EXPECT_TRUE(isSupported);
}

// Test parsing a .tbd file from a memory buffer/nmapped file
TEST_F(LibTapiTest_TBDv4, LIF_Load) {
  writeTempFile(tbd_v4_file);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::None, PackedVersion32(10, 15, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);
  EXPECT_EQ((std::vector<uint32_t>{PLATFORM_MACOS, PLATFORM_MACCATALYST}),
            file->getPlatformSet());
  EXPECT_EQ(
      std::string("/System/Library/Frameworks/Umbrella.framework/Umbrella"),
      file->getInstallName());
  EXPECT_TRUE(file->isApplicationExtensionSafe());
  EXPECT_FALSE(file->hasTwoLevelNamespace());
  EXPECT_TRUE(file->hasReexportedLibraries());
}

TEST_F(LibTapiTest_TBDv4, LIF_LoadSymbols_x86_64) {
  writeTempFile(tbd_v4_file);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::None, PackedVersion32(10, 15, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);

  ExportedSymbolSeq tbd_v4_exports = {
      {"_symAB", false, false, false}, {"_symB", false, false, false},
      {"_weak0", true, false, false},  {"_weak1", true, false, false},
      {"_weak2", true, false, false},
  };

  ExportedSymbolSeq tbd_v4_undefs = {
      {"_weak3", false, false, true},
      {"_weak4", false, false, true},
  };

  checkSyms(tbd_v4_exports, file->exports());
  checkSyms(tbd_v4_undefs, file->undefineds());
}

TEST_F(LibTapiTest_TBDv4, LIF_LoadSymbols_i386) {
  writeTempFile(tbd_v4_file);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_I386, CPU_SUBTYPE_I386_ALL,
      ParsingFlags::None, PackedVersion32(10, 15, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);

  ExportedSymbolSeq tbd_v4_exports = {
      {"_symA", false, false, false},
      {"_symC", false, false, false},
      {"_weak5", true, false, false},
  };

  ExportedSymbolSeq tbd_v4_undefs = {
      {"_symD", false, false, false},
      {"_weak6", false, false, true},
  };

  checkSyms(tbd_v4_exports, file->exports());
  checkSyms(tbd_v4_undefs, file->undefineds());
}

TEST_F(LibTapiTest_TBDv4, LIF_Platform_macOS) {
  testPlatform("x86_64-macos", PLATFORM_MACOS, CPU_TYPE_X86_64,
               CPU_SUBTYPE_X86_ALL);
}

TEST_F(LibTapiTest_TBDv4, LIF_Platform_driverkit) {
  testPlatform("x86_64-driverkit", PLATFORM_DRIVERKIT, CPU_TYPE_X86_64,
               CPU_SUBTYPE_X86_ALL);
}

TEST_F(LibTapiTest_TBDv4, LIF_Platform_iOS) {
  testPlatform("arm64-ios", PLATFORM_IOS, CPU_TYPE_ARM64,
               CPU_SUBTYPE_ARM64_ALL);
}

TEST_F(LibTapiTest_TBDv4, LIF_Platform_watchOS) {
  testPlatform("armv7k-watchos", PLATFORM_WATCHOS, CPU_TYPE_ARM,
               CPU_SUBTYPE_ARM_V7K);
}

TEST_F(LibTapiTest_TBDv4, LIF_Platform_tvOS) {
  testPlatform("arm64-tvos", PLATFORM_TVOS, CPU_TYPE_ARM64,
               CPU_SUBTYPE_ARM64_ALL);
}

TEST_F(LibTapiTest_TBDv4, LIF_Platform_bridgeOS) {
  testPlatform("arm64-bridgeos", PLATFORM_BRIDGEOS, CPU_TYPE_ARM64,
               CPU_SUBTYPE_ARM64_ALL);
}



TEST_F(LibTapiTest_TBDv4, LIF_Load_iosmac1) {
  static const char tbd_v4_iosmac[] =
      "--- !tapi-tbd\n"
      "tbd-version: 4\n"
      "targets: [ x86_64-maccatalyst ]\n"
      "install-name: /System/Library/Frameworks/Foo.framework/Foo\n"
      "...\n";
  writeTempFile(tbd_v4_iosmac);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_ALL,
      ParsingFlags::None, PackedVersion32(10, 14, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);
  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_MACCATALYST},
            file->getPlatformSet());
  EXPECT_TRUE(file->exports().empty());
}

TEST_F(LibTapiTest_TBDv4, LIF_Load_iosmac2) {
  static const char tbd_v4_iosmac[] =
      "--- !tapi-tbd\n"
      "tbd-version: 4\n"
      "targets: [ x86_64-maccatalyst ]\n"
      "install-name: /System/Library/Frameworks/Foo.framework/Foo\n"
      "...\n";
  writeTempFile(tbd_v4_iosmac);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_ALL,
      ParsingFlags::None, PackedVersion32(10, 14, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);
  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_MACCATALYST},
            file->getPlatformSet());
  EXPECT_TRUE(file->exports().empty());
}

TEST_F(LibTapiTest_TBDv4, LIF_Load_iosmac_preRename) {
  static const char tbd_v4_iosmac[] =
      "--- !tapi-tbd\n"
      "tbd-version: 4\n"
      "targets: [ x86_64-<6> ]\n"
      "install-name: /System/Library/Frameworks/Foo.framework/Foo\n"
      "...\n";
  writeTempFile(tbd_v4_iosmac);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_ALL,
      ParsingFlags::None, PackedVersion32(10, 14, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);
  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_MACCATALYST},
            file->getPlatformSet());
  EXPECT_TRUE(file->exports().empty());
}

TEST_F(LibTapiTest_TBDv4, LIF_Load_iosmac3) {
  static const char tbd_v4_iosmac[] =
      "--- !tapi-tbd\n"
      "tbd-version: 4\n"
      "targets: [ x86_64-maccatalyst ]\n"
      "install-name: /System/Library/Frameworks/Foo.framework/Foo\n"
      "...\n";
  writeTempFile(tbd_v4_iosmac);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_ALL,
      ParsingFlags::None, PackedVersion32(10, 14, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);
  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_MACCATALYST},
            file->getPlatformSet());
  EXPECT_TRUE(file->exports().empty());
}

TEST_F(LibTapiTest_TBDv4, LIF_Load_zippered) {
  static const char tbd_v4_zippered[] =
      "--- !tapi-tbd\n"
      "tbd-version: 4\n"
      "targets: [ i386-macos, x86_64-macos, x86_64-maccatalyst ]\n"
      "install-name: /System/Library/Frameworks/Foo.framework/Foo\n"
      "exports:\n"
      "  - targets: [ i386-macos, x86_64-macos, x86_64-maccatalyst ]\n"
      "    objc-classes: [ Foo, Bar ]\n"
      "...\n";

  static std::vector<std::string> tbd_v4_i386_symbols = {
      ".objc_class_name_Bar",
      ".objc_class_name_Foo",
  };

  static std::vector<std::string> tbd_v4_x86_64_symbols = {
      "_OBJC_CLASS_$_Bar",
      "_OBJC_CLASS_$_Foo",
      "_OBJC_METACLASS_$_Bar",
      "_OBJC_METACLASS_$_Foo",
  };

  std::vector<std::string> exports;
  writeTempFile(tbd_v4_zippered);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_I386, CPU_SUBTYPE_I386_ALL,
      ParsingFlags::None, PackedVersion32(10, 14, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);
  EXPECT_EQ((std::vector<uint32_t>{PLATFORM_MACOS}), file->getPlatformSet());

  exports.clear();
  for (const auto &sym : file->exports())
    exports.emplace_back(sym.getName());
  std::sort(exports.begin(), exports.end());

  ASSERT_EQ(tbd_v4_i386_symbols.size(), exports.size());
  EXPECT_TRUE(
      std::equal(exports.begin(), exports.end(), tbd_v4_i386_symbols.begin()));

  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_ALL,
      ParsingFlags::None, PackedVersion32(10, 14, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);
  EXPECT_EQ((std::vector<uint32_t>{PLATFORM_MACOS, PLATFORM_MACCATALYST}),
            file->getPlatformSet());

  exports.clear();
  for (const auto &sym : file->exports())
    exports.emplace_back(sym.getName());
  std::sort(exports.begin(), exports.end());

  ASSERT_EQ(tbd_v4_x86_64_symbols.size(), exports.size());
  EXPECT_TRUE(std::equal(exports.begin(), exports.end(),
                         tbd_v4_x86_64_symbols.begin()));
}

// Test invalid files.
TEST_F(LibTapiTest_TBDv4, LIF_Load_Unknown_Target) {
  const char input[] =
      "--- !tapi-tbd\n"
      "tbd-version: 4\n"
      "targets:  [ i386-debian ]\n"
      "install-name: /System/Library/Frameworks/Umbrella.framework/Umbrella\n"
      "...\n";
  writeTempFile(input);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_I386, CPU_SUBTYPE_I386_ALL,
      ParsingFlags::None, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  ASSERT_EQ(std::string("malformed file\n") + getTempFilePath() +
                ":3:13: error: unknown platform\n"
                "targets:  [ i386-debian ]\n"
                "            ^~~~~~~~~~~~\n",
            errorMessage);
}

TEST_F(LibTapiTest_TBDv4, LIF_MalformedFile) {
  const char input[] =
      "--- !tapi-tbd\n"
      "tbd-version: 4\n"
      "targets:  [ armv7-ios ]\n"
      "install-name: /System/Library/Frameworks/Umbrella.framework/Umbrella\n"
      "foo: bar\n"
      "...\n";
  writeTempFile(input);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7,
      ParsingFlags::ExactCpuSubType, PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  ASSERT_EQ(std::string("malformed file\n") + getTempFilePath() +
                ":5:1: error: unknown key 'foo'\nfoo: bar\n^~~\n",
            errorMessage);
}

TEST_F(LibTapiTest_TBDv4, LIF_LegacyLinkerSymbols) {
  const char input[] =
    "--- !tapi-tbd\n"
    "tbd-version: 4\n"
    "targets:  [ x86_64-macos ]\n"
    "uuids:\n"
    "  - target: x86_64-macos\n"
    "    value:  11111111-1111-1111-1111-111111111111\n"
    "install-name: /usr/lib/libfoo.dylib\n"
    "current-version: 1.2.3\n"
    "compatibility-version: 1.2\n"
    "swift-abi-version: 5\n"
    "exports:\n"
    "  - targets: [ x86_64-macos ]\n"
    "    symbols: [ $ld$add$os10.15$_symA,\n"
    "               $ld$add$os10.14$_symB,\n"
    "               $ld$hide$os10.14$_symC, _symC,\n"
    "               $ld$install_name$os10.14$/usr/lib/libfoo.1.dylib,\n"
    "               $ld$unsupport_action$some_os$some_sym ]\n"
    "...\n";
  writeTempFile(input);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::None, PackedVersion32(10, 14, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);

  ExportedSymbolSeq tbd_v4_exports = {
      {"_symB", false, false, false},
  };

  checkSyms(tbd_v4_exports, file->exports());
  ASSERT_TRUE(file->isInstallNameVersionSpecific());
  ASSERT_EQ(file->getInstallName(), "/usr/lib/libfoo.1.dylib");
}

TEST_F(LibTapiTest_TBDv4, LIF_NewLinkerSymbols) {
  const char input[] =
    "--- !tapi-tbd\n"
    "tbd-version: 4\n"
    "targets:  [ x86_64-macos ]\n"
    "uuids:\n"
    "  - target: x86_64-macos\n"
    "    value:  11111111-1111-1111-1111-111111111111\n"
    "install-name: /usr/lib/libfoo.dylib\n"
    "current-version: 1.2.3\n"
    "compatibility-version: 1.2\n"
    "swift-abi-version: 5\n"
    "exports:\n"
    "  - targets: [ x86_64-macos ]\n"
    "    symbols: [ $ld$previous$/usr/lib/libbar.dylib$$1$10.9$10.16$_foo$ ]\n"
    "...\n";
  writeTempFile(input);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::None, PackedVersion32(10, 14, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);

  ExportedSymbolSeq tbd_v4_exports = {
      {"$ld$previous$/usr/lib/libbar.dylib$$1$10.9$10.16$_foo$", false, false,
       false},
  };

  checkSyms(tbd_v4_exports, file->exports());
  ASSERT_FALSE(file->isInstallNameVersionSpecific());
}

#pragma clang diagnostic pop
} // end namespace
