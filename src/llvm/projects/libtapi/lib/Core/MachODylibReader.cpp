//===- lib/Core/MachODylibReader.cpp - TAPI MachO Dylib Reader --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the object specific parts of reading the dylib files.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/MachODylibReader.h"
#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Core/XPI.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/BinaryFormat/Magic.h"
#include "llvm/Object/MachO.h"
#include "llvm/Object/MachOUniversal.h"
#include <tuple>

using namespace llvm;
using namespace llvm::object;

// Define missing platform enums.
namespace llvm {
namespace MachO {
// clang-format off
  enum MissingPlatformType {
    PLATFORM_IOSSIMULATOR     = 7,
    PLATFORM_TVOSSIMULATOR    = 8,
    PLATFORM_WATCHOSSIMULATOR = 9,
  };
// clang-format on
} // end namespace MachO.
} // end namespace llvm.

TAPI_NAMESPACE_INTERNAL_BEGIN

Expected<FileType>
MachODylibReader::getFileType(file_magic magic,
                              MemoryBufferRef bufferRef) const {
  switch (magic) {
  default:
    return FileType::Invalid;
  case file_magic::macho_bundle:
    return FileType::MachO_Bundle;
  case file_magic::macho_dynamically_linked_shared_lib:
    return FileType::MachO_DynamicLibrary;
  case file_magic::macho_dynamically_linked_shared_lib_stub:
    return FileType::MachO_DynamicLibrary_Stub;
  case file_magic::macho_universal_binary:
    break;
  }

  auto binaryOrErr = createBinary(bufferRef);
  if (!binaryOrErr)
    return binaryOrErr.takeError();

  Binary &bin = *binaryOrErr.get();
  assert(isa<MachOUniversalBinary>(&bin) && "Unexpected MachO binary");
  auto *UB = cast<MachOUniversalBinary>(&bin);

  FileType fileType = FileType::Invalid;
  // Check if any of the architecture slices are a MachO dylib.
  for (auto OI = UB->begin_objects(), OE = UB->end_objects(); OI != OE; ++OI) {
    // This can fail if the object is an archive.
    auto objOrErr = OI->getAsObjectFile();
    // Skip the archive and comsume the error.
    if (!objOrErr) {
      consumeError(objOrErr.takeError());
      continue;
    }

    auto &obj = *objOrErr.get();
    switch (obj.getHeader().filetype) {
    default:
      continue;
    case MachO::MH_BUNDLE:
      if (fileType == FileType::Invalid)
        fileType = FileType::MachO_Bundle;
      else if (fileType != FileType::MachO_Bundle)
        return FileType::Invalid;
      break;
    case MachO::MH_DYLIB:
      if (fileType == FileType::Invalid)
        fileType = FileType::MachO_DynamicLibrary;
      else if (fileType != FileType::MachO_DynamicLibrary)
        return FileType::Invalid;
      break;
    case MachO::MH_DYLIB_STUB:
      if (fileType == FileType::Invalid)
        fileType = FileType::MachO_DynamicLibrary_Stub;
      else if (fileType != FileType::MachO_DynamicLibrary_Stub)
        return FileType::Invalid;
      break;
    }
  }

  return fileType;
}

bool MachODylibReader::canRead(file_magic magic, MemoryBufferRef bufferRef,
                               FileType types) const {
  if (!(types & FileType::MachO_DynamicLibrary) &&
      !(types & FileType::MachO_DynamicLibrary_Stub) &&
      !(types & FileType::MachO_Bundle))
    return false;

  auto fileType = getFileType(magic, bufferRef);
  if (!fileType) {
    consumeError(fileType.takeError());
    return false;
  }

  return types & fileType.get();
}

static std::tuple<StringRef, XPIKind> parseSymbol(StringRef symbolName) {
  StringRef name;
  XPIKind kind;
  if (symbolName.startswith(".objc_class_name_")) {
    name = symbolName.drop_front(17);
    kind = XPIKind::ObjectiveCClass;
  } else if (symbolName.startswith("_OBJC_CLASS_$_")) {
    name = symbolName.drop_front(14);
    kind = XPIKind::ObjectiveCClass;
  } else if (symbolName.startswith("_OBJC_METACLASS_$_")) {
    name = symbolName.drop_front(18);
    kind = XPIKind::ObjectiveCClass;
  } else if (symbolName.startswith("_OBJC_EHTYPE_$_")) {
    name = symbolName.drop_front(15);
    kind = XPIKind::ObjectiveCClassEHType;
  } else if (symbolName.startswith("_OBJC_IVAR_$_")) {
    name = symbolName.drop_front(13);
    kind = XPIKind::ObjectiveCInstanceVariable;
  } else {
    name = symbolName;
    kind = XPIKind::GlobalSymbol;
  }
  return std::make_tuple(name, kind);
}

static Error readMachOHeaderData(MachOObjectFile *object,
                                 ExtendedInterfaceFile *file) {
  auto H = object->getHeader();
  auto arch = getArchType(H.cputype, H.cpusubtype);
  if (arch == Architecture::unknown)
    return make_error<StringError>(
        "unknown/unsupported architecture",
        std::make_error_code(std::errc::not_supported));
  file->setArch(arch);
  FileType fileType = FileType::Invalid;
  switch (H.filetype) {
  default:
    llvm_unreachable("unsupported binary type");
  case MachO::MH_DYLIB:
    fileType = FileType::MachO_DynamicLibrary;
    break;
  case MachO::MH_DYLIB_STUB:
    fileType = FileType::MachO_DynamicLibrary_Stub;
    break;
  case MachO::MH_BUNDLE:
    fileType = FileType::MachO_Bundle;
    break;
  }

  file->setFileType(fileType);

  if (H.flags & MachO::MH_TWOLEVEL)
    file->setTwoLevelNamespace();

  if (H.flags & MachO::MH_APP_EXTENSION_SAFE)
    file->setApplicationExtensionSafe();

  for (const auto &LCI : object->load_commands()) {
    switch (LCI.C.cmd) {
    case MachO::LC_ID_DYLIB: {
      auto DLLC = object->getDylibIDLoadCommand(LCI);
      file->setInstallName(LCI.Ptr + DLLC.dylib.name);
      file->setCurrentVersion(DLLC.dylib.current_version);
      file->setCompatibilityVersion(DLLC.dylib.compatibility_version);
      break;
    }
    case MachO::LC_REEXPORT_DYLIB: {
      auto DLLC = object->getDylibIDLoadCommand(LCI);
      file->addReexportedLibrary(LCI.Ptr + DLLC.dylib.name, arch);
      break;
    }
    case MachO::LC_SUB_FRAMEWORK: {
      auto SFC = object->getSubFrameworkCommand(LCI);
      file->setParentUmbrella(LCI.Ptr + SFC.umbrella);
      break;
    }
    case MachO::LC_SUB_CLIENT: {
      auto SCLC = object->getSubClientCommand(LCI);
      file->addAllowableClient(LCI.Ptr + SCLC.client, arch);
      break;
    }
    case MachO::LC_UUID: {
      auto UUIDLC = object->getUuidCommand(LCI);
      file->addUUID(UUIDLC.uuid, arch);
      break;
    }
    case MachO::LC_VERSION_MIN_MACOSX:
      file->setPlatform(Platform::macOS);
      break;
    case MachO::LC_VERSION_MIN_IPHONEOS:
      if (arch == Architecture::i386 || arch == Architecture::x86_64)
        file->setPlatform(Platform::iOSSimulator);
      else
        file->setPlatform(Platform::iOS);
      break;
    case MachO::LC_VERSION_MIN_WATCHOS:
      if (arch == Architecture::i386 || arch == Architecture::x86_64)
        file->setPlatform(Platform::watchOSSimulator);
      else
        file->setPlatform(Platform::watchOS);
      break;
    case MachO::LC_VERSION_MIN_TVOS:
      if (arch == Architecture::i386 || arch == Architecture::x86_64)
        file->setPlatform(Platform::tvOSSimulator);
      else
        file->setPlatform(Platform::tvOS);
      break;
    case MachO::LC_BUILD_VERSION: {
      auto BVC = object->getBuildVersionLoadCommand(LCI);
      switch (BVC.platform) {
      default:
        return make_error<StringError>(
            "unknown/unsupported platform",
            std::make_error_code(std::errc::not_supported));
      case MachO::PLATFORM_MACOS:
        file->setPlatform(Platform::macOS);
        break;
      case MachO::PLATFORM_IOS:
        file->setPlatform(Platform::iOS);
        break;
      case MachO::PLATFORM_TVOS:
        file->setPlatform(Platform::tvOS);
        break;
      case MachO::PLATFORM_WATCHOS:
        file->setPlatform(Platform::watchOS);
        break;
      case MachO::PLATFORM_BRIDGEOS:
        file->setPlatform(Platform::bridgeOS);
        break;
      case MachO::PLATFORM_IOSSIMULATOR:
        file->setPlatform(Platform::iOSSimulator);
        break;
      case MachO::PLATFORM_TVOSSIMULATOR:
        file->setPlatform(Platform::tvOSSimulator);
        break;
      case MachO::PLATFORM_WATCHOSSIMULATOR:
        file->setPlatform(Platform::watchOSSimulator);
        break;
      }
      break;
    }
    default:
      break;
    }
  }

  for (auto &section : object->sections()) {
    StringRef sectionName;
    section.getName(sectionName);
    if (sectionName != "__objc_imageinfo" && sectionName != "__image_info")
      continue;
    StringRef content;
    section.getContents(content);
    if ((content.size() >= 8) && (content[0] == 0)) {
      uint32_t flags;
      if (object->isLittleEndian()) {
        auto *p =
            reinterpret_cast<const support::ulittle32_t *>(content.data() + 4);
        flags = *p;
      } else {
        auto *p =
            reinterpret_cast<const support::ubig32_t *>(content.data() + 4);
        flags = *p;
      }
      if ((flags & 4) == 4)
        file->setObjCConstraint(ObjCConstraint::GC);
      else if ((flags & 2) == 2)
        file->setObjCConstraint(ObjCConstraint::Retain_Release_Or_GC);
      else if ((flags & 32) == 32)
        file->setObjCConstraint(ObjCConstraint::Retain_Release_For_Simulator);
      else
        file->setObjCConstraint(ObjCConstraint::Retain_Release);

      file->setSwiftABIVersion(((flags >> 8) & 0xFF));
    }
  }

  return Error::success();
}

static Error readExportedSymbols(MachOObjectFile *object,
                                 ExtendedInterfaceFile *file) {
  auto H = object->getHeader();
  auto arch = getArchType(H.cputype, H.cpusubtype);
  assert(arch != Architecture::unknown && "unknown architecture slice");

  Error error = Error::success();
  for (const auto &symbol : object->exports(error)) {
    StringRef name;
    XPIKind kind;
    std::tie(name, kind) = parseSymbol(symbol.name());
    SymbolFlags flags = SymbolFlags::None;
    switch (symbol.flags() & MachO::EXPORT_SYMBOL_FLAGS_KIND_MASK) {
    case MachO::EXPORT_SYMBOL_FLAGS_KIND_REGULAR:
      if (symbol.flags() & MachO::EXPORT_SYMBOL_FLAGS_WEAK_DEFINITION)
        flags = SymbolFlags::WeakDefined;
      break;
    case MachO::EXPORT_SYMBOL_FLAGS_KIND_THREAD_LOCAL:
      flags = SymbolFlags::ThreadLocalValue;
      break;
    }
    file->addSymbol(kind, name, arch, flags);
  }

  return error;
}

static Error readUndefinedSymbols(MachOObjectFile *object,
                                  ExtendedInterfaceFile *file) {
  auto H = object->getHeader();
  auto arch = getArchType(H.cputype, H.cpusubtype);
  assert(arch != Architecture::unknown && "unknown architecture slice");

  for (const auto &symbol : object->symbols()) {
    auto symbolFlags = symbol.getFlags();
    if ((symbolFlags & BasicSymbolRef::SF_Global) == 0)
      continue;
    if ((symbolFlags & BasicSymbolRef::SF_Undefined) == 0)
      continue;

    auto flags = symbolFlags & BasicSymbolRef::SF_Weak
                     ? SymbolFlags::WeakReferenced
                     : SymbolFlags::None;
    auto symbolName = symbol.getName();
    if (!symbolName)
      return symbolName.takeError();

    StringRef name;
    XPIKind kind;
    std::tie(name, kind) = parseSymbol(symbolName.get());
    file->addUndefinedSymbol(kind, name, arch, flags);
  }

  return Error::success();
}

static Error load(MachOObjectFile *object, ExtendedInterfaceFile *file,
                  ReadFlags readFlags) {
  if (readFlags >= ReadFlags::Header) {
    auto error = readMachOHeaderData(object, file);
    if (error)
      return error;
  }

  if (readFlags >= ReadFlags::Symbols) {
    auto error = readExportedSymbols(object, file);
    if (error)
      return error;
  }

  // Only record undef symbols for flat namespace dylibs.
  if (file->isTwoLevelNamespace())
    return Error::success();

  if (readFlags >= ReadFlags::Symbols)
    return readUndefinedSymbols(object, file);

  return Error::success();
}

Expected<std::unique_ptr<File>>
MachODylibReader::readFile(std::unique_ptr<MemoryBuffer> memBuffer,
                           ReadFlags readFlags, ArchitectureSet arches) const {
  auto file = std::unique_ptr<ExtendedInterfaceFile>(new ExtendedInterfaceFile);
  file->setPath(memBuffer->getBufferIdentifier());
  file->setMemoryBuffer(std::move(memBuffer));

  auto binaryOrErr = createBinary(file->getMemBufferRef());
  if (!binaryOrErr)
    return binaryOrErr.takeError();

  Binary &binary = *binaryOrErr.get();
  if (auto *object = dyn_cast<MachOObjectFile>(&binary)) {
    auto arch = getArchType(object->getHeader().cputype,
                            object->getHeader().cpusubtype);
    if (!arches.has(arch))
      return make_error<StringError>(
          "Requested architectures don't exist",
          std::make_error_code(std::errc::not_supported));

    auto error = load(object, file.get(), readFlags);
    if (error)
      return std::move(error);
    return std::move(file);
  }

  // Only expecting MachO universal binaries at this point.
  assert(isa<MachOUniversalBinary>(&binary) &&
         "Expected a MachO universal binary.");
  auto *UB = cast<MachOUniversalBinary>(&binary);

  bool foundArch = false;
  for (auto OI = UB->begin_objects(), OE = UB->end_objects(); OI != OE; ++OI) {
    // Skip the architecture that is not requested.
    auto arch = getArchType(OI->getCPUType(), OI->getCPUSubType());
    if (!arches.has(arch))
      continue;

    // Skip unknown architectures.
    if (arch == Architecture::unknown)
      continue;

    foundArch = true;
    // This can fail if the object is an archive.
    auto objOrErr = OI->getAsObjectFile();

    // Skip the archive and comsume the error.
    if (!objOrErr) {
      consumeError(objOrErr.takeError());
      continue;
    }

    auto &object = *objOrErr.get();
    switch (object.getHeader().filetype) {
    default:
      break;
    case MachO::MH_BUNDLE:
    case MachO::MH_DYLIB:
    case MachO::MH_DYLIB_STUB:
      auto error = load(&object, file.get(), readFlags);
      if (error)
        return std::move(error);
      break;
    }
  }

  if (!foundArch)
    return make_error<StringError>(
        "Requested architectures don't exist",
        std::make_error_code(std::errc::not_supported));

  return std::move(file);
}

TAPI_NAMESPACE_INTERNAL_END
