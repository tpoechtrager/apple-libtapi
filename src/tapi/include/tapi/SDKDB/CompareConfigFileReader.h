//===- tapi/SDKDB/CompareConfigFileReader.h - Config Reader -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief The JSON configuration file is used to communicate additional
///        information to tapi-sdkdb --compare. For now this only includes a
///        list of expected changes that skips diagnostics.
///
//===----------------------------------------------------------------------===//

#ifndef TAPI_SDKDB_COMPARE_CONFIG_FILE_READER_H
#define TAPI_SDKDB_COMPARE_CONFIG_FILE_READER_H

#include "tapi/Core/LLVM.h"
#include "tapi/Defines.h"
#include "tapi/Diagnostics/Diagnostics.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/MemoryBuffer.h"
#include <set>

TAPI_NAMESPACE_INTERNAL_BEGIN

enum class ChangeType {
  Remove,
  UpdateAccess,
  Add,
};

enum class EntryType {
  Library,
  Global,
  Interface,
  Protocol,
  Category,
  Selector,
};

class CompareConfigFileReader {
  class Implementation;

  Implementation &impl;

  CompareConfigFileReader(std::unique_ptr<MemoryBuffer> inputBuffer,
                          llvm::Error &error);

public:
  static llvm::Expected<std::unique_ptr<CompareConfigFileReader>>
  get(std::unique_ptr<llvm::MemoryBuffer> inputBuffer);

  ~CompareConfigFileReader();

  CompareConfigFileReader(const CompareConfigFileReader &) = delete;
  CompareConfigFileReader &operator=(const CompareConfigFileReader &) = delete;

  int getVersion() const;

  struct Change {
    ChangeType change;
    EntryType type;
    StringRef name;
    StringRef installName; // Optional.
    StringRef container;   // Optional. For identifying selectors.

    bool operator==(const Change &other) const {
      bool matchingChange = std::tie(change, type, name) ==
                            std::tie(other.change, other.type, other.name);

      if (type != EntryType::Library)
        matchingChange = matchingChange && installName == other.installName;

      if (type == EntryType::Selector)
        matchingChange = matchingChange && container == other.container;

      return matchingChange;
    }

    bool operator<(const Change &other) const {
      return std::tie(change, type, installName, container, name) <
             std::tie(other.change, other.type, other.installName,
                      other.container, other.name);
    }

    Change() = delete;
    Change(ChangeType change, EntryType type, StringRef name)
        : change(change), type(type), name(name) {}
    Change(ChangeType change, EntryType type, StringRef name,
           StringRef installName)
        : change(change), type(type), name(name), installName(installName) {}
    Change(ChangeType change, EntryType type, StringRef name,
           StringRef installName, StringRef container)
        : change(change), type(type), name(name), installName(installName),
          container(container) {}
  };

  const std::set<Change> &getExpectedChanges(const llvm::Triple) const;
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_SDKDB_COMPARE_CONFIG_FILE_READER_H
