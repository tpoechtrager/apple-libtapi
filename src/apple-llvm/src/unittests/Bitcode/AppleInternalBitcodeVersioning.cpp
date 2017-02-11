//=- unittests/Bitcode/AppleInternalBitcodeVersioning.cpp - Bitcode Version -=//
//
//                     APPLE INTERNAL
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/AsmParser/Parser.h"
#include "llvm/Bitcode/BitstreamReader.h"
#include "llvm/Bitcode/BitstreamWriter.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/DiagnosticPrinter.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/DataStream.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/StreamingMemoryObject.h"
#include "gtest/gtest.h"

using namespace llvm;

namespace llvm {
std::error_code
checkBitcodeCompatibility(const DiagnosticHandlerFunction &DiagnosticHandler,
                          const std::string &ProducerIdentification,
                          const std::string &CurrentVersion,
                          int CurrentPatchVersion);
}

namespace {
// Store error code and a string representing the diagnostic.
using DiagInfo = std::pair<int, std::string>;
struct DiagHandler {
  std::vector<DiagInfo> *Diags;
  void operator()(const DiagnosticInfo &Info) {
    ASSERT_EQ(Info.getKind(), DK_Bitcode);
    auto Diag = static_cast<const BitcodeDiagnosticInfo &>(Info);
    std::string S;
    raw_string_ostream OS(S);
    DiagnosticPrinterRawOStream Printer(OS);
    Diag.print(Printer);
    // errs() << "Diagnose '" << OS.str() << "'\n";
    Diags->emplace_back(Diag.getError().value(), OS.str());
  }
};
}

// Do not diagnostic unknown version
TEST(BitcodeVersioning, OpenSource) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC = checkBitcodeCompatibility(Diag, "LLVM3.7.0svn", "702.1.2.3", 0);
  EXPECT_EQ(EC, std::error_code());
  ASSERT_TRUE(Diags.empty());
}

// Matching version
TEST(BitcodeVersioning, Match) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC =
      checkBitcodeCompatibility(Diag, "APPLE_1_702.1.2.3_0", "702.1.2.3", 0);
  EXPECT_EQ(EC, std::error_code());
  EXPECT_TRUE(Diags.empty());
}

// Matching version, no second minor
TEST(BitcodeVersioning, MatchNoSecondMinor) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC = checkBitcodeCompatibility(Diag, "APPLE_1_702.1.2_0", "702.1.2", 0);
  EXPECT_EQ(EC, std::error_code());
  EXPECT_TRUE(Diags.empty());
}

// Identity is either a match or a warning
TEST(BitcodeVersioning, CurrentVersionIdentity) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC = checkBitcodeCompatibility(Diag, APPLE_BITCODE_VERSION,
                                      APPLE_BITCODE_VERSION, 0);
  EXPECT_EQ(EC, std::error_code());
  EXPECT_TRUE(Diags.empty() || Diags[0].first == std::error_code().value());
}

// Can't parse this version
TEST(BitcodeVersioning, WrongFormat) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC = checkBitcodeCompatibility(Diag, "APPLE_1_702.1_0", "702.1.2.3", 0);
  EXPECT_EQ(EC.value(), static_cast<int>(BitcodeError::CorruptedBitcode));
  ASSERT_FALSE(Diags.empty());
  EXPECT_EQ(Diags[0].first, static_cast<int>(BitcodeError::CorruptedBitcode));
  EXPECT_EQ(Diags[0].second, "Invalid bitcode version (Producer: "
                             "'APPLE_1_702.1_0' Reader: '702.1.2.3_0')");
}

// Reject major
TEST(BitcodeVersioning, RejectMajor) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC =
      checkBitcodeCompatibility(Diag, "APPLE_1_702.1.2.3_0", "700.1.2.3", 0);
  EXPECT_EQ(EC.value(), static_cast<int>(BitcodeError::CorruptedBitcode));
  ASSERT_FALSE(Diags.empty());
  EXPECT_EQ(Diags[0].first, static_cast<int>(BitcodeError::CorruptedBitcode));
  EXPECT_EQ(Diags[0].second, "Invalid bitcode version (Producer: "
                             "'702.1.2.3_0' Reader: '700.1.2.3_0')");
}

// Accept older major
TEST(BitcodeVersioning, AcceptOlderMajor) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC =
      checkBitcodeCompatibility(Diag, "APPLE_1_700.1.2.3_0", "702.1.2.3", 0);
  EXPECT_EQ(EC, std::error_code());
  EXPECT_TRUE(Diags.empty());
}

// Reject newer patch, same major
TEST(BitcodeVersioning, RejectNewerPatchSameMajor) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC =
      checkBitcodeCompatibility(Diag, "APPLE_1_702.1.2.3_4", "702.1.2.3", 3);
  EXPECT_EQ(EC.value(), static_cast<int>(BitcodeError::CorruptedBitcode));
  ASSERT_FALSE(Diags.empty());
  EXPECT_EQ(Diags[0].first, static_cast<int>(BitcodeError::CorruptedBitcode));
  EXPECT_EQ(Diags[0].second, "Invalid bitcode version (Producer: "
                             "'702.1.2.3_4' Reader: '702.1.2.3_3')");
}

// Accept older patch
TEST(BitcodeVersioning, AcceptOlderPatchSameMajor) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC =
      checkBitcodeCompatibility(Diag, "APPLE_1_702.1.2.3_1", "702.1.2.3", 2);
  EXPECT_EQ(EC, std::error_code());
  EXPECT_TRUE(Diags.empty());
}

// Accept newer patch, older major
TEST(BitcodeVersioning, AcceptNewerPatchOlderMajor) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC =
      checkBitcodeCompatibility(Diag, "APPLE_1_700.1.2.3_6", "702.1.2.3", 0);
  EXPECT_EQ(EC, std::error_code());
  EXPECT_TRUE(Diags.empty());
}

// Do not warn on newer minor and older major
TEST(BitcodeVersioning, DoNotWarnOnOlderMinorAndSameMajor) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC =
      checkBitcodeCompatibility(Diag, "APPLE_1_702.1.2_0", "702.1.5.3", 0);
  EXPECT_EQ(EC, std::error_code());
  EXPECT_TRUE(Diags.empty());
}

// Do not warn on newer minor, same major.
TEST(BitcodeVersioning, DoNotWarnOnNewerMinor) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC =
      checkBitcodeCompatibility(Diag, "APPLE_1_700.1.5_0", "700.1.4.3", 0);
  EXPECT_EQ(EC, std::error_code());
  ASSERT_TRUE(Diags.empty());
}

// Do not warn on newer minor and older major
TEST(BitcodeVersioning, DoNotWarnOnNewerMinorAndOlderMajor) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC =
      checkBitcodeCompatibility(Diag, "APPLE_1_700.1.6_0", "702.1.2.3", 0);
  EXPECT_EQ(EC, std::error_code());
  EXPECT_TRUE(Diags.empty());
}

// Do not warn on newer minor and older major
TEST(BitcodeVersioning, DoNotWarnOnOlderSecondMinorAndSameMinorMajor) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC =
      checkBitcodeCompatibility(Diag, "APPLE_1_702.1.2.3_0", "702.1.2.5", 0);
  EXPECT_EQ(EC, std::error_code());
  EXPECT_TRUE(Diags.empty());
}

// Do not warn on newer minor, same major.
TEST(BitcodeVersioning, DoNotWarnOnNewerSecondMinorAndSameMinorMajor) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC =
      checkBitcodeCompatibility(Diag, "APPLE_1_700.1.2.5_0", "700.1.2.4", 0);
  EXPECT_EQ(EC, std::error_code());
  ASSERT_TRUE(Diags.empty());
}

// Do not warn on newer minor and older major
TEST(BitcodeVersioning, DoNotWarnOnSecondNewerMinorAndOlderMinorMajor) {
  std::vector<DiagInfo> Diags;
  DiagHandler Diag{&Diags};
  auto EC =
      checkBitcodeCompatibility(Diag, "APPLE_1_702.1.2.6_0", "702.1.6.3", 0);
  EXPECT_EQ(EC, std::error_code());
  EXPECT_TRUE(Diags.empty());
  EC = checkBitcodeCompatibility(Diag, "APPLE_1_700.1.2.6_0", "702.1.2.3", 0);
  EXPECT_EQ(EC, std::error_code());
  EXPECT_TRUE(Diags.empty());
}
