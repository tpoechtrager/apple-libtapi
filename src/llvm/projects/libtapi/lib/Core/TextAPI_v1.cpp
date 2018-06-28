//===- tapi/Core/TextAPI_v1.cpp - Text API ----------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the Text API file reader/writer.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/TextAPI_v1.h"
#include "tapi/Core/ArchitectureSet.h"
#include "tapi/Core/ArchitectureSupport.h"
#include "tapi/Core/AvailabilityInfo.h"
#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/Framework.h"
#include "tapi/Core/XPI.h"
#include "tapi/Core/YAML.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/YAMLTraits.h"
#include <set>
#include <string>

using namespace llvm;
using namespace llvm::yaml;
using namespace TAPI_INTERNAL;

namespace {

struct Availability {
  std::vector<Platform> platform;
  std::string installName;
  PackedVersion currentVersion;
  PackedVersion compatibilityVersion;
  ArchitectureSet archs;
  AvailabilityInfo osRange;
};

/// \brief
struct ExportSection {
  std::vector<Availability> availabilities;
  std::vector<FlowStringRef> symbols;
  std::vector<FlowStringRef> classes;
  std::vector<FlowStringRef> classEHs;
  std::vector<FlowStringRef> ivars;
  std::vector<FlowStringRef> weakDefSymbols;
  std::vector<FlowStringRef> tlvSymbols;
};

} // namespace

LLVM_YAML_IS_FLOW_SEQUENCE_VECTOR(tapi::Platform)
LLVM_YAML_IS_SEQUENCE_VECTOR(Availability)
LLVM_YAML_IS_SEQUENCE_VECTOR(ExportSection)

namespace llvm {
namespace yaml {

template <> struct MappingTraits<Availability> {
  static void mapping(IO &io, Availability &avail) {
    io.mapRequired("install-name", avail.installName);
    io.mapOptional("current-version", avail.currentVersion,
                   PackedVersion(1, 0, 0));
    io.mapOptional("compatibility-version", avail.compatibilityVersion,
                   PackedVersion(1, 0, 0));
    io.mapRequired("arch", avail.archs);
    io.mapOptional("sdk-version", avail.osRange, AvailabilityInfo());
  }
};

template <> struct MappingTraits<ExportSection> {
  static void mapping(IO &io, ExportSection &section) {
    io.mapRequired("availability", section.availabilities);
    io.mapOptional("symbols", section.symbols);
    io.mapOptional("objc-classes", section.classes);
    io.mapOptional("objc-eh-types", section.classEHs);
    io.mapOptional("objc-ivars", section.ivars);
    io.mapOptional("weak-def-symbols", section.weakDefSymbols);
    io.mapOptional("thread-local-symbols", section.tlvSymbols);
  }
};

template <> struct MappingTraits<const ExtendedInterfaceFile *> {
  struct NormalizedAPI1 {
    explicit NormalizedAPI1(IO &io) {}
    NormalizedAPI1(IO &io, const ExtendedInterfaceFile *&file) {
      if (file->getFileType() == TAPI_INTERNAL::FileType::API_V1)
        isPrivate = false;
      else if (file->getFileType() == TAPI_INTERNAL::FileType::SPI_V1)
        isPrivate = true;
      else
        llvm_unreachable("Unexpected file type");

      std::set<std::pair<Architecture, AvailabilityInfo>> availabilitySet;
      for (const auto arch : file->getArchitectures()) {
        for (const auto *symbol : file->exports()) {
          if ((symbol->getAccess() != XPIAccess::Public) != isPrivate)
            continue;

          auto avail = symbol->getAvailabilityInfo(arch);
          if (!avail)
            continue;

          availabilitySet.insert(std::make_pair(arch, *avail));
        }
      }

      for (const auto &info : availabilitySet) {
        auto arch = info.first;
        auto &avail = info.second;
        ExportSection section;
        Availability availability;
        availability.installName = file->getInstallName();
        availability.currentVersion = file->getCurrentVersion();
        availability.compatibilityVersion = file->getCompatibilityVersion();
        availability.archs.set(arch);
        availability.osRange = avail;
        section.availabilities.push_back(std::move(availability));

        for (const auto *symbol : file->exports()) {
          if ((symbol->getAccess() != XPIAccess::Public) != isPrivate)
            continue;

          switch (symbol->getKind()) {
          case XPIKind::GlobalSymbol:
            if (symbol->isWeakDefined())
              section.weakDefSymbols.push_back(symbol->getName());
            else if (symbol->isThreadLocalValue())
              section.tlvSymbols.push_back(symbol->getName());
            else
              section.symbols.push_back(symbol->getName());
            break;
          case XPIKind::ObjectiveCClass:
            section.classes.push_back(symbol->getName());
            break;
          case XPIKind::ObjectiveCClassEHType:
            section.classEHs.push_back(symbol->getName());
            break;
          case XPIKind::ObjectiveCInstanceVariable:
            section.ivars.push_back(symbol->getName());
            break;
          default:
            llvm_unreachable("Unexpected symbol kind for exported symbols");
          }
        }

        sort(section.symbols);
        sort(section.classes);
        sort(section.classEHs);
        sort(section.ivars);
        sort(section.weakDefSymbols);
        sort(section.tlvSymbols);
        exports.emplace_back(std::move(section));
      }
    }

    const ExtendedInterfaceFile *denormalize(IO &io) {
      auto ctx = reinterpret_cast<YAMLContext *>(io.getContext());
      assert(ctx);

      auto *file = new ExtendedInterfaceFile;
      file->setPath(ctx->path);
      file->setFileType(TAPI_INTERNAL::FileType::API_V1);

      auto access = isPrivate ? XPIAccess::Private : XPIAccess::Public;
      for (const auto &section : exports) {
        ArchitectureSet archs;
        for (const auto &avail : section.availabilities)
          archs |= avail.archs;
        for (auto &symbolName : section.symbols)
          file->addSymbol(XPIKind::GlobalSymbol, symbolName, archs,
                          SymbolFlags::None, access);
        for (auto &symbolName : section.classes)
          file->addSymbol(XPIKind::ObjectiveCClass, symbolName, archs,
                          SymbolFlags::None, access);
        for (auto &symbolName : section.classEHs)
          file->addSymbol(XPIKind::ObjectiveCClassEHType, symbolName, archs,
                          SymbolFlags::None, access);
        for (auto &symbolName : section.ivars)
          file->addSymbol(XPIKind::ObjectiveCInstanceVariable, symbolName,
                          archs, SymbolFlags::None, access);
        for (auto &symbolName : section.weakDefSymbols)
          file->addSymbol(XPIKind::GlobalSymbol, symbolName, archs,
                          SymbolFlags::WeakDefined, access);
        for (auto &symbolName : section.tlvSymbols)
          file->addSymbol(XPIKind::GlobalSymbol, symbolName, archs,
                          SymbolFlags::ThreadLocalValue, access);
      }

      return file;
    }

    bool isPrivate;
    std::vector<ExportSection> exports;
  };

  static void mappingAPI1(IO &io, const ExtendedInterfaceFile *&file) {
    MappingNormalization<NormalizedAPI1, const ExtendedInterfaceFile *> keys(
        io, file);
    if (keys->isPrivate)
      io.mapTag("!tapi-spi-v1", true);
    else
      io.mapTag("!tapi-api-v1", true);
    io.mapOptional("exports", keys->exports);
  }
};

} // end namespace yaml.
} // end namespace llvm.

TAPI_NAMESPACE_INTERNAL_BEGIN

namespace api {
namespace v1 {

/// /brief Creates an unoptimized API/SPI file.
std::unique_ptr<ExtendedInterfaceFile>
createTextAPIFile(const Framework &framework,
                  const ExtendedInterfaceFile *interface) {
  std::unique_ptr<ExtendedInterfaceFile> file(new ExtendedInterfaceFile);
  file->setFileType(FileType::API_V1);
  file->setPlatform(interface->getPlatform());
  file->setArchitectures(interface->getArchitectures());
  file->setInstallName(interface->getInstallName());
  file->setCurrentVersion(interface->getCurrentVersion());
  file->setCompatibilityVersion(interface->getCompatibilityVersion());
  file->setTwoLevelNamespace(interface->isTwoLevelNamespace());
  file->setApplicationExtensionSafe(interface->isApplicationExtensionSafe());

  for (const auto *dsym : interface->exports()) {
    // Get the access value from the headers - if available.
    auto access = XPIAccess::Private;
    if (const auto *hsym = framework._headerSymbols->findSymbol(
            dsym->getKind(), dsym->getName()))
      access = hsym->getAccess();

    file->addSymbol(dsym->getKind(), dsym->getName(), dsym->getArchitectures(),
                    dsym->getSymbolFlags(), access);
  }

  return file;
}

bool YAMLDocumentHandler::canRead(MemoryBufferRef memBufferRef,
                                  FileType types) const {
  if (!(types & FileType::API_V1) && !(types & FileType::SPI_V1))
    return false;

  auto str = memBufferRef.getBuffer().trim();
  if (!(str.startswith("--- !tapi-api-v1\n") ||
        str.startswith("--- !tapi-spi-v1\n")) ||
      !str.endswith("..."))
    return false;

  return true;
}

FileType YAMLDocumentHandler::getFileType(MemoryBufferRef memBufferRef) const {
  if (!canRead(memBufferRef))
    return FileType::Invalid;

  auto str = memBufferRef.getBuffer().trim();
  if (str.startswith("--- !tapi-api-v1\n"))
    return FileType::API_V1;

  return FileType::SPI_V1;
}

bool YAMLDocumentHandler::canWrite(const File *file) const {
  auto *interface = dyn_cast<ExtendedInterfaceFile>(file);
  if (interface == nullptr)
    return false;

  if (interface->getFileType() != FileType::API_V1 &&
      interface->getFileType() != FileType::SPI_V1)
    return false;

  return true;
}

bool YAMLDocumentHandler::handleDocument(IO &io, const File *&file) const {
  if (io.outputting() && (file->getFileType() != FileType::API_V1 &&
                          file->getFileType() != FileType::SPI_V1))
    return false;

  if (!io.outputting() &&
      !(io.mapTag("!tapi-api-v1") || io.mapTag("!tapi-spi-v1")))
    return false;

  const auto *interface = dyn_cast_or_null<ExtendedInterfaceFile>(file);
  MappingTraits<const ExtendedInterfaceFile *>::mappingAPI1(io, interface);
  file = interface;

  return true;
}

} // end namespace v1.
} // end namespace api.

TAPI_NAMESPACE_INTERNAL_END
