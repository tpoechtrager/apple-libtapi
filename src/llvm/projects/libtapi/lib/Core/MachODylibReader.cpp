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

#if 0

#include "tapi/Core/MachODylibReader.h"
#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Core/XPI.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/BinaryFormat/Magic.h"
#include "llvm/ObjCMetadata/ObjCMachOBinary.h"
#include "llvm/Object/MachO.h"
#include "llvm/Object/MachOUniversal.h"
#include <tuple>

using namespace llvm;
using namespace llvm::object;

TAPI_NAMESPACE_INTERNAL_BEGIN

Expected<FileType>
MachODylibReader::getFileType(file_magic magic,
                              MemoryBufferRef bufferRef) const {
  switch (magic) {
  default:
    return FileType::Invalid;
  case file_magic::macho_bundle: // Assume dylib for now.
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
    case MachO::MH_BUNDLE: // Assume dylib for now.
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
      !(types & FileType::MachO_DynamicLibrary_Stub))
    return false;
  auto fileType = getFileType(magic, bufferRef);
  if (!fileType) {
    consumeError(fileType.takeError());
    return false;
  }
  return fileType.get() != FileType::Invalid;
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
  auto fileType = H.filetype == MachO::MH_DYLIB
                      ? FileType::MachO_DynamicLibrary
                      : FileType::MachO_DynamicLibrary_Stub;
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
      file->setPlatform(Platform::OSX);
      break;
    case MachO::LC_VERSION_MIN_IPHONEOS:
      file->setPlatform(Platform::iOS);
      break;
    case MachO::LC_VERSION_MIN_WATCHOS:
      file->setPlatform(Platform::watchOS);
      break;
    case MachO::LC_VERSION_MIN_TVOS:
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
        file->setPlatform(Platform::OSX);
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

static Error readObjectiveCMetadata(MachOObjectFile *object,
                                    ExtendedInterfaceFile *file) {
  auto H = object->getHeader();
  auto arch = getArchType(H.cputype, H.cpusubtype);

  auto error = Error::success();
  MachOMetadata metadata(object, error);
  if (error)
    return std::move(error);

  ///
  /// Classes
  ///
  auto classes = metadata.classes();
  if (!classes)
    return classes.takeError();

  for (const auto &classRef : *classes) {
    auto objcClassMeta = classRef.getObjCClass();
    if (!objcClassMeta)
      return objcClassMeta.takeError();

    auto superClassName = objcClassMeta->getSuperClassName();
    if (!superClassName)
      return superClassName.takeError();

    auto className = objcClassMeta->getName();
    if (!className)
      return className.takeError();

    ObjCClass *objcSuperClass = nullptr;
    if (!superClassName->empty())
      objcSuperClass =
          file->addObjCClass(superClassName->str(), arch, XPIAccess::Unknown);

    auto *objcClass = file->addObjCClass(className->str(), arch,
                                         XPIAccess::Unknown, objcSuperClass);

    auto access = objcClass->getAccess();
    auto properties = objcClassMeta->properties();
    if (!properties)
      return properties.takeError();

    for (const auto &property : *properties) {
      auto name = property.getName();
      if (!name)
        return name.takeError();

      auto isDynamic = property.isDynamic();
      if (!isDynamic)
        return isDynamic.takeError();

      auto setter = property.getSetter();
      if (!setter)
        return setter.takeError();
      if (!setter->empty())
        file->addObjCSelector(objcClass, *setter, arch,
                              /*isInstanceMethod=*/true, *isDynamic, access);

      auto getter = property.getGetter();
      if (!getter)
        return getter.takeError();
      file->addObjCSelector(objcClass, *getter, arch, /*isInstanceMethod=*/true,
                            *isDynamic, access);
    }

    auto classMethods = objcClassMeta->classMethods();
    if (!classMethods)
      return classMethods.takeError();

    for (const auto &method : *classMethods) {
      auto name = method.getName();
      if (!name)
        return name.takeError();

      file->addObjCSelector(objcClass, *name, arch, /*isInstanceMethod=*/false,
                            /*isDynamic=*/false, access);
    }

    auto instanceMethods = objcClassMeta->instanceMethods();
    if (!instanceMethods)
      return instanceMethods.takeError();

    for (const auto &method : *instanceMethods) {
      auto name = method.getName();
      if (!name)
        return name.takeError();

      file->addObjCSelector(objcClass, *name, arch, /*isInstanceMethod=*/true,
                            /*isDynamic=*/false, access);
    }
  }

  ///
  /// Categories
  ///
  auto categories = metadata.categories();
  if (!categories)
    return categories.takeError();

  for (const auto &categoryRef : *categories) {
    auto category = categoryRef.getObjCCategory();
    if (!category)
      return category.takeError();

    auto categoryName = category->getName();
    if (!categoryName)
      return categoryName.takeError();

    auto baseClassName = category->getBaseClassName();
    if (!baseClassName)
      return baseClassName.takeError();

    auto *objcBaseClass =
        file->addObjCClass(baseClassName->str(), arch, XPIAccess::Unknown);
    auto access = objcBaseClass->getAccess();
    auto *objcCategory =
        file->addObjCCategory(objcBaseClass, *categoryName, arch, access);

    auto properties = category->properties();
    if (!properties)
      return properties.takeError();

    for (const auto &property : *properties) {
      auto name = property.getName();
      if (!name)
        return name.takeError();

      auto isDynamic = property.isDynamic();
      if (!isDynamic)
        return isDynamic.takeError();

      auto setter = property.getSetter();
      if (!setter)
        return setter.takeError();
      if (!setter->empty())
        file->addObjCSelector(objcCategory, *setter, arch,
                              /*isInstanceMethod=*/true, *isDynamic, access);

      auto getter = property.getGetter();
      if (!getter)
        return getter.takeError();
      file->addObjCSelector(objcCategory, *getter, arch,
                            /*isInstanceMethod=*/true, *isDynamic, access);
    }

    auto classMethods = category->classMethods();
    if (!classMethods)
      return classMethods.takeError();

    for (const auto &method : *classMethods) {
      auto name = method.getName();
      if (!name)
        return name.takeError();

      file->addObjCSelector(objcCategory, *name, arch,
                            /*isInstanceMethod=*/false, /*isDynamic=*/false,
                            access);
    }

    auto instanceMethods = category->instanceMethods();
    if (!instanceMethods)
      return instanceMethods.takeError();

    for (const auto &method : *instanceMethods) {
      auto name = method.getName();
      if (!name)
        return name.takeError();

      file->addObjCSelector(objcCategory, *name, arch,
                            /*isInstanceMethod=*/true, /*isDynamic=*/false,
                            access);
    }
  }

  ///
  /// Protocols
  ///
  auto protocols = metadata.protocols();
  if (!protocols)
    return protocols.takeError();

  for (const auto &protocolRef : *protocols) {
    auto protocol = protocolRef.getObjCProtocol();
    if (!protocol)
      return protocol.takeError();

    auto protocolName = protocol->getName();
    if (!protocolName)
      return protocolName.takeError();

    auto *objcProtocol = file->addObjCProtocol(*protocolName, arch);

    auto properties = protocol->properties();
    if (!properties)
      return properties.takeError();

    for (const auto &property : *properties) {
      auto name = property.getName();
      if (!name)
        return name.takeError();

      auto isDynamic = property.isDynamic();
      if (!isDynamic)
        return isDynamic.takeError();

      auto setter = property.getSetter();
      if (!setter)
        return setter.takeError();
      if (!setter->empty())
        file->addObjCSelector(objcProtocol, *setter, arch,
                              /*isInstanceMethod=*/true, *isDynamic);

      auto getter = property.getGetter();
      if (!getter)
        return getter.takeError();
      file->addObjCSelector(objcProtocol, *getter, arch,
                            /*isInstanceMethod=*/true, *isDynamic);
    }

    auto classMethods = protocol->classMethods();
    if (!classMethods)
      return classMethods.takeError();

    for (const auto &method : *classMethods) {
      auto name = method.getName();
      if (!name)
        return name.takeError();

      file->addObjCSelector(objcProtocol, *name, arch,
                            /*isInstanceMethod=*/false, /*isDynamic=*/false);
    }

    classMethods = protocol->optionalClassMethods();
    if (!classMethods)
      return classMethods.takeError();

    for (const auto &method : *classMethods) {
      auto name = method.getName();
      if (!name)
        return name.takeError();

      file->addObjCSelector(objcProtocol, *name, arch,
                            /*isInstanceMethod=*/false, /*isDynamic=*/false);
    }

    auto instanceMethods = protocol->instanceMethods();
    if (!instanceMethods)
      return instanceMethods.takeError();

    for (const auto &method : *instanceMethods) {
      auto name = method.getName();
      if (!name)
        return name.takeError();

      file->addObjCSelector(objcProtocol, *name, arch,
                            /*isInstanceMethod=*/true, /*isDynamic=*/false);
    }

    instanceMethods = protocol->optionalInstanceMethods();
    if (!instanceMethods)
      return instanceMethods.takeError();

    for (const auto &method : *instanceMethods) {
      auto name = method.getName();
      if (!name)
        return name.takeError();

      file->addObjCSelector(objcProtocol, *name, arch,
                            /*isInstanceMethod=*/true, /*isDynamic=*/false);
    }
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

  if (readFlags >= ReadFlags::ObjCMetadata) {
    auto error = readObjectiveCMetadata(object, file);
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

#endif
