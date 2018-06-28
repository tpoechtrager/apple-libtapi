//===- ExtendedInterfaceFile.cpp - Extended Interface File ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the Extended Interface File
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/Path.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

TAPI_NAMESPACE_INTERNAL_BEGIN

void ExtendedInterfaceFile::addSymbol(XPIKind kind, StringRef name,
                                      ArchitectureSet archs, SymbolFlags flags,
                                      XPIAccess access) {
  switch (kind) {
  default:
    llvm_unreachable("invalid XPI kind");
  case XPIKind::GlobalSymbol:
    _symbols->addGlobalSymbol(name, archs, flags, access);
    break;
  case XPIKind::ObjectiveCClass:
    _symbols->addObjCClass(name, archs, access);
    break;
  case XPIKind::ObjectiveCClassEHType:
    _symbols->addObjCClassEHType(name, archs, access);
    break;
  case XPIKind::ObjectiveCInstanceVariable:
    _symbols->addObjCInstanceVariable(name, archs, access);
    break;
  }
}

ObjCClass *ExtendedInterfaceFile::addObjCClass(StringRef name,
                                               ArchitectureSet archs,
                                               XPIAccess access,
                                               ObjCClass *superClass) {
  return _symbols->addObjCClass(name, archs, access, superClass);
}

ObjCSelector *ExtendedInterfaceFile::addObjCSelector(
    ObjCContainer *container, StringRef name, ArchitectureSet archs,
    bool isInstanceMethod, bool isDynamic, XPIAccess access) {
  return _symbols->addObjCSelector(container, name, archs, isInstanceMethod,
                                   isDynamic, access);
}

ObjCCategory *ExtendedInterfaceFile::addObjCCategory(ObjCClass *baseClass,
                                                     StringRef name,
                                                     ArchitectureSet archs,
                                                     XPIAccess access) {
  return _symbols->addObjCCategory(baseClass, name, archs, access);
}

ObjCProtocol *ExtendedInterfaceFile::addObjCProtocol(StringRef name,
                                                     ArchitectureSet archs,
                                                     XPIAccess access) {
  return _symbols->addObjCProtocol(name, archs, access);
}

void ExtendedInterfaceFile::addUndefinedSymbol(XPIKind kind, StringRef name,
                                               ArchitectureSet archs,
                                               SymbolFlags flags) {
  switch (kind) {
  default:
    llvm_unreachable("invalid XPI kind");
  case XPIKind::GlobalSymbol:
    _undefineds->addGlobalSymbol(name, archs, flags, XPIAccess::Exported);
    break;
  case XPIKind::ObjectiveCClass:
    _undefineds->addObjCClass(name, archs, XPIAccess::Exported);
    break;
  case XPIKind::ObjectiveCClassEHType:
    _undefineds->addObjCClassEHType(name, archs, XPIAccess::Exported);
    break;
  case XPIKind::ObjectiveCInstanceVariable:
    _undefineds->addObjCInstanceVariable(name, archs, XPIAccess::Exported);
    break;
  }
}

bool ExtendedInterfaceFile::convertTo(FileType fileType, StringRef path) {
  switch (fileType) {
  case FileType::TBD_V1:
  case FileType::TBD_V2:
    if (!path.empty() && !path.endswith(".tbd"))
      return false;
    break;
  case FileType::API_V1:
    if (!path.empty() && !path.endswith(".api"))
      return false;
    break;
  case FileType::SPI_V1:
    if (!path.empty() && !path.endswith(".spi"))
      return false;
    break;
  default:
    return false;
  }

  if ((fileType == FileType::TBD_V1) &&
      (!isTwoLevelNamespace() || !isApplicationExtensionSafe()))
    return false;

  setFileType(fileType);

  if (!path.empty())
    setPath(path.str());
  else {
    SmallString<PATH_MAX> newpath(getPath());
    StringRef extension;
    switch (fileType) {
    case FileType::TBD_V1:
    case FileType::TBD_V2:
      extension = ".tbd";
      break;
    case FileType::API_V1:
      extension = ".api";
      break;
    case FileType::SPI_V1:
      extension = ".spi";
      break;
    default:
      llvm_unreachable("Unsupported file type for conversion.");
    }
    TAPI_INTERNAL::replace_extension(newpath, extension);
    setPath(newpath.str().str());
  }

  return true;
}

bool ExtendedInterfaceFile::contains(XPIKind kind, StringRef name,
                                     XPI const **result) const {
  if (auto *it = _symbols->findSymbol(kind, name)) {
    if (result)
      *result = it;
    return true;
  }

  return false;
}

Expected<std::unique_ptr<ExtendedInterfaceFile>> ExtendedInterfaceFile::merge(
    const ExtendedInterfaceFile *otherInterface) const {
  // Verify files can be merged.
  if (getFileType() != otherInterface->getFileType()) {
    return make_error<StringError>("file types do not match",
                                   inconvertibleErrorCode());
  }

  if ((getArchitectures() & otherInterface->getArchitectures()) !=
      Architecture::unknown) {
    return make_error<StringError>("architectures overlap",
                                   inconvertibleErrorCode());
  }

  if (getPlatform() != otherInterface->getPlatform()) {
    return make_error<StringError>("platforms do not match",
                                   inconvertibleErrorCode());
  }

  if (getInstallName() != otherInterface->getInstallName()) {
    return make_error<StringError>("install names do not match",
                                   inconvertibleErrorCode());
  }

  if (getCurrentVersion() != otherInterface->getCurrentVersion()) {
    return make_error<StringError>("current versions do not match",
                                   inconvertibleErrorCode());
  }

  if (getCompatibilityVersion() != otherInterface->getCompatibilityVersion()) {
    return make_error<StringError>("compatibility versions do not match",
                                   inconvertibleErrorCode());
  }

  if (getSwiftABIVersion() != otherInterface->getSwiftABIVersion()) {
    return make_error<StringError>("swift ABI versions do not match",
                                   inconvertibleErrorCode());
  }

  if (isTwoLevelNamespace() != otherInterface->isTwoLevelNamespace()) {
    return make_error<StringError>("two level namespace flags do not match",
                                   inconvertibleErrorCode());
  }

  if (isApplicationExtensionSafe() !=
      otherInterface->isApplicationExtensionSafe()) {
    return make_error<StringError>(
        "application extension safe flags do not match",
        inconvertibleErrorCode());
  }

  if (isInstallAPI() != otherInterface->isInstallAPI()) {
    return make_error<StringError>("installapi flags do not match",
                                   inconvertibleErrorCode());
  }

  if (getObjCConstraint() != otherInterface->getObjCConstraint()) {
    return make_error<StringError>("installapi flags do not match",
                                   inconvertibleErrorCode());
  }

  if (getParentUmbrella() != otherInterface->getParentUmbrella()) {
    return make_error<StringError>("parent umbrellas do not match",
                                   inconvertibleErrorCode());
  }

  std::unique_ptr<ExtendedInterfaceFile> interface(new ExtendedInterfaceFile());
  interface->setFileType(getFileType());
  interface->setPath(getPath());
  interface->setPlatform(getPlatform());
  interface->setInstallName(getInstallName());
  interface->setCurrentVersion(getCurrentVersion());
  interface->setCompatibilityVersion(getCompatibilityVersion());
  interface->setSwiftABIVersion(getSwiftABIVersion());
  interface->setTwoLevelNamespace(isTwoLevelNamespace());
  interface->setApplicationExtensionSafe(isApplicationExtensionSafe());
  interface->setInstallAPI(isInstallAPI());
  interface->setObjCConstraint(getObjCConstraint());
  interface->setParentUmbrella(getParentUmbrella());

  interface->setArchitectures(getArchitectures() |
                              otherInterface->getArchitectures());

  for (const auto &lib : allowableClients())
    interface->addAllowableClient(lib.getInstallName(), lib.getArchitectures());

  for (const auto &lib : otherInterface->allowableClients())
    interface->addAllowableClient(lib.getInstallName(), lib.getArchitectures());

  for (const auto &lib : reexportedLibraries())
    interface->addReexportedLibrary(lib.getInstallName(),
                                    lib.getArchitectures());

  for (const auto &lib : otherInterface->reexportedLibraries())
    interface->addReexportedLibrary(lib.getInstallName(),
                                    lib.getArchitectures());

  for (const auto &uuid : uuids())
    interface->addUUID(uuid.first, uuid.second);

  for (const auto &uuid : otherInterface->uuids())
    interface->addUUID(uuid.first, uuid.second);

  for (const auto *symbol : symbols())
    interface->addSymbol(symbol->getKind(), symbol->getName(),
                         symbol->getArchitectures(), symbol->getSymbolFlags(),
                         symbol->getAccess());

  for (const auto *symbol : otherInterface->symbols())
    interface->addSymbol(symbol->getKind(), symbol->getName(),
                         symbol->getArchitectures(), symbol->getSymbolFlags(),
                         symbol->getAccess());

  for (const auto *symbol : undefineds())
    interface->addUndefinedSymbol(symbol->getKind(), symbol->getName(),
                                  symbol->getArchitectures(),
                                  symbol->getSymbolFlags());

  for (const auto *symbol : otherInterface->undefineds())
    interface->addUndefinedSymbol(symbol->getKind(), symbol->getName(),
                                  symbol->getArchitectures(),
                                  symbol->getSymbolFlags());

  return std::move(interface);
}

void ExtendedInterfaceFile::printSymbolsForArch(Architecture arch) const {
  std::vector<std::string> exports;
  for (const auto *symbol : this->exports()) {
    if (!symbol->getArchitectures().has(arch))
      continue;

    switch (symbol->getKind()) {
    case XPIKind::GlobalSymbol:
      exports.emplace_back(symbol->getName());
      break;
    case XPIKind::ObjectiveCClass:
      if (getPlatform() == Platform::OSX && arch == Architecture::i386) {
        exports.emplace_back(".objc_class_name_" + symbol->getName().str());
      } else {
        exports.emplace_back("_OBJC_CLASS_$_" + symbol->getName().str());
        exports.emplace_back("_OBJC_METACLASS_$_" + symbol->getName().str());
      }
      break;
    case XPIKind::ObjectiveCClassEHType:
      exports.emplace_back("_OBJC_EHTYPE_$_" + symbol->getName().str());
      break;
    case XPIKind::ObjectiveCInstanceVariable:
      exports.emplace_back("_OBJC_IVAR_$_" + symbol->getName().str());
      break;
    default:
      llvm_unreachable("Unexpected symbol kind for exported symbols");
    }
  }

  sort(exports);
  for (auto &symbol : exports)
    outs() << symbol << "\n";
}

void ExtendedInterfaceFile::printSymbols(ArchitectureSet archs) const {
  if (archs.empty())
    archs = getArchitectures();

  if (getArchitectures().contains(archs)) {
    bool firstItr = true;
    for (auto arch : getArchitectures()) {
      if (!archs.has(arch))
        continue;

      if (firstItr)
        firstItr = false;
      else
        outs() << "\n";
      if (archs.count() > 1)
        outs() << getPath() << " (for architecture " << arch << "):\n";
      printSymbolsForArch(arch);
    }
  } else {
    outs() << "file: " << getPath()
           << " does not contain architecture: " << archs << "\n";
  }
}

TAPI_NAMESPACE_INTERNAL_END
