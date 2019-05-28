//===- lib/Core/Framework.cpp - Framework Context ---------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the Framework context.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/Framework.h"
#include "tapi/Core/HeaderFile.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"
#include <set>

using namespace llvm;

TAPI_NAMESPACE_INTERNAL_BEGIN

StringRef Framework::getName() const {
  StringRef path = _baseDirectory;
  // Returns the framework name extract from path.
  while (!path.empty()) {
    if (path.endswith(".framework"))
      return sys::path::filename(path);
    path = sys::path::parent_path(path);
  }

  // Otherwise, return the name of the baseDirectory.
  // First, remove all the trailing seperator.
  path = _baseDirectory;
  return sys::path::filename(path.rtrim("/"));
}

bool Framework::verify(bool warnAll) const {
  for (auto &framework : _subFrameworks)
    framework.verify(warnAll);

  for (auto &framework : _versions)
    framework.verify(warnAll);

  std::set<const XPI *> visitedSymbols;
  std::vector<const XPI *> symbols;
  for (auto &dylib : _interfaceFiles) {
    // Check if each symbol that is exported from the dylib is also in a header
    // file.
    symbols.clear();
    for (const auto *symbol : dylib->symbols())
      symbols.emplace_back(symbol);
    sort(symbols, [](const XPI *lhs, const XPI *rhs) { return *lhs < *rhs; });
    for (const auto *dsym : symbols) {
      const auto &hsym =
          _headerSymbols->findSymbol(dsym->getKind(), dsym->getName());
      if (hsym == nullptr && dsym->isExportedSymbol()) {
        if (warnAll) {
          outs() << "warning: dylib " << dylib->getFileName()
                 << " exports symbol " << dsym->getAnnotatedName()
                 << " without a definition in a header file.\n";
        }
        continue;
      }

      if (!dsym->isExportedSymbol())
        continue;

      // Check the availability information.
      for (auto arch : dylib->getArchitectures()) {
        auto davail = dsym->getAvailabilityInfo(arch);
        auto havail = hsym->getAvailabilityInfo(arch);
        if (!davail && !havail)
          continue;

        if (!davail) {
          outs() << "warning: headers exports symbol "
                 << hsym->getAnnotatedName() << " (" << arch << ")"
                 << " which is not exported by the dylib.\n";
          continue;
        }

        if (!havail) {
          if (warnAll) {
            outs() << "warning: dylib " << dylib->getFileName()
                   << " exports symbol " << dsym->getAnnotatedName() << " ("
                   << arch << ")"
                   << " which is not defined by a header file.\n";
          }
          continue;
        }

        if (havail.getValue()._unavailable) {
          if (warnAll) {
            outs() << "warning: dylib " << dylib->getFileName()
                   << " exports symbol " << dsym->getAnnotatedName() << " ("
                   << arch << ")"
                   << " which is marked as not available in headers.\n";
          }
          continue;
        }
      }

      visitedSymbols.insert(hsym);
    }

    for (const auto &dselector : dylib->selectors()) {
      const auto *hselector = _headerSymbols->findSelector(dselector.first);

      if (hselector == nullptr) {
        if (warnAll) {
          outs() << "warning: dylib " << dylib->getFileName() << " exports "
                 << dselector.first.containerName
                 << "::" << dselector.second->getAnnotatedName()
                 << " without a definition in a header file.\n";
        }
        continue;
      }

      // Check the availability information.
      for (auto arch : dylib->getArchitectures()) {
        auto davail = dselector.second->getAvailabilityInfo(arch);
        auto havail = hselector->getAvailabilityInfo(arch);
        if (!davail && !havail)
          continue;

        if (!davail) {
          outs() << "warning: headers exports " << dselector.first.containerName
                 << "::" << hselector->getAnnotatedName() << " (" << arch << ")"
                 << " which is not exported by the dylib.\n";
          continue;
        }

        if (!havail) {
          if (warnAll) {
            outs() << "warning: dylib " << dylib->getFileName()
                   << " exports symbol " << dselector.first.containerName
                   << "::" << dselector.second->getAnnotatedName() << " ("
                   << arch << ")"
                   << " which is not defined by a header file.\n";
          }
          continue;
        }

        if (havail.getValue()._unavailable) {
          if (warnAll) {
            outs() << "warning: dylib " << dylib->getFileName()
                   << " exports symbol " << dselector.first.containerName
                   << "::" << dselector.second->getAnnotatedName() << " ("
                   << arch << ")"
                   << " which is marked as not available in headers.\n";
          }
          continue;
        }
      }

      visitedSymbols.insert(hselector);
    }
  }

  // Check if the headers are exporting more symbols than the dylib.
  symbols.clear();
  for (const auto *symbol : _headerSymbols->exports())
    symbols.emplace_back(symbol);
  sort(symbols, [](const XPI *lhs, const XPI *rhs) { return *lhs < *rhs; });
  for (const auto *sym : symbols) {
    if (!visitedSymbols.count(sym) && !sym->isUnavailable()) {
      outs() << "warning: headers exports symbol " << sym->getAnnotatedName()
             << " without an export in a dylib.\n";
    }
  }

  for (const auto &sym : _headerSymbols->selectors()) {
    // Protocol methods are not implemented.
    if (sym.first.bits.containerIsProtocol)
      continue;

    auto *selector = sym.second;

    if (selector->isDerivedFromProtocol())
      continue;

    if (!visitedSymbols.count(selector) && !selector->isUnavailable() &&
        !selector->isDynamic()) {
      outs() << "warning: header exports " << sym.first.containerName
             << "::" << selector->getAnnotatedName()
             << " without an export in a dylib.\n";
    }
  }

  return true;
}

void Framework::print(raw_ostream &os) const {
  os << "=== Framework " << getName() << " ===\n"
     << "directory: " << _baseDirectory << "\n"
     << "header files:\n";
  for (auto &header : _headerFiles)
    os << "  " << header << "\n";
  os << "dylib files:\n";
  for (auto file : _dynamicLibraryFiles)
    os << "  " << sys::path::filename(file) << "\n";
  os << "dynamic libraries:\n";
  // FIXME:
  /*for (auto &dylib : _interfaceFiles)
    os << dylib << "\n";*/
  os << "header symbols:\n";
  for (const auto *header : _headerSymbols->symbols())
    os << "  " << *header << "\n";
  os << "sub frameworks:\n";
  for (auto &framework : _subFrameworks)
    os << framework << "\n";
  os << "versions:\n";
  for (auto &framework : _versions)
    os << framework << "\n";
}

TAPI_NAMESPACE_INTERNAL_END
