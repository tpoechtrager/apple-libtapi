//===- tapi/Core/SDKDBFile.h - TAPI SDKDB File ------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief A generic representation for SDKDB files.
///
//===----------------------------------------------------------------------===//
#ifndef TAPI_CORE_SDKDB_FILE_H
#define TAPI_CORE_SDKDB_FILE_H

#include "tapi/Core/File.h"
#include "tapi/Core/InterfaceFile.h"
#include "tapi/Core/XPISet.h"
#include "tapi/Defines.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/YAMLTraits.h"
#include <string>

TAPI_NAMESPACE_INTERNAL_BEGIN

class SDKDBFile : public File {
public:
  struct ObjCClassEntry;
  struct ObjCCategoryEntry;
  struct ObjCProtocolEntry;
  struct ObjCContainerEntry;

  static bool classof(const File *file) {
    return file->kind() == File::Kind::SDKDBFile;
  }

  SDKDBFile() : File(File::Kind::SDKDBFile) {}
  SDKDBFile(SDKDBFile &&other)
      : File(File::Kind::SDKDBFile), error(std::move(other.error)) {
    installName = std::move(other.installName);
    symbols = std::move(other.symbols);
    classes = std::move(other.classes);
    categories = std::move(other.categories);
    protocols = std::move(other.protocols);
  }

  SDKDBFile &operator=(SDKDBFile &&) = default;
  virtual ~SDKDBFile() = default;

  /// \brief Set the install name for the framework/dylib.
  void setInstallName(StringRef installName) {
    this->installName = installName;
  }
  StringRef getInstallName() const { return installName; }

  /// \brief Merge the declarations from the headers with the definitions from
  ///        the dyanmic library file and add them to the SDKDB file.
  void addXPISets(const XPISet *declarations, const XPISet *definitions);

  void addGlobalSymbol(StringRef name, bool isPublic,
                       const AvailabilityInfo &availability);
  ObjCClassEntry *addObjectiveCClass(StringRef name, StringRef superClassName,
                                     bool isPublic,
                                     const AvailabilityInfo &availability);
  ObjCCategoryEntry *
  addObjectiveCCategory(StringRef name, StringRef baseClassName, bool isPublic,
                        const AvailabilityInfo &availability);
  ObjCProtocolEntry *
  addObjectiveCProtocol(StringRef name, bool isPublic,
                        const AvailabilityInfo &availability);
  void addObjectiveCMethod(ObjCContainerEntry *objcContainer, StringRef name,
                           bool isInstanceMethod, bool isPublic,
                           const AvailabilityInfo &availability);

  /// \brief Merge two SDKDBFile instance
  void merge(SDKDBFile &&Other);

  /// \brief Merge ObjC Category into the base class.
  void categoryMerge();

  /// \brief Take SDKDB internal errors.
  llvm::Error takeError() { return std::move(error); }

  /// \brief Print SDKDBFile in human readable format.
  LLVM_DUMP_METHOD void dump(raw_ostream &os) const;
  LLVM_DUMP_METHOD void dump() const { dump(llvm::errs()); }

  llvm::Error verifySDKDBFile(const SDKDBFile *baseline) const;

  struct InfoEntry {
    std::string name;
    bool isPublic = false;
    AvailabilityInfo availability;

    InfoEntry() = default;
    InfoEntry(StringRef name, bool isPublic,
              const AvailabilityInfo &availability)
        : name(name), isPublic(isPublic), availability(availability) {}

    bool isAvailable() const { return !availability._unavailable; }
    std::string toString() const;
  };

  using SymbolEntry = InfoEntry;

  struct MethodEntry : InfoEntry {
    bool isInstanceMethod;

    MethodEntry(StringRef name, bool isInstanceMethod, bool isPublic,
                const AvailabilityInfo &availability)
        : InfoEntry(name, isPublic, availability),
          isInstanceMethod(isInstanceMethod) {}
  };

  struct MethodMapKey {
    StringRef name;
    bool isInstanceMethod;

    MethodMapKey(StringRef name, bool isInstanceMethod)
        : name(name), isInstanceMethod(isInstanceMethod) {}
  };

  struct MethodMapKeyLess {
    bool operator()(const MethodMapKey &lhs, const MethodMapKey &rhs) const {
      return std::tie(lhs.name, lhs.isInstanceMethod) <
             std::tie(rhs.name, rhs.isInstanceMethod);
    }
  };

  using MethodMapType = std::map<MethodMapKey, MethodEntry, MethodMapKeyLess>;

  struct ObjCContainerEntry : public InfoEntry {
    MethodMapType methods;

    ObjCContainerEntry() = default;
    ObjCContainerEntry(StringRef name, bool isPublic,
                       const AvailabilityInfo &availability)
        : InfoEntry(name, isPublic, availability) {}
  };

  struct ObjCClassEntry : public ObjCContainerEntry {
    std::string superClassName;

    ObjCClassEntry() = default;
    ObjCClassEntry(StringRef name, StringRef superClassName, bool isPublic,
                   const AvailabilityInfo &availability)
        : ObjCContainerEntry(name, isPublic, availability),
          superClassName(superClassName) {}
  };

  struct ObjCCategoryEntry : public ObjCContainerEntry {
    std::string baseClassName;

    ObjCCategoryEntry() = default;
    ObjCCategoryEntry(StringRef name, StringRef baseClassName, bool isPublic,
                      const AvailabilityInfo &availability)
        : ObjCContainerEntry(name, isPublic, availability),
          baseClassName(baseClassName) {}
  };

  struct CategoriesMapKey {
    StringRef className;
    StringRef categoryName;

    CategoriesMapKey(StringRef className, StringRef categoryName)
        : className(className), categoryName(categoryName) {}
  };

  struct CategoriesMapKeyLess {
    bool operator()(const CategoriesMapKey &lhs,
                    const CategoriesMapKey &rhs) const {
      return std::tie(lhs.className, lhs.categoryName) <
             std::tie(rhs.className, rhs.categoryName);
    }
  };

  struct ObjCProtocolEntry : public ObjCContainerEntry {
    ObjCProtocolEntry() = default;
    ObjCProtocolEntry(StringRef name, bool isPublic,
                      const AvailabilityInfo &availability)
        : ObjCContainerEntry(name, isPublic, availability) {}
  };

private:
  using SymbolsMapType = std::map<std::string, SymbolEntry>;
  using ObjCClassMapType = std::map<std::string, ObjCClassEntry>;
  using ObjCCategoryMapType =
      std::map<CategoriesMapKey, ObjCCategoryEntry, CategoriesMapKeyLess>;
  using ObjCProtocolMapType = std::map<std::string, ObjCProtocolEntry>;

  std::string installName;
  SymbolsMapType symbols;
  ObjCClassMapType classes;
  ObjCCategoryMapType categories;
  ObjCProtocolMapType protocols;

  llvm::Error error = llvm::Error::success();

  template <typename T> friend struct llvm::yaml::MappingTraits;

  void appendError(llvm::Error &&Err) {
    error = joinErrors(std::move(error), std::move(Err));
  }

};

TAPI_NAMESPACE_INTERNAL_END

#endif
