//===-- LinkerInterfaceFileTest_TBD_v5.cpp - Linker Interface File Test ---===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
#include "LibTapiTest.h"
#include "tapi/Core/ArchitectureConfig.h"
#include "llvm/TextAPI/Target.h"
#include "gtest/gtest.h"
#include <mach-o/loader.h>
#include <mach/machine.h>
#include <tapi/tapi.h>

using namespace tapi;

#define DEBUG_TYPE "libtapi-test"


namespace {

class LibTapiTest_TBDv5 : public LibTapiTest {};

static const char tbd_v5_file[] = R"({
"main_library": {
    "allowable_clients": [
        {
            "clients": [
                "ClientA",
                "ClientB"
            ]
        }
    ],
    "compatibility_versions": [
        {
            "version": "1.2"
        }
    ],
    "current_versions": [
        {
            "version": "1.2.3"
        }
    ],
    "exported_symbols": [
        {
            "data": {
                "global": [
                    "_symA"
                ],
                "weak": [
                    "_weak5"
                ]
            },
            "targets": [
                "i386-macos"
            ]
        },
        {
            "data": {
                "global": [
                    "_symB"
                ],
                "weak": [
                    "_weak0"
                ]
            },
            "targets": [
                "x86_64-maccatalyst"
            ]
        },
        {
            "data": {
                "global": [
                    "_symAB"
                ],
                "weak": [
                    "_weak2",
                    "_weak1"
                ]
            },
            "targets": [
                "x86_64-macos",
                "x86_64-maccatalyst"
            ]
        }
    ],
    "flags": [
        {
            "attributes": [
                "flat_namespace"
            ]
        }
    ],
    "install_names": [
        {
            "name": "/System/Library/Frameworks/Umbrella.framework/Umbrella"
        }
    ],
    "parent_umbrellas": [
        {
            "umbrella": "System"
        }
    ],
    "rpaths": [
      {
        "targets": [
            "x86_64-macos"
        ],
        "paths": [
            "@executable_path/.../Frameworks"
        ]
      }
    ],
    "reexported_libraries": [
        {
            "names": [
                "/System/Library/Frameworks/A.framework/A"
            ],
            "targets": [
                "i386-macos"
            ]
        },
        {
            "names": [
                "/System/Library/Frameworks/B.framework/B",
                "/System/Library/Frameworks/C.framework/C"
            ],
            "targets": [
                "x86_64-macos",
                "x86_64-maccatalyst"
            ]
        }
    ],
    "reexported_symbols": [
        {
            "data": {
                "global": [
                    "_symC"
                ]
            },
            "targets": [
                "i386-macos"
            ]
        }
    ],
    "swift_abi": [
        {
            "abi": 5
        }
    ],
    "target_info": [
        {
            "min_deployment": "0",
            "target": "i386-macos"
        },
        {
            "min_deployment": "0",
            "target": "x86_64-macos"
        },
        {
            "min_deployment": "0",
            "target": "x86_64-maccatalyst"
        }
    ],
    "undefined_symbols": [
        {
            "data": {
                "global": [
                    "_symD"
                ],
                "weak": [
                    "_weak6"
                ]
            },
            "targets": [
                "i386-macos"
            ]
        },
        {
            "data": {
                "weak": [
                    "_weak4",
                    "_weak3"
                ]
            },
            "targets": [
                "x86_64-maccatalyst"
            ]
        }
    ]
},
"tapi_tbd_version": 5
})";

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

TEST_F(LibTapiTest_TBDv5, LIF_isSupported) {
  llvm::StringRef buffer(tbd_v5_file);
  bool isSupported = LinkerInterfaceFile::isSupported(
      "Test.tbd", reinterpret_cast<const uint8_t *>(buffer.data()),
      buffer.size());
  EXPECT_TRUE(isSupported);
}

// Test parsing a .tbd file from a memory buffer/nmapped file.
TEST_F(LibTapiTest_TBDv5, LIF_Load) {
  writeTempFile(tbd_v5_file);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::None, PackedVersion32(10, 15, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);

// Validate that deprecated API still works with newer format.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
  EXPECT_EQ((std::vector<uint32_t>{PLATFORM_MACOS, PLATFORM_MACCATALYST}),
            file->getPlatformSet());
#pragma clang diagnostic pop
  EXPECT_EQ(
      std::string("/System/Library/Frameworks/Umbrella.framework/Umbrella"),
      file->getInstallName());
  EXPECT_TRUE(file->isApplicationExtensionSafe());
  EXPECT_FALSE(file->hasTwoLevelNamespace());
  EXPECT_TRUE(file->hasReexportedLibraries());
}

TEST_F(LibTapiTest_TBDv5, LIF_LoadSymbols_x86_64) {
  writeTempFile(tbd_v5_file);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::None, PackedVersion32(10, 15, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);

  ExportedSymbolSeq tbd_v5_exports = {
      {"_symAB", false, false, false}, {"_symB", false, false, false},
      {"_weak0", true, false, false},  {"_weak1", true, false, false},
      {"_weak2", true, false, false},
  };

  ExportedSymbolSeq tbd_v5_undefs = {
      {"_weak3", false, false, true},
      {"_weak4", false, false, true},
  };

  checkSyms(tbd_v5_exports, file->exports());
  checkSyms(tbd_v5_undefs, file->undefineds());
}

TEST_F(LibTapiTest_TBDv5, LIF_LoadSymbols_i386) {
  writeTempFile(tbd_v5_file);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_I386, CPU_SUBTYPE_I386_ALL,
      ParsingFlags::None, PackedVersion32(10, 15, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);

  ExportedSymbolSeq tbd_v5_exports = {
      {"_symA", false, false, false},
      {"_symC", false, false, false},
      {"_weak5", true, false, false},
  };

  ExportedSymbolSeq tbd_v5_undefs = {
      {"_symD", false, false, false},
      {"_weak6", false, false, true},
  };

  checkSyms(tbd_v5_exports, file->exports());
  checkSyms(tbd_v5_undefs, file->undefineds());
}

TEST_F(LibTapiTest_TBDv5, LIF_Platform_macOS) {
  testPlatform("x86_64-macos", PLATFORM_MACOS, CPU_TYPE_X86_64,
               CPU_SUBTYPE_X86_ALL, /*isYAML=*/false);
}

TEST_F(LibTapiTest_TBDv5, LIF_Platform_driverkit) {
  testPlatform("x86_64-driverkit", PLATFORM_DRIVERKIT, CPU_TYPE_X86_64,
               CPU_SUBTYPE_X86_ALL, /*isYAML=*/false);
}

TEST_F(LibTapiTest_TBDv5, LIF_Platform_iOS) {
  testPlatform("arm64-ios", PLATFORM_IOS, CPU_TYPE_ARM64, CPU_SUBTYPE_ARM64_ALL,
               /*isYAML=*/false);
}

TEST_F(LibTapiTest_TBDv5, LIF_Platform_watchOS) {
  testPlatform("armv7k-watchos", PLATFORM_WATCHOS, CPU_TYPE_ARM,
               CPU_SUBTYPE_ARM_V7K, /*isYAML=*/false);
}

TEST_F(LibTapiTest_TBDv5, LIF_Platform_tvOS) {
  testPlatform("arm64-tvos", PLATFORM_TVOS, CPU_TYPE_ARM64,
               CPU_SUBTYPE_ARM64_ALL, /*isYAML=*/false);
}

TEST_F(LibTapiTest_TBDv5, LIF_Platform_bridgeOS) {
  testPlatform("arm64-bridgeos", PLATFORM_BRIDGEOS, CPU_TYPE_ARM64,
               CPU_SUBTYPE_ARM64_ALL, /*isYAML=*/false);
}


TEST_F(LibTapiTest_TBDv5, LIF_MalformedFile) {
  static const char input[] = R"({ 
"tapi_tbd_version": 5,
"main_library": {
  "target_info": [
    {
      "target": "armv7-ios",
      "min_deployment": "9.0"
    }
  ],
  "install_names":[
    { "name":"/S/L/F/Foo.framework/Foo" }
  ],
  "exported_symbols": [
    {
      "daa": {
        "global": {
            "weak": []
          }
      }
    }
  ]
}})";

  writeTempFile(input);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_ARM, CPU_SUBTYPE_ARM_V7,
      ParsingFlags::ExactCpuSubType, PackedVersion32(9, 0, 0), errorMessage));
  ASSERT_EQ(nullptr, file);
  ASSERT_EQ("invalid exported_symbols section\n", errorMessage);
}

TEST_F(LibTapiTest_TBDv5, LIF_LegacyLinkerSymbols) {
  static const char input[] = R"({ 
"tapi_tbd_version": 5,
"main_library": {
  "target_info": [
    {
      "target": "x86_64-macos",
      "min_deployment": "10.14"
    }
  ],
  "install_names":[
    { "name":"usr/lib/libfoo.1.dylib" }
  ],
  "exported_symbols": [
   { "data" : {
     "global": [
          "$ld$add$os10.15$_symA",
          "$ld$add$os10.14$_symB",
          "$ld$hide$os10.14$_symC, _symC",
          "$ld$install_name$os10.14$/usr/lib/libfoo.1.dylib",
          "$ld$unsupport_action$some_os$some_sym"
      ]
    }
   }
  ]
}})";
  writeTempFile(input);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::None, PackedVersion32(10, 14, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);

  ExportedSymbolSeq tbd_v5_exports = {
      {"_symB", false, false, false},
  };

  checkSyms(tbd_v5_exports, file->exports());
  ASSERT_TRUE(file->isInstallNameVersionSpecific());
  ASSERT_EQ(file->getInstallName(), "/usr/lib/libfoo.1.dylib");
}

TEST_F(LibTapiTest_TBDv5, LIF_NewLinkerSymbols) {
  static const char input[] = R"({ 
"tapi_tbd_version": 5,
"main_library": {
  "target_info": [
    {
      "target": "x86_64-macos",
      "min_deployment": "10.14"
    }
  ],
  "install_names":[
    { "name":"/S/L/F/Foo.framework/Foo" }
  ],
  "exported_symbols": [
   { "data" : {
     "global": ["$ld$previous$/usr/lib/libbar.dylib$$1$10.9$10.16$_foo$"]
    }
   }
  ]
}})";
  writeTempFile(input);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::None, PackedVersion32(10, 14, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);
  std::pair<uint32_t, PackedVersion32> expectedPlatform{
      PLATFORM_MACOS, PackedVersion32(10, 14, 0)};
  auto platforms = file->getPlatformsAndMinDeployment();
  EXPECT_EQ(platforms.size(), 1U);
  EXPECT_EQ(platforms[0], expectedPlatform);

  ExportedSymbolSeq tbd_v5_exports = {
      {"$ld$previous$/usr/lib/libbar.dylib$$1$10.9$10.16$_foo$", false, false,
       false},
  };
  checkSyms(tbd_v5_exports, file->exports());
  ASSERT_FALSE(file->isInstallNameVersionSpecific());
}
TEST_F(LibTapiTest_TBDv5, LIF_RPaths) {
  static const char input[] = R"({ 
"tapi_tbd_version": 5,
"main_library": {
  "target_info": [
    {
      "target": "x86_64-macos",
      "min_deployment": "10.14"
    },
    {
      "target": "arm644-macos",
      "min_deployment": "10.14"
    }
  ],
  "install_names":[
    { "name":"@rpath/foo.dylib" }
  ],
  "rpaths": [
   { 
     "paths": ["@executable_path/../lib"]
   },
   { 
      "targets": [
          "arm64-macos"
      ],
     "paths": ["@executable_path/../Frameworks"]
   }
  ]
}})";
  writeTempFile(input);
  std::string errorMessage;
  auto file = std::unique_ptr<LinkerInterfaceFile>(LinkerInterfaceFile::create(
      getTempFilePath(), CPU_TYPE_X86_64, CPU_SUBTYPE_X86_64_ALL,
      ParsingFlags::None, PackedVersion32(10, 14, 0), errorMessage));
  ASSERT_TRUE(errorMessage.empty());
  ASSERT_NE(nullptr, file);
  std::vector<std::string> rpaths = {"@executable_path/../lib"};
  EXPECT_EQ(rpaths, file->rPaths());
}

} // end namespace
