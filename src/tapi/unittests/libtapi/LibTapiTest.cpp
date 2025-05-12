//===-- LibTapiTest.h - Lib Tapi Test -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// Implementation of helpers for libtapi tests.
//===----------------------------------------------------------------------===//
#include "LibTapiTest.h"

void LibTapiTest::testPlatform(const std::string &target, unsigned platform,
                               unsigned cpuType, unsigned subType, bool isYAML,
                               tapi::PackedVersion32 expectedDV) {
  std::string TBD;
  if (isYAML)
    TBD = std::string("--- !tapi-tbd\n") + "tbd-version: 4\n" + "targets: [ " +
          target + " ]\n" + "install-name: Test.dylib\n" + "...\n";
  else
    TBD = std::string(R"({"tapi_tbd_version": 5,)") +
          R"("main_library":{"install_names":[{ "name": "Test.dylib" }],)" +
          R"("target_info": [{"min_deployment": "11.0", "target": ")" + target +
          "\"}]}}";
  writeTempFile(TBD);
  std::string errorMessage;
  auto file = std::unique_ptr<tapi::LinkerInterfaceFile>(
      tapi::LinkerInterfaceFile::create(getTempFilePath(), cpuType, subType,
                                        tapi::ParsingFlags::ExactCpuSubType,
                                        tapi::PackedVersion32(1, 0, 0),
                                        errorMessage));
  ASSERT_NE(nullptr, file);
  if (isYAML) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
    EXPECT_EQ(std::vector<uint32_t>{platform}, file->getPlatformSet());
#pragma clang diagnostic pop
  } else {
    std::pair<uint32_t, tapi::PackedVersion32> expected = {platform,
                                                           expectedDV};
    EXPECT_EQ(1u, file->getPlatformsAndMinDeployment().size());
    EXPECT_EQ(expected, file->getPlatformsAndMinDeployment()[0]);
  }
}
