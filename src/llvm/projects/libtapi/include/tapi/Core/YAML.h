//===- tapi/Core/YAML.h - YAML ----------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines common YAML mappings
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_CORE_YAML_H
#define TAPI_CORE_YAML_H

#include "tapi/Core/Architecture.h"
#include "tapi/Core/ArchitectureSet.h"
#include "tapi/Core/ArchitectureSupport.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/YAMLTraits.h"
#include "tapi/Core/AvailabilityInfo.h"

using UUID = std::pair<TAPI_INTERNAL::Architecture, std::string>;

LLVM_YAML_STRONG_TYPEDEF(llvm::StringRef, FlowStringRef)
LLVM_YAML_STRONG_TYPEDEF(uint8_t, SwiftVersion)
LLVM_YAML_IS_FLOW_SEQUENCE_VECTOR(UUID)
LLVM_YAML_IS_FLOW_SEQUENCE_VECTOR(FlowStringRef)

namespace llvm {
namespace yaml {

template <> struct ScalarTraits<FlowStringRef> {
  using Impl = ScalarTraits<StringRef>;
  static void output(const FlowStringRef &value, void *ctx, raw_ostream &os) {
    Impl::output(value, ctx, os);
  }
  static StringRef input(StringRef value, void *ctx, FlowStringRef &out) {
    return Impl::input(value, ctx, out.value);
  }
  static bool mustQuote(StringRef name) { return Impl::mustQuote(name); }
};

using tapi::ObjCConstraint;
template <> struct ScalarEnumerationTraits<ObjCConstraint> {
  static void enumeration(IO &io, ObjCConstraint &constraint) {
    io.enumCase(constraint, "none", ObjCConstraint::None);
    io.enumCase(constraint, "retain_release", ObjCConstraint::Retain_Release);
    io.enumCase(constraint, "retain_release_for_simulator",
                ObjCConstraint::Retain_Release_For_Simulator);
    io.enumCase(constraint, "retain_release_or_gc",
                ObjCConstraint::Retain_Release_Or_GC);
    io.enumCase(constraint, "gc", ObjCConstraint::GC);
  }
};

using tapi::Platform;
template <> struct ScalarEnumerationTraits<Platform> {
  static void enumeration(IO &io, Platform &platform) {
    io.enumCase(platform, "unknown", Platform::Unknown);
    io.enumCase(platform, "macosx", Platform::OSX);
    io.enumCase(platform, "ios", Platform::iOS);
    io.enumCase(platform, "watchos", Platform::watchOS);
    io.enumCase(platform, "tvos", Platform::tvOS);
    io.enumCase(platform, "bridgeos", Platform::bridgeOS);
  }
};

using TAPI_INTERNAL::Architecture;
using TAPI_INTERNAL::ArchitectureSet;
template <> struct ScalarBitSetTraits<ArchitectureSet> {
  static void bitset(IO &io, ArchitectureSet &archs) {
#define ARCHINFO(arch, type, subtype)                                          \
  io.bitSetCase(archs, #arch, 1U << static_cast<int>(Architecture::arch));
#include "tapi/Core/Architecture.def"
#undef ARCHINFO
  }
};

// FIXME: Once the YAML parser supports default cases, we can conver this back
// to a ScalarEnumerationTraits.
using TAPI_INTERNAL::getArchType;
template <> struct ScalarTraits<Architecture> {
  static void output(const Architecture &value, void *, raw_ostream &os) {
    os << value;
  }

  static StringRef input(StringRef scalar, void *, Architecture &value) {
    value = getArchType(scalar);
    return {};
  }

  static bool mustQuote(StringRef) { return false; }
};

using TAPI_INTERNAL::PackedVersion;
template <> struct ScalarTraits<PackedVersion> {
  static void output(const PackedVersion &value, void *, raw_ostream &os) {
    os << value;
  }

  static StringRef input(StringRef scalar, void *, PackedVersion &value) {
    if (!value.parse32(scalar))
      return "invalid packed version string.";
    return {};
  }

  static bool mustQuote(StringRef) { return false; }
};

template <> struct ScalarTraits<SwiftVersion> {
  static void output(const SwiftVersion &value, void *, raw_ostream &os) {
    switch (value) {
    case 1:
      os << "1.0";
      break;
    case 2:
      os << "1.1";
      break;
    case 3:
      os << "2.0";
      break;
    case 4:
      os << "3.0";
      break;
    default:
      os << (unsigned)value;
      break;
    }
  }

  static StringRef input(StringRef scalar, void *, SwiftVersion &value) {
    value = StringSwitch<SwiftVersion>(scalar)
                .Case("1.0", 1)
                .Case("1.1", 2)
                .Case("2.0", 3)
                .Case("3.0", 4)
                .Default(0);
    if (value != SwiftVersion(0))
      return {};

    if (scalar.getAsInteger(10, value))
      return "invalid Swift ABI version.";

    return StringRef();
  }

  static bool mustQuote(StringRef) { return false; }
};

using TAPI_INTERNAL::AvailabilityInfo;
template <> struct ScalarTraits<AvailabilityInfo> {
  static void output(const AvailabilityInfo &value, void *, raw_ostream &os) {
    if (value._unavailable) {
      os << "n/a";
      return;
    }

    os << value._introduced;
    if (!value._obsoleted.empty())
      os << ".." << value._obsoleted;
  }

  static StringRef input(StringRef scalar, void *, AvailabilityInfo &value) {
    if (scalar == "n/a") {
      value._unavailable = true;
      return {};
    }

    auto split = scalar.split("..");
    auto introduced = split.first.trim();
    auto obsoleted = split.second.trim();

    if (!value._introduced.parse32(introduced))
      return "invalid packed version string.";

    if (obsoleted.empty())
      return StringRef();

    if (!value._obsoleted.parse32(obsoleted))
      return "invalid packed version string.";

    return StringRef();
  }

  static bool mustQuote(StringRef) { return false; }
};

template <> struct ScalarTraits<UUID> {
  static void output(const UUID &value, void *, raw_ostream &os) {
    os << value.first << ": " << value.second;
  }

  static StringRef input(StringRef scalar, void *, UUID &value) {
    auto split = scalar.split(':');
    auto arch = split.first.trim();
    auto uuid = split.second.trim();
    if (uuid.empty())
      return "invalid uuid string pair";
    value.first = getArchType(arch);
    value.second = uuid;
    return {};
  }

  static bool mustQuote(StringRef) { return true; }
};

} // end namespace yaml.
} // end namespace llvm.

#endif // TAPI_CORE_YAML_H
