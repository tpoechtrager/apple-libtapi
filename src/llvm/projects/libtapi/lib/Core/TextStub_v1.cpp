//===- tapi/Core/TextStub_v1.cpp - Text Stub v1 -----------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the text stub file (TBD v1) reader/writer.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/TextStub_v1.h"
#include "tapi/Core/ArchitectureSupport.h"
#include "tapi/Core/InterfaceFile.h"
#include "tapi/Core/Registry.h"
#include "tapi/Core/YAML.h"
#include "tapi/Core/YAMLReaderWriter.h"
#include "tapi/LinkerInterfaceFile.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/YAMLTraits.h"
#include <set>

using namespace llvm;
using namespace llvm::yaml;
using namespace TAPI_INTERNAL;

namespace {

struct ExportSection {
  std::vector<Architecture> archs;
  std::vector<FlowStringRef> allowableClients;
  std::vector<FlowStringRef> reexportedLibraries;
  std::vector<FlowStringRef> symbols;
  std::vector<FlowStringRef> classes;
  std::vector<FlowStringRef> ivars;
  std::vector<FlowStringRef> weakDefSymbols;
  std::vector<FlowStringRef> tlvSymbols;
};

} // end anonymous namespace.

LLVM_YAML_IS_FLOW_SEQUENCE_VECTOR(Architecture)
LLVM_YAML_IS_SEQUENCE_VECTOR(ExportSection)

namespace llvm {
namespace yaml {

template <> struct MappingTraits<ExportSection> {
  static void mapping(IO &io, ExportSection &section) {
    io.mapRequired("archs", section.archs);
    io.mapOptional("allowed-clients", section.allowableClients);
    io.mapOptional("re-exports", section.reexportedLibraries);
    io.mapOptional("symbols", section.symbols);
    io.mapOptional("objc-classes", section.classes);
    io.mapOptional("objc-ivars", section.ivars);
    io.mapOptional("weak-def-symbols", section.weakDefSymbols);
    io.mapOptional("thread-local-symbols", section.tlvSymbols);
  }
};

template <> struct MappingTraits<const InterfaceFile *> {
  struct NormalizedTBD1 {
    explicit NormalizedTBD1(IO &io) {}
    NormalizedTBD1(IO &io, const InterfaceFile *&file) {
      archs = file->getArchitectures();
      platform = file->getPlatform();
      installName = file->getInstallName();
      currentVersion = file->getCurrentVersion();
      compatibilityVersion = file->getCompatibilityVersion();
      swiftVersion = file->getSwiftABIVersion();
      objcConstraint = file->getObjCConstraint();

      std::set<ArchitectureSet> archSet;
      for (const auto &library : file->allowableClients())
        archSet.insert(library.getArchitectures());

      for (const auto &library : file->reexportedLibraries())
        archSet.insert(library.getArchitectures());

      std::map<const Symbol *, ArchitectureSet> symbolToArchSet;
      for (const auto *symbol : file->exports()) {
        auto archs = symbol->getArchitectures();
        symbolToArchSet[symbol] = archs;
        archSet.insert(archs);
      }

      for (auto archs : archSet) {
        ExportSection section;
        section.archs = archs;

        for (const auto &library : file->allowableClients())
          if (library.getArchitectures() == archs)
            section.allowableClients.emplace_back(library.getInstallName());

        for (const auto &library : file->reexportedLibraries())
          if (library.getArchitectures() == archs)
            section.reexportedLibraries.emplace_back(library.getInstallName());

        for (const auto &symArch : symbolToArchSet) {
          if (symArch.second != archs)
            continue;

          const auto *symbol = symArch.first;
          switch (symbol->getKind()) {
          case SymbolKind::GlobalSymbol:
            if (symbol->isWeakDefined())
              section.weakDefSymbols.emplace_back(symbol->getName());
            else if (symbol->isThreadLocalValue())
              section.tlvSymbols.emplace_back(symbol->getName());
            else
              section.symbols.emplace_back(symbol->getName());
            break;
          case SymbolKind::ObjectiveCClass:
            section.classes.emplace_back(
                copyString("_" + symbol->getName().str()));
            break;
          case SymbolKind::ObjectiveCClassEHType:
            section.symbols.emplace_back(
                copyString("_OBJC_EHTYPE_$_" + symbol->getName().str()));
            break;
          case SymbolKind::ObjectiveCInstanceVariable:
            section.ivars.emplace_back(
                copyString("_" + symbol->getName().str()));
            break;
          }
        }
        TAPI_INTERNAL::sort(section.symbols);
        TAPI_INTERNAL::sort(section.classes);
        TAPI_INTERNAL::sort(section.ivars);
        TAPI_INTERNAL::sort(section.weakDefSymbols);
        TAPI_INTERNAL::sort(section.tlvSymbols);
        exports.emplace_back(std::move(section));
      }
    }

    const InterfaceFile *denormalize(IO &io) {
      auto ctx = reinterpret_cast<YAMLContext *>(io.getContext());
      assert(ctx);

      auto *file = new InterfaceFile;
      file->setPath(ctx->path);
      file->setFileType(TAPI_INTERNAL::FileType::TBD_V1);
      file->setArchitectures(archs);
      file->setPlatform(mapToSim(platform, file->getArchitectures().hasX86()));
      file->setInstallName(installName);
      file->setCurrentVersion(currentVersion);
      file->setCompatibilityVersion(compatibilityVersion);
      file->setSwiftABIVersion(swiftVersion);
      file->setTwoLevelNamespace();
      file->setApplicationExtensionSafe();
      file->setObjCConstraint(objcConstraint);

      for (const auto &section : exports) {
        for (const auto &client : section.allowableClients)
          file->addAllowableClient(client, section.archs);
        for (const auto &lib : section.reexportedLibraries)
          file->addReexportedLibrary(lib, section.archs);

        // Skip symbols if requested.
        if (ctx->readFlags < ReadFlags::Symbols)
          continue;

        for (auto &sym : section.symbols) {
          if (sym.value.startswith("_OBJC_EHTYPE_$_"))
            file->addSymbolImpl(SymbolKind::ObjectiveCClassEHType,
                                sym.value.drop_front(15), section.archs,
                                SymbolFlags::None, /*copyStrings=*/false);
          else
            file->addSymbolImpl(SymbolKind::GlobalSymbol, sym, section.archs,
                                SymbolFlags::None,
                                /*copyStrings=*/false);
        }
        for (auto &sym : section.classes)
          file->addSymbolImpl(SymbolKind::ObjectiveCClass,
                              sym.value.drop_front(), section.archs,
                              SymbolFlags::None, /*copyStrings=*/false);
        for (auto &sym : section.ivars)
          file->addSymbolImpl(SymbolKind::ObjectiveCInstanceVariable,
                              sym.value.drop_front(), section.archs,
                              SymbolFlags::None, /*copyStrings=*/false);
        for (auto &sym : section.weakDefSymbols)
          file->addSymbolImpl(SymbolKind::GlobalSymbol, sym, section.archs,
                              SymbolFlags::WeakDefined,
                              /*copyStrings=*/false);
        for (auto &sym : section.tlvSymbols)
          file->addSymbolImpl(SymbolKind::GlobalSymbol, sym, section.archs,
                              SymbolFlags::ThreadLocalValue,
                              /*copyStrings=*/false);
      }

      return file;
    }

    std::vector<Architecture> archs;
    Platform platform;
    StringRef installName;
    PackedVersion currentVersion;
    PackedVersion compatibilityVersion;
    SwiftVersion swiftVersion;
    ObjCConstraint objcConstraint;
    std::vector<ExportSection> exports;

    llvm::BumpPtrAllocator allocator;
    StringRef copyString(StringRef string) {
      if (string.empty())
        return {};

      void *ptr = allocator.Allocate(string.size(), 1);
      memcpy(ptr, string.data(), string.size());
      return {reinterpret_cast<const char *>(ptr), string.size()};
    }
  };

  static void mappingTBD1(IO &io, const InterfaceFile *&file) {
    MappingNormalization<NormalizedTBD1, const InterfaceFile *> keys(io, file);
    // Don't write the tag into the .tbd file for TBD v1.
    if (!io.outputting())
      io.mapTag("tapi-tbd-v1", true);

    io.mapRequired("archs", keys->archs);
    io.mapRequired("platform", keys->platform);
    io.mapRequired("install-name", keys->installName);
    io.mapOptional("current-version", keys->currentVersion,
                   PackedVersion(1, 0, 0));
    io.mapOptional("compatibility-version", keys->compatibilityVersion,
                   PackedVersion(1, 0, 0));
    io.mapOptional("swift-version", keys->swiftVersion, SwiftVersion(0));
    io.mapOptional("objc-constraint", keys->objcConstraint,
                   ObjCConstraint::None);
    io.mapOptional("exports", keys->exports);
  }
};

} // end namespace yaml.
} // end namespace llvm.

TAPI_NAMESPACE_INTERNAL_BEGIN

namespace stub {
namespace v1 {

bool YAMLDocumentHandler::canRead(MemoryBufferRef memBufferRef,
                                  FileType types) const {
  if (!(types & FileType::TBD_V1))
    return false;

  auto str = memBufferRef.getBuffer().trim();
  if (!(str.startswith("---\n") || str.startswith("--- !tapi-tbd-v1\n")) ||
      !str.endswith("..."))
    return false;

  return true;
}

FileType YAMLDocumentHandler::getFileType(MemoryBufferRef memBufferRef) const {
  if (canRead(memBufferRef))
    return FileType::TBD_V1;

  return FileType::Invalid;
}

bool YAMLDocumentHandler::canWrite(const File *file) const {
  auto *interface = dyn_cast<InterfaceFile>(file);
  if (interface == nullptr)
    return false;

  if (interface->getFileType() != FileType::TBD_V1)
    return false;

  return true;
}

bool YAMLDocumentHandler::handleDocument(IO &io, const File *&file) const {
  if (io.outputting() && file->getFileType() != FileType::TBD_V1)
    return false;

  if (!io.outputting() && !io.mapTag("!tapi-tbd-v1") &&
      !io.mapTag("tag:yaml.org,2002:map"))
    return false;

  auto *ctx = reinterpret_cast<YAMLContext *>(io.getContext());
  ctx->fileType = FileType::TBD_V1;

  const auto *interface = dyn_cast_or_null<InterfaceFile>(file);
  MappingTraits<const InterfaceFile *>::mappingTBD1(io, interface);
  file = interface;

  return true;
}

} // end namespace v1.
} // end namespace stub.

TAPI_NAMESPACE_INTERNAL_END
