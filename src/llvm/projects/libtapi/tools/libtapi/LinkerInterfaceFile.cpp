//===- libtapi/LinkerInterfaceFile.cpp - TAPI File Interface ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the C++ linker interface file API.
///
//===----------------------------------------------------------------------===//
#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/InterfaceFile.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Core/Registry.h"
#include "tapi/Core/STLExtras.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Object/MachO.h"
#include <string>
#include <tapi/LinkerInterfaceFile.h>
#include <tapi/PackedVersion32.h>
#include <vector>

using namespace llvm;

TAPI_NAMESPACE_V1_BEGIN

using namespace tapi::internal;

static PackedVersion32 parseVersion32(StringRef str) {
  uint32_t version = 0;
  if (str.empty())
    return 0;

  SmallVector<StringRef, 3> parts;
  SplitString(str, parts, ".");

  unsigned long long num = 0;
  if (getAsUnsignedInteger(parts[0], 10, num))
    return 0;

  if (num > UINT16_MAX)
    return 0;

  version = num << 16;

  if (parts.size() > 1) {
    if (getAsUnsignedInteger(parts[1], 10, num))
      return 0;

    if (num > UINT8_MAX)
      return 0;

    version |= (num << 8);
  }

  if (parts.size() > 2) {
    if (getAsUnsignedInteger(parts[2], 10, num))
      return 0;

    if (num > UINT8_MAX)
      return 0;

    version |= num;
  }

  return version;
}

class LinkerInterfaceFile::Impl {
public:
  FileType _fileType{FileType::Unsupported};
  Platform _platform{Platform::Unknown};
  std::string _installName;
  std::string _parentFrameworkName;

  PackedVersion32 _currentVersion;
  PackedVersion32 _compatibilityVersion;
  unsigned _swiftABIVersion;
  ObjCConstraint _objcConstraint;
  bool _hasTwoLevelNamespace{false};
  bool _isAppExtensionSafe{false};
  bool _hasWeakDefExports{false};
  bool _installPathOverride{false};

  std::vector<std::string> _reexportedLibraries;
  std::vector<std::string> _allowableClients;
  std::vector<std::string> _ignoreExports;
  std::vector<Symbol> _exports;
  std::vector<Symbol> _undefineds;

  Impl() noexcept = default;

  template <typename T> void addSymbol(T &&name, SymbolFlags flags) {
    if (find(_ignoreExports, name) == _ignoreExports.end())
      _exports.emplace_back(std::forward<T>(name), flags);
  }

  void processSymbol(StringRef name, PackedVersion32 minOSVersion,
                     bool disallowWeakImports) {
    // $ld$ <action> $ <condition> $ <symbol-name>
    if (!name.startswith("$ld$"))
      return;

    StringRef action, condition, symbolName;
    std::tie(action, name) = name.drop_front(4).split('$');
    std::tie(condition, symbolName) = name.split('$');
    if (action.empty() || condition.empty() || symbolName.empty())
      return;

    if (!condition.startswith("os"))
      return;

    auto version = parseVersion32(condition.drop_front(2));
    if (version != minOSVersion)
      return;

    if (action == "hide") {
      _ignoreExports.emplace_back(symbolName);
      return;
    }

    if (action == "add") {
      _exports.emplace_back(symbolName);
      return;
    }

    if (action == "weak") {
      if (disallowWeakImports)
        _ignoreExports.emplace_back(symbolName);

      return;
    }

    if (action == "install_name") {
      _installName = symbolName;
      _installPathOverride = true;
      if (_installName == "/System/Library/Frameworks/"
                          "ApplicationServices.framework/Versions/A/"
                          "ApplicationServices") {
        _compatibilityVersion = PackedVersion32(1, 0, 0);
      }
      return;
    }

    if (action == "compatibility_version") {
      _compatibilityVersion = parseVersion32(symbolName);
      return;
    }
  }
};

static Architecture getArchForCPU(cpu_type_t cpuType, cpu_subtype_t cpuSubType,
                                  bool enforceCpuSubType,
                                  ArchitectureSet archs) {
  // First check the exact cpu type and cpu sub type.
  auto arch = getArchType(cpuType, cpuSubType);
  if (archs.has(arch))
    return arch;

  if (enforceCpuSubType)
    return Architecture::unknown;

  // Find ABI compatible slice instead.
  return archs.getABICompatibleSlice(arch);
}

LinkerInterfaceFile::LinkerInterfaceFile() noexcept
    : _pImpl{new LinkerInterfaceFile::Impl} {}
LinkerInterfaceFile::~LinkerInterfaceFile() noexcept = default;
LinkerInterfaceFile::LinkerInterfaceFile(LinkerInterfaceFile &&) noexcept =
    default;
LinkerInterfaceFile &LinkerInterfaceFile::
operator=(LinkerInterfaceFile &&) noexcept = default;

std::vector<std::string>
LinkerInterfaceFile::getSupportedFileExtensions() noexcept {
  return {".tbd"};
}

/// \brief Load and parse the provided TBD file in the buffer and return on
///        success the interface file.
static Expected<std::unique_ptr<const InterfaceFile>>
loadFile(std::unique_ptr<MemoryBuffer> buffer,
         ReadFlags readFlags = ReadFlags::Symbols) {
  Registry registry;
  registry.addYAMLReaders();

  auto textFile = registry.readFile(std::move(buffer), readFlags);
  if (!textFile)
    return textFile.takeError();

  return std::unique_ptr<const InterfaceFile>(
      cast<const InterfaceFile>(textFile.get().release()));
}

bool LinkerInterfaceFile::isSupported(const std::string &path,
                                      const uint8_t *data,
                                      size_t size) noexcept {
  Registry registry;
  registry.addYAMLReaders();
  auto memBuffer = MemoryBufferRef(
      StringRef(reinterpret_cast<const char *>(data), size), path);
  return registry.canRead(memBuffer);
}

bool LinkerInterfaceFile::shouldPreferTextBasedStubFile(
    const std::string &path) noexcept {
  auto errorOr = MemoryBuffer::getFile(path);
  if (errorOr.getError())
    return false;

  auto file = loadFile(std::move(errorOr.get()), ReadFlags::Header);
  if (!file) {
    consumeError(file.takeError());
    return false;
  }

  return file.get()->isInstallAPI();
}

bool LinkerInterfaceFile::areEquivalent(const std::string &tbdPath,
                                        const std::string &dylibPath) noexcept {
  fprintf(stderr, "comparing %s with %s is currently not implemented in this "
                  "port of the TAPI library.\n", tbdPath.c_str(), dylibPath.c_str());
  fprintf(stderr, "Assuming they are NOT equal.\n");
  fprintf(stderr, "Please file a bugreport to the apple-libtapi repository if you "
                  "really stumble across this issue.\n");
  return false;
#warning Fix this mess
#if 0
  Registry registry;
  registry.addYAMLReaders();
  registry.addBinaryReaders();

  auto tbdErrorOr = MemoryBuffer::getFile(tbdPath);
  if (tbdErrorOr.getError())
    return false;

  auto textFile = loadFile(std::move(tbdErrorOr.get()), ReadFlags::Header);
  if (!textFile) {
    consumeError(textFile.takeError());
    return false;
  }

  if (textFile.get()->uuids().empty())
    return false;

  auto machoErrorOr = MemoryBuffer::getFile(dylibPath);
  if (machoErrorOr.getError())
    return false;

  auto machoFile =
      registry.readFile(std::move(machoErrorOr.get()), ReadFlags::Header);
  if (!machoFile) {
    consumeError(machoFile.takeError());
    return false;
  }

  auto *dylibFile = cast<InterfaceFileBase>(machoFile.get().get());
  for (const auto &uuid1 : textFile.get()->uuids()) {
    // Ignore unknown architectures.
    if (uuid1.first == Architecture::unknown)
      continue;

    auto it = find_if(dylibFile->uuids(),
                      [&](const std::pair<Architecture, std::string> &uuid2) {
                        return uuid1.first == uuid2.first;
                      });

    if (it == dylibFile->uuids().end())
      continue;

    if (uuid1 != *it)
      return false;
  }
  return true;
#endif
}

LinkerInterfaceFile *LinkerInterfaceFile::create(
    const std::string &path, const uint8_t *data, size_t size,
    cpu_type_t cpuType, cpu_subtype_t cpuSubType,
    CpuSubTypeMatching matchingMode, PackedVersion32 minOSVersion,
    std::string &errorMessage) noexcept {
  ParsingFlags flags = (matchingMode == CpuSubTypeMatching::Exact)
                           ? ParsingFlags::ExactCpuSubType
                           : ParsingFlags::None;

  return create(path, data, size, cpuType, cpuSubType, flags, minOSVersion,
                errorMessage);
}

LinkerInterfaceFile *LinkerInterfaceFile::create(
    const std::string &path, const uint8_t *data, size_t size,
    cpu_type_t cpuType, cpu_subtype_t cpuSubType, ParsingFlags flags,
    PackedVersion32 minOSVersion, std::string &errorMessage) noexcept {
  if (path.empty() || data == nullptr || size < 8) {
    errorMessage = "invalid argument";
    return nullptr;
  }

  // Use a copy to make sure the buffer is null-terminated (the YAML parser
  // relies on that). Mmap guarantees that pages are padded with zeros, so
  // this mostly works, but it breaks down when a TBD file size is exactly
  // a multiple of the page size. 
  // We could make the copy conditional on the file size, but as we're going
  // to read it completely anyway, I doubt there's any real performance
  // benefit to balance the added complexity.
  auto input = MemoryBuffer::getMemBufferCopy(
      StringRef(reinterpret_cast<const char *>(data), size), path);

  auto inputFile = loadFile(std::move(input));
  if (!inputFile) {
    errorMessage = toString(inputFile.takeError());
    return nullptr;
  }

  const auto *interface = inputFile.get().get();
  bool enforceCpuSubType = flags & ParsingFlags::ExactCpuSubType;
  auto arch = getArchForCPU(cpuType, cpuSubType, enforceCpuSubType,
                            interface->getArchitectures());
  if (arch == Architecture::unknown) {
    auto arch = getArchType(cpuType, cpuSubType);
    auto count = interface->getArchitectures().count();
    if (count > 1)
      errorMessage = "missing required architecture " +
                     getArchName(arch).str() + " in file " + path + " (" +
                     std::to_string(count) + " slices)";
    else
      errorMessage = "missing required architecture " +
                     getArchName(arch).str() + " in file " + path;
    return nullptr;
  }

  // Remove the patch level.
  minOSVersion =
      PackedVersion32(minOSVersion.getMajor(), minOSVersion.getMinor(), 0);

  auto file = new LinkerInterfaceFile;
  if (file == nullptr) {
    errorMessage = "could not allocate memory";
    return nullptr;
  }

  file->_pImpl->_platform = interface->getPlatform();
  file->_pImpl->_installName = interface->getInstallName();
  file->_pImpl->_currentVersion = interface->getCurrentVersion();
  file->_pImpl->_compatibilityVersion = interface->getCompatibilityVersion();
  file->_pImpl->_hasTwoLevelNamespace = interface->isTwoLevelNamespace();
  file->_pImpl->_isAppExtensionSafe = interface->isApplicationExtensionSafe();
  file->_pImpl->_objcConstraint = interface->getObjCConstraint();
  file->_pImpl->_swiftABIVersion = interface->getSwiftABIVersion();
  file->_pImpl->_parentFrameworkName = interface->getParentUmbrella();
  if (interface->getFileType() == TAPI_INTERNAL::FileType::TBD_V1)
    file->_pImpl->_fileType = FileType::TBD_V1;
  else if (interface->getFileType() == TAPI_INTERNAL::FileType::TBD_V2)
    file->_pImpl->_fileType = FileType::TBD_V2;
  else
    file->_pImpl->_fileType = FileType::Unsupported;

  auto platform = interface->getPlatform();

  // Pre-scan for special linker symbols.
  for (const auto *symbol : interface->exports()) {
    if (symbol->getKind() != SymbolKind::GlobalSymbol)
      continue;

    if (!symbol->getArchitectures().has(arch))
      continue;

    file->_pImpl->processSymbol(symbol->getName(), minOSVersion,
                                flags & ParsingFlags::DisallowWeakImports);
  }
  sort(file->_pImpl->_ignoreExports);
  auto last = std::unique(file->_pImpl->_ignoreExports.begin(),
                          file->_pImpl->_ignoreExports.end());
  file->_pImpl->_ignoreExports.erase(last, file->_pImpl->_ignoreExports.end());

  for (const auto *symbol : interface->exports()) {
    if (!symbol->getArchitectures().has(arch))
      continue;

    switch (symbol->getKind()) {
    case SymbolKind::GlobalSymbol:
      if (symbol->getName().startswith("$ld$"))
        continue;
      file->_pImpl->addSymbol(symbol->getName(), symbol->getFlags());
      break;
    case SymbolKind::ObjectiveCClass:
      if (platform == Platform::OSX && arch == Architecture::i386) {
        file->_pImpl->addSymbol(".objc_class_name_" + symbol->getName().str(),
                                symbol->getFlags());
      } else {
        file->_pImpl->addSymbol("_OBJC_CLASS_$_" + symbol->getName().str(),
                                symbol->getFlags());
        file->_pImpl->addSymbol("_OBJC_METACLASS_$_" + symbol->getName().str(),
                                symbol->getFlags());
      }
      break;
    case SymbolKind::ObjectiveCClassEHType:
      file->_pImpl->addSymbol("_OBJC_EHTYPE_$_" + symbol->getName().str(),
                              symbol->getFlags());
      break;
    case SymbolKind::ObjectiveCInstanceVariable:
      file->_pImpl->addSymbol("_OBJC_IVAR_$_" + symbol->getName().str(),
                              symbol->getFlags());
      break;
    }

    if (symbol->isWeakDefined())
      file->_pImpl->_hasWeakDefExports = true;
  }

  for (const auto *symbol : interface->undefineds()) {
    if (!symbol->getArchitectures().has(arch))
      continue;

    switch (symbol->getKind()) {
    case SymbolKind::GlobalSymbol:
      file->_pImpl->_undefineds.emplace_back(symbol->getName(),
                                             symbol->getFlags());
      break;
    case SymbolKind::ObjectiveCClass:
      if (platform == Platform::OSX && arch == Architecture::i386) {
        file->_pImpl->_undefineds.emplace_back(
            ".objc_class_name_" + symbol->getName().str(), symbol->getFlags());
      } else {
        file->_pImpl->_undefineds.emplace_back(
            "_OBJC_CLASS_$_" + symbol->getName().str(), symbol->getFlags());
        file->_pImpl->_undefineds.emplace_back(
            "_OBJC_METACLASS_$_" + symbol->getName().str(), symbol->getFlags());
      }
      break;
    case SymbolKind::ObjectiveCClassEHType:
      file->_pImpl->_undefineds.emplace_back(
          "_OBJC_EHTYPE_$_" + symbol->getName().str(), symbol->getFlags());
      break;
    case SymbolKind::ObjectiveCInstanceVariable:
      file->_pImpl->_undefineds.emplace_back(
          "_OBJC_IVAR_$_" + symbol->getName().str(), symbol->getFlags());
      break;
    }
  }

  for (const auto &client : interface->allowableClients())
    if (client.hasArchitecture(arch))
      file->_pImpl->_allowableClients.emplace_back(client.getInstallName());

  for (const auto &reexport : interface->reexportedLibraries())
    if (reexport.hasArchitecture(arch))
      file->_pImpl->_reexportedLibraries.emplace_back(
          reexport.getInstallName());

  return file;
}

FileType LinkerInterfaceFile::getFileType() const noexcept {
  return _pImpl->_fileType;
}

Platform LinkerInterfaceFile::getPlatform() const noexcept {
  return _pImpl->_platform;
}

const std::string &LinkerInterfaceFile::getInstallName() const noexcept {
  return _pImpl->_installName;
}

bool LinkerInterfaceFile::isInstallNameVersionSpecific() const noexcept {
  return _pImpl->_installPathOverride;
}

PackedVersion32 LinkerInterfaceFile::getCurrentVersion() const noexcept {
  return _pImpl->_currentVersion;
}

PackedVersion32 LinkerInterfaceFile::getCompatibilityVersion() const noexcept {
  return _pImpl->_compatibilityVersion;
}

unsigned LinkerInterfaceFile::getSwiftVersion() const noexcept {
  return _pImpl->_swiftABIVersion;
}

ObjCConstraint LinkerInterfaceFile::getObjCConstraint() const noexcept {
  return _pImpl->_objcConstraint;
}

bool LinkerInterfaceFile::hasTwoLevelNamespace() const noexcept {
  return _pImpl->_hasTwoLevelNamespace;
}

bool LinkerInterfaceFile::isApplicationExtensionSafe() const noexcept {
  return _pImpl->_isAppExtensionSafe;
}

bool LinkerInterfaceFile::hasAllowableClients() const noexcept {
  return !_pImpl->_allowableClients.empty();
}

bool LinkerInterfaceFile::hasReexportedLibraries() const noexcept {
  return !_pImpl->_reexportedLibraries.empty();
}

bool LinkerInterfaceFile::hasWeakDefinedExports() const noexcept {
  return _pImpl->_hasWeakDefExports;
}

const std::string &LinkerInterfaceFile::getParentFrameworkName() const noexcept {
  return _pImpl->_parentFrameworkName;
}

const std::vector<std::string> &LinkerInterfaceFile::allowableClients() const
    noexcept {
  return _pImpl->_allowableClients;
}

const std::vector<std::string> &LinkerInterfaceFile::reexportedLibraries() const
    noexcept {
  return _pImpl->_reexportedLibraries;
}

const std::vector<std::string> &LinkerInterfaceFile::ignoreExports() const
    noexcept {
  return _pImpl->_ignoreExports;
}

const std::vector<Symbol> &LinkerInterfaceFile::exports() const noexcept {
  return _pImpl->_exports;
}

const std::vector<Symbol> &LinkerInterfaceFile::undefineds() const noexcept {
  return _pImpl->_undefineds;
}

TAPI_NAMESPACE_V1_END
