//===- AppleBitcodeCompatibility.cpp - Bitcode Compatibility Checks -------===//
//
//                     The LLVM Compiler Infrastructure
//
//                            APPLE INTERNAL ONLY
//
/// Add Apple specific producer string in the bitcode identification block, and
/// check when reading bitcode that it is not produced by a future Apple
/// toolchain by parsing the version string.
//
//===----------------------------------------------------------------------===//

#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/DiagnosticPrinter.h"

#include <regex>

using namespace llvm;

namespace {

std::error_code error(DiagnosticHandlerFunction DiagnosticHandler,
                      std::error_code EC, const Twine &Message) {
  BitcodeDiagnosticInfo DI(EC, DS_Error, Message);
  DiagnosticHandler(DI);
  return EC;
}
}

namespace llvm {

std::error_code checkV1(const DiagnosticHandlerFunction &DiagnosticHandler,
                        const std::string &ProducerIdentification,
                        const std::string &CurrentVersion,
                        int CurrentMajorVersion, int CurrentMinorVersion,
                        int CurrentSecondMinorVersion, int CurrentPatchVersion);

// Check if the bitcode is compatible. Reject bitcode produced by a future
// major version of our toolchain and silently accept future minor version or
// unknown producers.
//
// The identification block in the bitcode is populated with a producer
// string. For our internal toolchain we will use it for versioning, the
// current scheme is to use the LLVM_VERSION_STRING which is expected to
// be set to the current version (e.g. 702.1.65.2). We reject bitcode
// generated with a more recent major number and accept anything else.
//
// A prefix is added and used as a "magic" to recognize our own generated
// string. The current prefix is APPLE_1. The 1 is used to version the
// the version number format. Finally a suffix can be added to manually
// bump an incompatibility inside a major version number.
//
// A valid full identification string can then be:
//   "APPLE_1_703.1.53.4_0"
//  or
//   "APPLE_1_703.1.53_0"
//
std::error_code
checkBitcodeCompatibility(const DiagnosticHandlerFunction &DiagnosticHandler,
                          const std::string &ProducerIdentification,
                          const std::string &CurrentVersion,
                          int CurrentPatchVersion) {
  // The only format we know about XXX.XX.XX(.XX)
  const std::string CurrentVersionFormat =
      "([[:digit:]]+)\\.([[:digit:]]+)\\.([[:digit:]]+)\\.?([[:digit:]]*)";
  std::smatch Match;
  if (!std::regex_search(CurrentVersion, Match,
                         std::regex(CurrentVersionFormat))) {
    // The current build does not contain a valid Apple version
    return std::error_code();
  }
  int CurrentMajorVersion = strtol(Match[1].str().c_str(), nullptr, 10);
  // int CurrentBranch = strtol(Match[2].str().c_str(), nullptr, 10);
  int CurrentMinorVersion = strtol(Match[3].str().c_str(), nullptr, 10);
  int CurrentSecondMinorVersion = strtol(Match[4].str().c_str(), nullptr, 10);

  // The format for the Magic is "APPLE" followed by a number describing the
  // versioning scheme (how to parse the rest of the string).
  // Currently only version "1" is supported, see below.
  std::regex Magic("^APPLE_([[:digit:]]+)_");
  if (!std::regex_search(ProducerIdentification, Match, Magic)) {
    // This bitcode is not produced by an Apple toolchain, ignore
    return std::error_code();
  }

  int Version = strtol(Match[1].str().c_str(), nullptr, 10);
  switch (Version) {
  default:
  case 0: {
    // Shouldn't happen, the regex matched but the version is 0.
    // This bitcode is not produced by an Apple toolchain, ignore.
    return std::error_code();
  }
  case 1:
    return checkV1(DiagnosticHandler, ProducerIdentification, CurrentVersion,
                   CurrentMajorVersion, CurrentMinorVersion,
                   CurrentSecondMinorVersion, CurrentPatchVersion);
    break;
  }

  return std::error_code();
}

std::error_code
checkBitcodeCompatibility(const DiagnosticHandlerFunction &DiagnosticHandler,
                          const std::string &ProducerIdentification,
                          const std::string &CurrentVersion) {
  return checkBitcodeCompatibility(DiagnosticHandler, ProducerIdentification,
                                   CurrentVersion, AppleCurrentPatchVersion);
}

/// Check implementation for V1 of the versioning string
std::error_code checkV1(const DiagnosticHandlerFunction &DiagnosticHandler,
                        const std::string &ProducerIdentification,
                        const std::string &CurrentVersion,
                        int CurrentMajorVersion, int CurrentMinorVersion,
                        int CurrentSecondMinorVersion,
                        int CurrentPatchVersion) {
  auto ErrorMsgId = [&](Twine Version) -> std::string {
    return std::string(" (Producer: '") + Version.str() + "' Reader: '" +
           CurrentVersion + "_" + Twine(CurrentPatchVersion).str() + "')";
  };

  // The expected format for version 1 is "APPLE_1_XXX.XX.XX(.XX)_X
  std::regex Format("^APPLE_1_([[:digit:]]+)\\.([[:digit:]]+)\\.([[:digit:]]+)"
                    "\\.?([[:digit:]]*)_([[:digit:]]+)$");
  std::smatch Match;
  if (!std::regex_search(ProducerIdentification, Match, Format)) {
    // This is an issue: we can't parse this string with the expected format
    return error(
        DiagnosticHandler, make_error_code(BitcodeError::CorruptedBitcode),
        "Invalid bitcode version" + ErrorMsgId(ProducerIdentification));
  }
  int MajorVersion = strtol(Match[1].str().c_str(), nullptr, 10);
  int Branch = strtol(Match[2].str().c_str(), nullptr, 10);
  int MinorVersion = strtol(Match[3].str().c_str(), nullptr, 10);
  int SecondMinorVersion = strtol(Match[4].str().c_str(), nullptr, 10);
  int PatchVersion = strtol(Match[5].str().c_str(), nullptr, 10);
  auto ErrorMsg = [&]() {
    return std::string("Invalid bitcode version") +
           ErrorMsgId(Twine(MajorVersion) + "." + Twine(Branch) + "." +
                      Twine(MinorVersion) + "." + Twine(SecondMinorVersion) +
                      "_" + Twine(PatchVersion));
  };
  // A major number from the future is a failure
  if (MajorVersion > CurrentMajorVersion) {
    return error(DiagnosticHandler,
                 make_error_code(BitcodeError::CorruptedBitcode), ErrorMsg());
  }
  // Same major revision, but we manually bumped the "patch"
  if (MajorVersion == CurrentMajorVersion &&
      PatchVersion > CurrentPatchVersion) {
    return error(DiagnosticHandler,
                 make_error_code(BitcodeError::CorruptedBitcode), ErrorMsg());
  }
  return std::error_code();
}
}
