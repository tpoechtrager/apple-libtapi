//===-- LinkerInterfaceFileTest_TBD_v2.cpp - Linker Interface File Test ---===//
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

namespace {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

class LibTapiTest_TBDv2 : public LibTapiTest {};

static const char tbd_v2_file[] =
    "--- !tapi-tbd-v2\n"
    "archs: [ armv7, armv7s, armv7k, arm64 ]\n"
    "platform: ios\n"
    "install-name: Test.dylib\n"
    "current-version: 2.3.4\n"
    "compatibility-version: 1.0\n"
    "swift-version: 1.1\n"
    "parent-umbrella: Umbrella.dylib\n"
    "exports:\n"
    "  - archs: [ armv7, armv7s, armv7k, arm64 ]\n"
    "    allowable-clients: [ Foo.dylib ]\n"
    "    symbols: [ _sym1, _sym2, _sym3, _sym4, $ld$hide$os9.0$_sym1 ]\n"
    "    objc-classes: [ _class1, _class2 ]\n"
    "    objc-ivars: [ _class1._ivar1, _class1._ivar2 ]\n"
    "    weak-def-symbols: [ _weak1, _weak2 ]\n"
    "    thread-local-symbols: [ _tlv1, _tlv2 ]\n"
    "  - archs: [ armv7, armv7s, armv7k ]\n"
    "    allowable-clients: [ Bar.dylib ]\n"
    "    symbols: [ _sym5 ]\n"
    "    objc-classes: [ _class3 ]\n"
    "    objc-ivars: [ _class1._ivar3 ]\n"
    "    weak-def-symbols: [ _weak3 ]\n"
    "    thread-local-symbols: [ _tlv3 ]\n"
    "...\n";

static const char tbd_v2_file2[] =
    "--- !tapi-tbd-v2\n"
    "archs: [ armv7, armv7s, armv7k, arm64 ]\n"
    "platform: ios\n"
    "flags: [ flat_namespace ]\n"
    "install-name: Test.dylib\n"
    "current-version: 2.3.4\n"
    "compatibility-version: 1.0\n"
    "swift-version: 1.1\n"
    "exports:\n"
    "  - archs: [ armv7, armv7s, armv7k, arm64 ]\n"
    "    symbols: [ _sym1, _sym2, _sym3, _sym4, $ld$hide$os9.0$_sym1 ]\n"
    "    objc-classes: [ _class1, _class2 ]\n"
    "    objc-ivars: [ _class1._ivar1, _class1._ivar2 ]\n"
    "    weak-def-symbols: [ _weak1, _weak2 ]\n"
    "    thread-local-symbols: [ _tlv1, _tlv2 ]\n"
    "  - archs: [ armv7, armv7s, armv7k ]\n"
    "    symbols: [ _sym5 ]\n"
    "    objc-classes: [ _class3 ]\n"
    "    objc-ivars: [ _class1._ivar3 ]\n"
    "    weak-def-symbols: [ _weak3 ]\n"
    "    thread-local-symbols: [ _tlv3 ]\n"
    "undefineds:\n"
    "  - archs: [ armv7, armv7s, armv7k, arm64 ]\n"
    "    symbols: [ _undefSym1, _undefSym2, _undefSym3 ]\n"
    "    objc-classes: [ _undefClass1, _undefClass2 ]\n"
    "    objc-ivars: [ _undefClass1._ivar1, _undefClass1._ivar2 ]\n"
    "    weak-ref-symbols: [ _undefWeak1, _undefWeak2 ]\n"
    "...\n";

static const char tbd_v2_file_unknown_platform[] = "--- !tapi-tbd-v2\n"
                                                   "archs: [ i386 ]\n"
                                                   "platform: unknown\n"
                                                   "install-name: Test.dylib\n"
                                                   "...\n";

static const unsigned char unsupported_file[] = {0xcf, 0xfa, 0xed, 0xfe, 0x07,
                                                 0x00, 0x00, 0x00, 0x00, 0x00};

static const char malformed_file[] = "--- !tapi-tbd-v2\n"
                                     "archs: [ arm64 ]\n"
                                     "foobar: \"Unsupported key\"\n"
                                     "...\n";

static const char malformed_file2[] = "--- !tapi-tbd-v2\n"
                                      "archs: [ arm64 ]\n"
                                      "platform: ios\n"
                                      "install-name: Test.dylib\n"
                                      "foobar: \"Unsupported key\"\n"
                                      "...\n";

static const char malformed_file3[] = "--- !tapi-tbd-v2\n"
                                      "archs: [ arm64 ]\n"
                                      "platform: ios\n"
                                      "flags: [ two_level_namespace ]\n"
                                      "...\n";

using ExportedSymbol = std::tuple<std::string, bool, bool>;
using ExportedSymbolSeq = std::vector<ExportedSymbol>;

static ExportedSymbolSeq tbd_v2_arm_exports = {
    {"_OBJC_CLASS_$_class1", false, false},
    {"_OBJC_CLASS_$_class2", false, false},
    {"_OBJC_CLASS_$_class3", false, false},
    {"_OBJC_IVAR_$_class1._ivar1", false, false},
    {"_OBJC_IVAR_$_class1._ivar2", false, false},
    {"_OBJC_IVAR_$_class1._ivar3", false, false},
    {"_OBJC_METACLASS_$_class1", false, false},
    {"_OBJC_METACLASS_$_class2", false, false},
    {"_OBJC_METACLASS_$_class3", false, false},
    {"_sym2", false, false},
    {"_sym3", false, false},
    {"_sym4", false, false},
    {"_sym5", false, false},
    {"_tlv1", false, true},
    {"_tlv2", false, true},
    {"_tlv3", false, true},
    {"_weak1", true, false},
    {"_weak2", true, false},
    {"_weak3", true, false},
};

using UndefinedSymbol = std::tuple<std::string, bool>;
using UndefinedSymbolSeq = std::vector<UndefinedSymbol>;

static UndefinedSymbolSeq tbd_v2_arm_undefineds = {
    {"_OBJC_CLASS_$_undefClass1", false},
    {"_OBJC_CLASS_$_undefClass2", false},
    {"_OBJC_IVAR_$_undefClass1._ivar1", false},
    {"_OBJC_IVAR_$_undefClass1._ivar2", false},
    {"_OBJC_METACLASS_$_undefClass1", false},
    {"_OBJC_METACLASS_$_undefClass2", false},
    {"_undefSym1", false},
    {"_undefSym2", false},
    {"_undefSym3", false},
    {"_undefWeak1", true},
    {"_undefWeak2", true},
};

static ExportedSymbolSeq tbd_v2_arm64_exports = {
    {"_OBJC_CLASS_$_class1", false, false},
    {"_OBJC_CLASS_$_class2", false, false},
    {"_OBJC_IVAR_$_class1._ivar1", false, false},
    {"_OBJC_IVAR_$_class1._ivar2", false, false},
    {"_OBJC_METACLASS_$_class1", false, false},
    {"_OBJC_METACLASS_$_class2", false, false},
    {"_sym2", false, false},
    {"_sym3", false, false},
    {"_sym4", false, false},
    {"_tlv1", false, true},
    {"_tlv2", false, true},
    {"_weak1", true, false},
    {"_weak2", true, false},
};

TEST_F(LibTapiTest_TBDv2, LIF_isSupported) {
  llvm::StringRef buffer(tbd_v2_file);
  EXPECT_TRUE(LinkerInterfaceFile::isSupported(
      getTempFilePath(), reinterpret_cast<const uint8_t *>(buffer.data()),
      buffer.size()));
  EXPECT_FALSE(LinkerInterfaceFile::isSupported(
      getTempFilePath(), reinterpret_cast<const uint8_t *>(unsupported_file),
      sizeof(unsupported_file)));
}

// These tests intentionally call deprecated apis.
TEST_F(LibTapiTest_TBDv2, LIF_shouldPreferTextBasedStubFile) {
  EXPECT_TRUE(LinkerInterfaceFile::shouldPreferTextBasedStubFile(
      INPUT_PATH "/installapi.tbd"));
  EXPECT_TRUE(LinkerInterfaceFile::shouldPreferTextBasedStubFile(
      INPUT_PATH "/install.tbd"));
}

TEST_F(LibTapiTest_TBDv2, LIF_isEquivalent) {
  EXPECT_FALSE(LinkerInterfaceFile::areEquivalent(INPUT_PATH "/libuuid1.tbd",
                                                  INPUT_PATH "/libuuid.dylib"));
}

// Test parsing a .tbd file.
TEST_F(LibTapiTest_TBDv2, LIF_Load_ARM) {
  writeTempFile(tbd_v2_file);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_IOS}, file->getPlatformSet());
  EXPECT_EQ(std::string("Test.dylib"), file->getInstallName());
  EXPECT_EQ(0x20304U, file->getCurrentVersion());
  EXPECT_EQ(0x10000U, file->getCompatibilityVersion());
  EXPECT_EQ(2U, file->getSwiftVersion());
  EXPECT_TRUE(file->isApplicationExtensionSafe());
  EXPECT_TRUE(file->hasTwoLevelNamespace());
  EXPECT_EQ(std::string("Umbrella.dylib"), file->getParentFrameworkName());

  std::vector<std::string> allowableClients = {"Bar.dylib", "Foo.dylib"};
  EXPECT_EQ(allowableClients, file->allowableClients());

  ExportedSymbolSeq exports;
  for (const auto &sym : file->exports())
    exports.emplace_back(sym.getName(), sym.isWeakDefined(),
                         sym.isThreadLocalValue());
  std::sort(exports.begin(), exports.end());

  ASSERT_EQ(tbd_v2_arm_exports.size(), exports.size());
  EXPECT_TRUE(
      std::equal(exports.begin(), exports.end(), tbd_v2_arm_exports.begin()));
}

TEST_F(LibTapiTest_TBDv2, LIF_Load_ARM64) {
  writeTempFile(tbd_v2_file);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::None, PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_IOS}, file->getPlatformSet());
  EXPECT_EQ(std::string("Test.dylib"), file->getInstallName());
  EXPECT_EQ(0x20304U, file->getCurrentVersion());
  EXPECT_EQ(0x10000U, file->getCompatibilityVersion());
  EXPECT_EQ(2U, file->getSwiftVersion());
  EXPECT_TRUE(file->isApplicationExtensionSafe());
  EXPECT_TRUE(file->hasTwoLevelNamespace());
  EXPECT_EQ(std::string("Umbrella.dylib"), file->getParentFrameworkName());

  std::vector<std::string> allowableClients = {"Foo.dylib"};
  EXPECT_EQ(allowableClients, file->allowableClients());

  ExportedSymbolSeq exports;
  for (const auto &sym : file->exports())
    exports.emplace_back(sym.getName(), sym.isWeakDefined(),
                         sym.isThreadLocalValue());
  std::sort(exports.begin(), exports.end());

  ASSERT_EQ(tbd_v2_arm64_exports.size(), exports.size());
  EXPECT_TRUE(
      std::equal(exports.begin(), exports.end(), tbd_v2_arm64_exports.begin()));
}

TEST_F(LibTapiTest_TBDv2, LIF_Load_flat) {
  writeTempFile(tbd_v2_file2);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  llvm::outs() << errorMessage;
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_IOS}, file->getPlatformSet());
  EXPECT_EQ(std::string("Test.dylib"), file->getInstallName());
  EXPECT_EQ(0x20304U, file->getCurrentVersion());
  EXPECT_EQ(0x10000U, file->getCompatibilityVersion());
  EXPECT_EQ(2U, file->getSwiftVersion());
  EXPECT_TRUE(file->isApplicationExtensionSafe());
  EXPECT_FALSE(file->hasTwoLevelNamespace());

  ExportedSymbolSeq exports;
  for (const auto &sym : file->exports())
    exports.emplace_back(sym.getName(), sym.isWeakDefined(),
                         sym.isThreadLocalValue());
  std::sort(exports.begin(), exports.end());

  UndefinedSymbolSeq undefineds;
  for (const auto &sym : file->undefineds())
    undefineds.emplace_back(sym.getName(), sym.isWeakReferenced());
  std::sort(undefineds.begin(), undefineds.end());

  ASSERT_EQ(tbd_v2_arm_exports.size(), exports.size());
  EXPECT_TRUE(
      std::equal(exports.begin(), exports.end(), tbd_v2_arm_exports.begin()));
  ASSERT_EQ(tbd_v2_arm_undefineds.size(), undefineds.size());
  EXPECT_TRUE(std::equal(undefineds.begin(), undefineds.end(),
                         tbd_v2_arm_undefineds.begin()));
}

TEST_F(LibTapiTest_TBDv2, LIF_Load_Install_Name) {
  static const char tbd_file[] =
      "--- !tapi-tbd-v2\n"
      "archs: [ i386, x86_64 ]\n"
      "platform: macosx\n"
      "install-name: "
      "/System/Library/Frameworks/CoreImage.framework/Versions/A/CoreImage\n"
      "current-version: 5.0\n"
      "compatibility-version: 1.0.1\n"
      "exports:\n"
      "  - archs: [ i386, x86_64 ]\n"
      "    symbols: [ "
      "'$ld$install_name$os10.10$/System/Library/Frameworks/"
      "QuartzCore.framework/"
      "Versions/A/QuartzCore',\n"
      "               "
      "'$ld$install_name$os10.4$/System/Library/Frameworks/"
      "QuartzCore.framework/"
      "Versions/A/QuartzCore',\n"
      "               "
      "'$ld$install_name$os10.5$/System/Library/Frameworks/"
      "QuartzCore.framework/"
      "Versions/A/QuartzCore',\n"
      "               "
      "'$ld$install_name$os10.6$/System/Library/Frameworks/"
      "QuartzCore.framework/"
      "Versions/A/QuartzCore',\n"
      "               "
      "'$ld$install_name$os10.7$/System/Library/Frameworks/"
      "QuartzCore.framework/"
      "Versions/A/QuartzCore',\n"
      "               "
      "'$ld$install_name$os10.8$/System/Library/Frameworks/"
      "QuartzCore.framework/"
      "Versions/A/QuartzCore',\n"
      "               "
      "'$ld$install_name$os10.9$/System/Library/Frameworks/"
      "QuartzCore.framework/"
      "Versions/A/QuartzCore' ]\n"
      "...\n";

  writeTempFile(tbd_file);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86, CPU_SUBTYPE_X86_ALL, ParsingFlags::None,
      PackedVersion32(10, 10, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_MACOS}, file->getPlatformSet());
  EXPECT_EQ(std::string("/System/Library/Frameworks/QuartzCore.framework/"
                        "Versions/A/QuartzCore"),
            file->getInstallName());
  EXPECT_EQ(0x50000U, file->getCurrentVersion());
  EXPECT_EQ(0x10001U, file->getCompatibilityVersion());
  EXPECT_TRUE(file->isApplicationExtensionSafe());
  EXPECT_TRUE(file->hasTwoLevelNamespace());
  EXPECT_TRUE(file->isInstallNameVersionSpecific());
}

// Test for invalid files.
TEST_F(LibTapiTest_TBDv2, LIF_Load_Unknown_Platform) {
  writeTempFile(tbd_v2_file_unknown_platform);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_I386, CPU_SUBTYPE_I386_ALL,
      ParsingFlags::None, PackedVersion32(10, 11, 0), errorMessage));
  EXPECT_EQ(nullptr, file);
  ASSERT_EQ(std::string("malformed file\n") + getTempFilePath() +
                ":3:11: error: unknown platform\n"
                "platform: unknown\n"
                "          ^~~~~~~\n",
            errorMessage);
}

TEST_F(LibTapiTest_TBDv2, LIF_UnsupportedFileType) {
  writeTempFile(unsupported_file);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  EXPECT_EQ(nullptr, file);
  EXPECT_EQ("unsupported file type", errorMessage);
}

TEST_F(LibTapiTest_TBDv2, LIF_MalformedFile) {
  writeTempFile(malformed_file);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::None, PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  ASSERT_EQ(std::string("malformed file\n") + getTempFilePath() +
                ":2:1: error: missing required key "
                "'platform'\narchs: [ arm64 ]\n^\n",
            errorMessage);
}

TEST_F(LibTapiTest_TBDv2, LIF_MalformedFile2) {
  writeTempFile(malformed_file2);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::None, PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  ASSERT_EQ(std::string("malformed file\n") + getTempFilePath() +
                ":5:1: error: unknown key 'foobar'\nfoobar: \"Unsupported "
                "key\"\n^~~~~~\n",
            errorMessage);
}

TEST_F(LibTapiTest_TBDv2, LIF_MalformedFile3) {
  writeTempFile(malformed_file3);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::None, PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  ASSERT_EQ(std::string("malformed file\n") + getTempFilePath() +
                ":4:10: error: unknown bit value\nflags: [ "
                "two_level_namespace ]\n         ^~~~~~~~~~~~~~~~~~~~\n",
            errorMessage);
}

TEST_F(LibTapiTest_TBDv2, LIF_ArchitectureNotFound) {
  writeTempFile(tbd_v2_file);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_ALL,
      ParsingFlags::None, PackedVersion32(10, 11, 0), errorMessage));
  EXPECT_EQ(nullptr, file);
  EXPECT_EQ(std::string("missing required architecture x86_64 in file ") +
                getTempFilePath() + " (4 slices)",
            errorMessage);
}

// Test parsing a .tbd file from a memory buffer/nmapped file (weak import).
TEST_F(LibTapiTest_TBDv2, LIF_Load_WeakImport) {
  static const char tbd_file[] =
      "--- !tapi-tbd-v2\n"
      "archs: [ x86_64 ]\n"
      "platform: macosx\n"
      "install-name: Test.dylib\n"
      "exports:\n"
      "  - archs: [ x86_64 ]\n"
      "    symbols: [ '$ld$weak$os10.11$_sym1', _sym1 ]\n"
      "...\n";
  writeTempFile(tbd_file);
  EXPECT_TRUE(Version::isAtLeast(1, 1));
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_ALL,
      ParsingFlags::None, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);
  EXPECT_EQ(1U, file->exports().size());
  EXPECT_EQ("_sym1", file->exports().front().getName());

  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_ALL,
      ParsingFlags::DisallowWeakImports, PackedVersion32(10, 11, 0),
      errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);
  EXPECT_EQ(0U, file->exports().size());
  EXPECT_EQ(1U, file->ignoreExports().size());
  EXPECT_EQ("_sym1", file->ignoreExports().front());
}

TEST_F(LibTapiTest_TBDv2, LIF_SelectPreferedSlice_armv7) {
  static const char prefer_armv7[] = "--- !tapi-tbd-v2\n"
                                     "archs: [ armv7, armv7s, armv7k, arm64 ]\n"
                                     "platform: ios\n"
                                     "install-name: Test.dylib\n"
                                     "exports:\n"
                                     "  - archs: [ armv7 ]\n"
                                     "    symbols: [ _correct ]\n"
                                     "  - archs: [ armv7s, armv7k, arm64 ]\n"
                                     "    symbols: [ _incorrect ]\n"
                                     "...\n";

  writeTempFile(prefer_armv7);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());
}

TEST_F(LibTapiTest_TBDv2, LIF_SelectPreferedSlice_armv7s) {
  static const char prefer_armv7s[] =
      "--- !tapi-tbd-v2\n"
      "archs: [ armv7, armv7s, armv7k, arm64 ]\n"
      "platform: ios\n"
      "install-name: Test.dylib\n"
      "exports:\n"
      "  - archs: [ armv7s ]\n"
      "    symbols: [ _correct ]\n"
      "  - archs: [ armv7, armv7k, arm64 ]\n"
      "    symbols: [ _incorrect ]\n"
      "...\n";

  writeTempFile(prefer_armv7s);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7S, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());
}

TEST_F(LibTapiTest_TBDv2, LIF_SelectPreferedSlice_armv7k) {
  static const char prefer_armv7k[] =
      "--- !tapi-tbd-v2\n"
      "archs: [ armv7, armv7s, armv7k, arm64 ]\n"
      "platform: ios\n"
      "install-name: Test.dylib\n"
      "exports:\n"
      "  - archs: [ armv7k ]\n"
      "    symbols: [ _correct ]\n"
      "  - archs: [ armv7, armv7s, arm64 ]\n"
      "    symbols: [ _incorrect ]\n"
      "...\n";

  writeTempFile(prefer_armv7k);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7K, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());
}

TEST_F(LibTapiTest_TBDv2, LIF_SelectPreferedSlice_arm64) {
  static const char prefer_arm64[] = "--- !tapi-tbd-v2\n"
                                     "archs: [ armv7, armv7s, armv7k, arm64 ]\n"
                                     "platform: ios\n"
                                     "install-name: Test.dylib\n"
                                     "exports:\n"
                                     "  - archs: [ arm64 ]\n"
                                     "    symbols: [ _correct ]\n"
                                     "  - archs: [ armv7, armv7s, armv7k ]\n"
                                     "    symbols: [ _incorrect ]\n"
                                     "...\n";

  writeTempFile(prefer_arm64);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::None, PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());
}

TEST_F(LibTapiTest_TBDv2, LIF_SelectPreferedSlice_i386) {
  static const char prefer_i386[] = "--- !tapi-tbd-v2\n"
                                    "archs: [ i386, x86_64, x86_64h ]\n"
                                    "platform: macosx\n"
                                    "install-name: Test.dylib\n"
                                    "exports:\n"
                                    "  - archs: [ i386 ]\n"
                                    "    symbols: [ _correct ]\n"
                                    "  - archs: [ x86_64, x86_64h ]\n"
                                    "    symbols: [ _incorrect ]\n"
                                    "...\n";

  writeTempFile(prefer_i386);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_I386, CPU_SUBTYPE_386, ParsingFlags::None,
      PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());
}

TEST_F(LibTapiTest_TBDv2, LIF_SelectPreferedSlice_x86_64) {
  static const char prefer_x86_64[] = "--- !tapi-tbd-v2\n"
                                      "archs: [ i386, x86_64, x86_64h ]\n"
                                      "platform: macosx\n"
                                      "install-name: Test.dylib\n"
                                      "exports:\n"
                                      "  - archs: [ x86_64 ]\n"
                                      "    symbols: [ _correct ]\n"
                                      "  - archs: [ i386, x86_64h ]\n"
                                      "    symbols: [ _incorrect ]\n"
                                      "...\n";

  writeTempFile(prefer_x86_64);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::None, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());
}

TEST_F(LibTapiTest_TBDv2, LIF_SelectPreferedSlice_x86_64h) {
  static const char prefer_x86_64h[] = "--- !tapi-tbd-v2\n"
                                       "archs: [ i386, x86_64, x86_64h ]\n"
                                       "platform: macosx\n"
                                       "install-name: Test.dylib\n"
                                       "exports:\n"
                                       "  - archs: [ x86_64h ]\n"
                                       "    symbols: [ _correct ]\n"
                                       "  - archs: [ i386, x86_64]\n"
                                       "    symbols: [ _incorrect ]\n"
                                       "...\n";

  writeTempFile(prefer_x86_64h);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_H,
      ParsingFlags::None, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());
}

TEST_F(LibTapiTest_TBDv2, LIF_FallBack_armv7) {
  static const char fallback_armv7[] = "--- !tapi-tbd-v2\n"
                                       "archs: [ armv7 ]\n"
                                       "platform: ios\n"
                                       "install-name: Test.dylib\n"
                                       "exports:\n"
                                       "  - archs: [ armv7 ]\n"
                                       "    symbols: [ _correct ]\n"
                                       "...\n";

  writeTempFile(fallback_armv7);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());

  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7S, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());

  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7K, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  // BEGIN WORKAROUND FOR rdar://problem/25535679
  // ASSERT_EQ(nullptr, file);
  // ASSERT_EQ("missing required architecture armv7k in file Test.tbd",
  //           errorMessage);
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());
  // END WORKAROUND

  errorMessage.clear();
  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::None, PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  EXPECT_EQ(std::string("missing required architecture arm64 in file ") +
                getTempFilePath(),
            errorMessage);
}

TEST_F(LibTapiTest_TBDv2, LIF_FallBack_armv7s) {
  static const char fallback_armv7s[] = "--- !tapi-tbd-v2\n"
                                        "archs: [ armv7s ]\n"
                                        "platform: ios\n"
                                        "install-name: Test.dylib\n"
                                        "exports:\n"
                                        "  - archs: [ armv7s ]\n"
                                        "    symbols: [ _correct ]\n"
                                        "...\n";

  writeTempFile(fallback_armv7s);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());

  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7S, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());

  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7K, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  // BEGIN WORKAROUND FOR rdar://problem/25535679
  // ASSERT_EQ(nullptr, file);
  // ASSERT_EQ("missing required architecture armv7k in file Test.tbd",
  //           errorMessage);
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());
  // END WORKAROUND

  errorMessage.clear();
  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::None, PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  EXPECT_EQ(std::string("missing required architecture arm64 in file ") +
                getTempFilePath(),
            errorMessage);
}

TEST_F(LibTapiTest_TBDv2, LIF_FallBack_armv7k) {
  static const char fallback_armv7k[] = "--- !tapi-tbd-v2\n"
                                        "archs: [ armv7k ]\n"
                                        "platform: ios\n"
                                        "install-name: Test.dylib\n"
                                        "exports:\n"
                                        "  - archs: [ armv7k ]\n"
                                        "    symbols: [ _correct ]\n"
                                        "...\n";

  writeTempFile(fallback_armv7k);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  // BEGIN WORKAROUND FOR rdar://problem/25535679
  // ASSERT_EQ(nullptr, file);
  // ASSERT_EQ("missing required architecture armv7 in file Test.tbd",
  //           errorMessage);
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());
  // END WORKAROUND

  errorMessage.clear();
  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7S, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  // BEGIN WORKAROUND FOR rdar://problem/25535679
  // ASSERT_EQ(nullptr, file);
  // ASSERT_EQ("missing required architecture armv7s in file Test.tbd",
  //           errorMessage);
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());
  // END WORKAROUND

  errorMessage.clear();
  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7K, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());

  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::None, PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  EXPECT_EQ(std::string("missing required architecture arm64 in file ") +
                getTempFilePath(),
            errorMessage);
}

TEST_F(LibTapiTest_TBDv2, LIF_FallBack_arm64) {
  static const char fallback_arm64[] = "--- !tapi-tbd-v2\n"
                                       "archs: [ arm64 ]\n"
                                       "platform: ios\n"
                                       "install-name: Test.dylib\n"
                                       "exports:\n"
                                       "  - archs: [ arm64 ]\n"
                                       "    symbols: [ _correct ]\n"
                                       "...\n";

  writeTempFile(fallback_arm64);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  EXPECT_EQ(std::string("missing required architecture armv7 in file ") +
                getTempFilePath(),
            errorMessage);

  errorMessage.clear();
  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7S, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  EXPECT_EQ(std::string("missing required architecture armv7s in file ") +
                getTempFilePath(),
            errorMessage);

  errorMessage.clear();
  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7K, ParsingFlags::None,
      PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  EXPECT_EQ(std::string("missing required architecture armv7k in file ") +
                getTempFilePath(),
            errorMessage);

  errorMessage.clear();
  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::None, PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());
}

TEST_F(LibTapiTest_TBDv2, LIF_FallBack_i386) {
  static const char fallback_i386[] = "--- !tapi-tbd-v2\n"
                                      "archs: [ i386 ]\n"
                                      "platform: macosx\n"
                                      "install-name: Test.dylib\n"
                                      "exports:\n"
                                      "  - archs: [ i386 ]\n"
                                      "    symbols: [ _correct ]\n"
                                      "...\n";

  writeTempFile(fallback_i386);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_I386, CPU_SUBTYPE_386, ParsingFlags::None,
      PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());

  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::None, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  EXPECT_EQ(std::string("missing required architecture x86_64 in file ") +
                getTempFilePath(),
            errorMessage);

  errorMessage.clear();
  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_H,
      ParsingFlags::None, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  EXPECT_EQ(std::string("missing required architecture x86_64h in file ") +
                getTempFilePath(),
            errorMessage);
}

TEST_F(LibTapiTest_TBDv2, LIF_FallBack_x86_64) {
  static const char fallback_x86_64[] = "--- !tapi-tbd-v2\n"
                                        "archs: [ x86_64 ]\n"
                                        "platform: macosx\n"
                                        "install-name: Test.dylib\n"
                                        "exports:\n"
                                        "  - archs: [ x86_64 ]\n"
                                        "    symbols: [ _correct ]\n"
                                        "...\n";

  writeTempFile(fallback_x86_64);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_I386, CPU_SUBTYPE_386, ParsingFlags::None,
      PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  EXPECT_EQ(std::string("missing required architecture i386 in file ") +
                getTempFilePath(),
            errorMessage);

  errorMessage.clear();
  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::None, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());

  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_H,
      ParsingFlags::None, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());
}

TEST_F(LibTapiTest_TBDv2, LIF_FallBack_x86_64h) {
  static const char fallback_x86_64h[] = "--- !tapi-tbd-v2\n"
                                         "archs: [ x86_64h ]\n"
                                         "platform: macosx\n"
                                         "install-name: Test.dylib\n"
                                         "exports:\n"
                                         "  - archs: [ x86_64h ]\n"
                                         "    symbols: [ _correct ]\n"
                                         "...\n";

  writeTempFile(fallback_x86_64h);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_I386, CPU_SUBTYPE_386, ParsingFlags::None,
      PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  EXPECT_EQ(std::string("missing required architecture i386 in file ") +
                getTempFilePath(),
            errorMessage);

  errorMessage.clear();
  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::None, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());

  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_H,
      ParsingFlags::None, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);
  ASSERT_TRUE(errorMessage.empty());
  EXPECT_EQ("_correct", file->exports().front().getName());

  file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::ExactCpuSubType, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  EXPECT_EQ(std::string("missing required architecture x86_64 in file ") +
                getTempFilePath(),
            errorMessage);
}

TEST_F(LibTapiTest_TBDv2, LIF_Swift_1_0) {
  static const char tbd_v2_swift_1_0[] = "--- !tapi-tbd-v2\n"
                                         "archs: [ arm64 ]\n"
                                         "platform: ios\n"
                                         "install-name: Test.dylib\n"
                                         "swift-version: 1.0\n"
                                         "...\n";

  writeTempFile(tbd_v2_swift_1_0);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::ExactCpuSubType, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(1U, file->getSwiftVersion());
}

TEST_F(LibTapiTest_TBDv2, LIF_Swift_1_1) {
  static const char tbd_v2_swift_1_1[] = "--- !tapi-tbd-v2\n"
                                         "archs: [ arm64 ]\n"
                                         "platform: ios\n"
                                         "install-name: Test.dylib\n"
                                         "swift-version: 1.1\n"
                                         "...\n";

  writeTempFile(tbd_v2_swift_1_1);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::ExactCpuSubType, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(2U, file->getSwiftVersion());
}

TEST_F(LibTapiTest_TBDv2, LIF_Swift_2_0) {
  static const char tbd_v2_swift_2_0[] = "--- !tapi-tbd-v2\n"
                                         "archs: [ arm64 ]\n"
                                         "platform: ios\n"
                                         "install-name: Test.dylib\n"
                                         "swift-version: 2.0\n"
                                         "...\n";

  writeTempFile(tbd_v2_swift_2_0);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::ExactCpuSubType, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(3U, file->getSwiftVersion());
}

TEST_F(LibTapiTest_TBDv2, LIF_Swift_3_0) {
  static const char tbd_v2_swift_3_0[] = "--- !tapi-tbd-v2\n"
                                         "archs: [ arm64 ]\n"
                                         "platform: ios\n"
                                         "install-name: Test.dylib\n"
                                         "swift-version: 3.0\n"
                                         "...\n";

  writeTempFile(tbd_v2_swift_3_0);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::ExactCpuSubType, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(4U, file->getSwiftVersion());
}

TEST_F(LibTapiTest_TBDv2, LIF_Swift_4_0) {
  static const char tbd_v2_swift_4_0[] = "--- !tapi-tbd-v2\n"
                                         "archs: [ arm64 ]\n"
                                         "platform: ios\n"
                                         "install-name: Test.dylib\n"
                                         "swift-version: 4.0\n"
                                         "...\n";
  writeTempFile(tbd_v2_swift_4_0);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::ExactCpuSubType, PackedVersion32(10, 11, 0), errorMessage));
  EXPECT_EQ(nullptr, file);
  EXPECT_EQ(std::string("malformed file\n") + getTempFilePath() +
                ":5:16: error: invalid Swift ABI "
                "version.\nswift-version: 4.0\n               ^~~\n",
            errorMessage);
}

TEST_F(LibTapiTest_TBDv2, LIF_Swift_5) {
  static const char tbd_v2_swift_5[] = "--- !tapi-tbd-v2\n"
                                       "archs: [ arm64 ]\n"
                                       "platform: ios\n"
                                       "install-name: Test.dylib\n"
                                       "swift-version: 5\n"
                                       "...\n";
  writeTempFile(tbd_v2_swift_5);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::ExactCpuSubType, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(5U, file->getSwiftVersion());
}

TEST_F(LibTapiTest_TBDv2, LIF_Swift_99) {
  static const char tbd_v2_swift_99[] = "--- !tapi-tbd-v2\n"
                                        "archs: [ arm64 ]\n"
                                        "platform: ios\n"
                                        "install-name: Test.dylib\n"
                                        "swift-version: 99\n"
                                        "...\n";
  writeTempFile(tbd_v2_swift_99);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::ExactCpuSubType, PackedVersion32(10, 11, 0), errorMessage));
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(99U, file->getSwiftVersion());
}

TEST_F(LibTapiTest_TBDv2, LIF_Platform_macOS) {
  static const char tbd_v2_platform_macos[] = "--- !tapi-tbd-v2\n"
                                              "archs: [ x86_64 ]\n"
                                              "platform: macosx\n"
                                              "install-name: Test.dylib\n"
                                              "...\n";
  writeTempFile(tbd_v2_platform_macos);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_ALL,
      ParsingFlags::ExactCpuSubType, PackedVersion32(10, 12, 0), errorMessage));
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_MACOS}, file->getPlatformSet());
}

TEST_F(LibTapiTest_TBDv2, LIF_Platform_iOS) {
  static const char tbd_v2_platform_ios[] = "--- !tapi-tbd-v2\n"
                                            "archs: [ arm64 ]\n"
                                            "platform: ios\n"
                                            "install-name: Test.dylib\n"
                                            "...\n";
  writeTempFile(tbd_v2_platform_ios);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::ExactCpuSubType, PackedVersion32(10, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_IOS}, file->getPlatformSet());
}

TEST_F(LibTapiTest_TBDv2, LIF_Platform_watchOS) {
  static const char tbd_v2_platform_watchos[] = "--- !tapi-tbd-v2\n"
                                                "archs: [ armv7k ]\n"
                                                "platform: watchos\n"
                                                "install-name: Test.dylib\n"
                                                "...\n";
  writeTempFile(tbd_v2_platform_watchos);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7K,
      ParsingFlags::ExactCpuSubType, PackedVersion32(3, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_WATCHOS}, file->getPlatformSet());
}

TEST_F(LibTapiTest_TBDv2, LIF_Platform_tvOS) {
  static const char tbd_v2_platform_tvos[] = "--- !tapi-tbd-v2\n"
                                             "archs: [ arm64 ]\n"
                                             "platform: tvos\n"
                                             "install-name: Test.dylib\n"
                                             "...\n";
  writeTempFile(tbd_v2_platform_tvos);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
      ParsingFlags::ExactCpuSubType, PackedVersion32(10, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_TVOS}, file->getPlatformSet());
}

TEST_F(LibTapiTest_TBDv2, LIF_Platform_bridgeOS) {
  static const char tbd_v2_platform_bridgeos[] = "--- !tapi-tbd-v2\n"
                                                 "archs: [ armv7k ]\n"
                                                 "platform: bridgeos\n"
                                                 "install-name: Test.dylib\n"
                                                 "...\n";
  writeTempFile(tbd_v2_platform_bridgeos);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7K,
      ParsingFlags::ExactCpuSubType, PackedVersion32(2, 0, 0), errorMessage));
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_BRIDGEOS}, file->getPlatformSet());
}

TEST_F(LibTapiTest_TBDv2, LIF_Load_unknown_arch) {
  static const char tbd_v2_unknown_arch[] =
      "--- !tapi-tbd-v2\n"
      "archs: [ x86_64, FooBar ]\n"
      "uuids: [ 'x86_64: AEB543A6-A3DC-3B55-B5CB-E6C94B18CE12',\n"
      "         'FooBar: A63F8D6C-FF22-375E-A678-1C1B28A076C0' ]\n"
      "platform: macosx\n"
      "install-name: Test.dylib\n"
      "exports:\n"
      "  - archs: [ x86_64, FooBar ]\n"
      "    symbols: [ _sym1 ]\n"
      "  - archs: [ FooBar ]\n"
      "    symbols: [ _sym2 ]\n"
      "...\n";

  writeTempFile(tbd_v2_unknown_arch);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::ExactCpuSubType, PackedVersion32(10, 12, 0), errorMessage));

  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);

  EXPECT_EQ(std::vector<uint32_t>{PLATFORM_MACOS}, file->getPlatformSet());
  EXPECT_EQ(std::string("Test.dylib"), file->getInstallName());

  ASSERT_EQ(1U, file->exports().size());
  EXPECT_EQ("_sym1", file->exports().front().getName());
}


#pragma clang diagnostic pop
} // end namespace
