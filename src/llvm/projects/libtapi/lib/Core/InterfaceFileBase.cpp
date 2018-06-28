//===- lib/Core/InterfaceFileBase.cpp - Interface File Base -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the Interface File Base
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/InterfaceFileBase.h"
#include "tapi/Core/STLExtras.h"
#include <iomanip>
#include <sstream>

using namespace llvm;

TAPI_NAMESPACE_INTERNAL_BEGIN

namespace {
template <typename C>
typename C::iterator addEntry(C &container, StringRef installName) {
  auto it = lower_bound(container, installName,
                        [](const InterfaceFileRef &lhs, const StringRef &rhs) {
                          return lhs.getInstallName() < rhs;
                        });
  if ((it != std::end(container)) && !(installName < it->getInstallName()))
    return it;

  return container.emplace(it, installName);
}
} // end anonymous namespace.

void InterfaceFileBase::addAllowableClient(StringRef installName,
                                           ArchitectureSet archs) {
  auto client = addEntry(_allowableClients, installName);
  client->setArchitectures(archs);
}

void InterfaceFileBase::addReexportedLibrary(StringRef installName,
                                             ArchitectureSet archs) {
  auto lib = addEntry(_reexportedLibraries, installName);
  lib->setArchitectures(archs);
}

bool InterfaceFileBase::removeReexportedLibrary(StringRef installName) {
  auto it = lower_bound(_reexportedLibraries, installName,
                        [](const InterfaceFileRef &lhs, const StringRef &rhs) {
                          return lhs.getInstallName() < rhs;
                        });

  if ((it == _reexportedLibraries.end()) ||
      (installName < it->getInstallName()))
    return false;

  _reexportedLibraries.erase(it);
  return true;
}

void InterfaceFileBase::addUUID(Architecture arch, StringRef uuid) {
  auto it = lower_bound(_uuids, arch,
                        [](const std::pair<Architecture, std::string> &lhs,
                           Architecture rhs) { return lhs.first < rhs; });

  if ((it != _uuids.end()) && !(arch < it->first)) {
    it->second = uuid;
    return;
  }

  _uuids.emplace(it, arch, uuid);
  return;
}

void InterfaceFileBase::addUUID(uint8_t uuid[16], Architecture arch) {
  std::stringstream stream;
  for (unsigned i = 0; i < 16; ++i) {
    if (i == 4 || i == 6 || i == 8 || i == 10)
      stream << '-';
    stream << std::setfill('0') << std::setw(2) << std::uppercase << std::hex
           << static_cast<int>(uuid[i]);
  }
  addUUID(arch, stream.str());
}

TAPI_NAMESPACE_INTERNAL_END
