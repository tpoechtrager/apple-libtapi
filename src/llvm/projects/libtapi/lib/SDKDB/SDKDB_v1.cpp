//===- tapi/Core/SDKDB.cpp - SDKDB v1 file ----------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the SDKDB reader/writer.
///
//===----------------------------------------------------------------------===//

#include "tapi/SDKDB/SDKDB_v1.h"
#include "tapi/Core/Utils.h"
#include "tapi/Core/YAML.h"
#include "tapi/SDKDB/SDKDBFile.h"
#include "llvm/Support/YAMLTraits.h"

using namespace llvm;
using namespace llvm::yaml;

namespace {

enum class Access {
  Public,
  Private,
};

enum class MethodKind {
  Class,
  Instance,
};

struct Symbol {
  StringRef name;
  Access access;
  AvailabilityInfo availability;

  Symbol() = default;
  Symbol(StringRef name, Access access, const AvailabilityInfo &availability)
      : name(name), access(access), availability(availability) {}
};
using SymbolSeq = std::vector<Symbol>;

struct Method {
  StringRef name;
  MethodKind kind;
  Access access;
  AvailabilityInfo availability;

  Method() = default;
  Method(StringRef name, MethodKind kind, Access access,
         const AvailabilityInfo &availability)
      : name(name), kind(kind), access(access), availability(availability) {}
};
using MethodSeq = std::vector<Method>;

struct Class {
  StringRef name;
  StringRef superClass;
  Access access;
  AvailabilityInfo availability;
  llvm::Optional<MethodSeq> methods;

  Class() = default;
  Class(StringRef name, StringRef superClass, Access access,
        const AvailabilityInfo &availability)
      : name(name), superClass(superClass), access(access),
        availability(availability) {}
};
using ClassSeq = std::vector<Class>;

struct Category {
  StringRef name;
  StringRef className;
  Access access;
  AvailabilityInfo availability;
  llvm::Optional<MethodSeq> methods;

  Category() = default;
  Category(StringRef name, StringRef className, Access access,
           const AvailabilityInfo &availability)
      : name(name), className(className), access(access),
        availability(availability) {}
};
using CategorySeq = std::vector<Category>;

struct Protocol {
  StringRef name;
  Access access;
  AvailabilityInfo availability;
  llvm::Optional<MethodSeq> methods;

  Protocol() = default;
  Protocol(StringRef name, Access access, const AvailabilityInfo &availability)
      : name(name), access(access), availability(availability) {}
};
using ProtocolSeq = std::vector<Protocol>;

} // end anonymous namespace.

LLVM_YAML_IS_SEQUENCE_VECTOR(Symbol)
LLVM_YAML_IS_SEQUENCE_VECTOR(Method)
LLVM_YAML_IS_SEQUENCE_VECTOR(Class)
LLVM_YAML_IS_SEQUENCE_VECTOR(Category)
LLVM_YAML_IS_SEQUENCE_VECTOR(Protocol)

namespace llvm {
namespace yaml {

template <> struct ScalarEnumerationTraits<Access> {
  static void enumeration(IO &io, Access &access) {
    io.enumCase(access, "public", Access::Public);
    io.enumCase(access, "private", Access::Private);
  }
};

template <> struct ScalarEnumerationTraits<MethodKind> {
  static void enumeration(IO &io, MethodKind &kind) {
    io.enumCase(kind, "class", MethodKind::Class);
    io.enumCase(kind, "instance", MethodKind::Instance);
  }
};

template <> struct MappingTraits<Symbol> {
  static void mapping(IO &io, Symbol &symbol) {
    io.mapRequired("name", symbol.name);
    io.mapRequired("access", symbol.access);
    io.mapRequired("availability", symbol.availability);
  }
};

template <> struct MappingTraits<Method> {
  static void mapping(IO &io, Method &method) {
    io.mapRequired("name", method.name);
    io.mapRequired("kind", method.kind);
    io.mapRequired("access", method.access);
    io.mapRequired("availability", method.availability);
  }
};

template <> struct MappingTraits<Class> {
  static void mapping(IO &io, Class &entry) {
    io.mapRequired("name", entry.name);
    io.mapRequired("super-class", entry.superClass);
    io.mapRequired("access", entry.access);
    io.mapRequired("availability", entry.availability);
    io.mapOptional("methods", entry.methods);
  }
};

template <> struct MappingTraits<Category> {
  static void mapping(IO &io, Category &entry) {
    io.mapRequired("name", entry.name);
    io.mapRequired("extends", entry.className);
    io.mapRequired("access", entry.access);
    io.mapRequired("availability", entry.availability);
    io.mapOptional("methods", entry.methods);
  }
};

template <> struct MappingTraits<Protocol> {
  static void mapping(IO &io, Protocol &entry) {
    io.mapRequired("name", entry.name);
    io.mapRequired("access", entry.access);
    io.mapRequired("availability", entry.availability);
    io.mapOptional("methods", entry.methods);
  }
};

using TAPI_INTERNAL::SDKDBFile;
template <> struct MappingTraits<const SDKDBFile *> {
  struct NormalizedSDKDB1 {
    explicit NormalizedSDKDB1(IO &io) {}
    NormalizedSDKDB1(IO &io, const SDKDBFile *&file) {
      installName = file->getInstallName();

      access = TAPI_INTERNAL::isPublicLocation(installName) ? Access::Public
                                                            : Access::Private;

      if (!file->symbols.empty())
        symbols = SymbolSeq();

      for (const auto &it : file->symbols)
        symbols->emplace_back(it.second.name,
                              it.second.isPublic ? Access::Public
                                                 : Access::Private,
                              it.second.availability);

      if (!file->classes.empty())
        classes = ClassSeq();

      for (const auto &it : file->classes) {
        const auto &objcClass = it.second;
        Class entry(objcClass.name, objcClass.superClassName,
                    objcClass.isPublic ? Access::Public : Access::Private,
                    objcClass.availability);

        if (!objcClass.methods.empty())
          entry.methods = MethodSeq();

        for (const auto &it2 : objcClass.methods) {
          const auto &method = it2.second;
          entry.methods->emplace_back(
              method.name,
              method.isInstanceMethod ? MethodKind::Instance
                                      : MethodKind::Class,
              method.isPublic ? Access::Public : Access::Private,
              method.availability);
        }

        classes->emplace_back(entry);
      }

      if (!file->categories.empty())
        categories = CategorySeq();

      for (const auto &it : file->categories) {
        const auto &category = it.second;
        Category entry(category.name, category.baseClassName,
                       category.isPublic ? Access::Public : Access::Private,
                       category.availability);

        if (!category.methods.empty())
          entry.methods = MethodSeq();

        for (const auto &it2 : category.methods) {
          const auto &method = it2.second;
          entry.methods->emplace_back(
              method.name,
              method.isInstanceMethod ? MethodKind::Instance
                                      : MethodKind::Class,
              method.isPublic ? Access::Public : Access::Private,
              method.availability);
        }

        categories->emplace_back(entry);
      }

      if (!file->protocols.empty())
        protocols = ProtocolSeq();

      for (const auto &it : file->protocols) {
        const auto &protocol = it.second;
        Protocol entry(protocol.name,
                       protocol.isPublic ? Access::Public : Access::Private,
                       protocol.availability);

        if (!protocol.methods.empty())
          entry.methods = MethodSeq();

        for (const auto &it2 : protocol.methods) {
          const auto &method = it2.second;
          entry.methods->emplace_back(
              method.name,
              method.isInstanceMethod ? MethodKind::Instance
                                      : MethodKind::Class,
              method.isPublic ? Access::Public : Access::Private,
              method.availability);
        }

        protocols->emplace_back(entry);
      }
    }

    const SDKDBFile *denormalize(IO &io) {
      auto *file = new SDKDBFile;

      file->setInstallName(installName);

      if (symbols) {
        for (const auto &symbol : *symbols)
          file->addGlobalSymbol(symbol.name, symbol.access == Access::Public,
                                symbol.availability);
      }

      if (classes) {
        for (const auto &objcClass : *classes) {
          auto *entry = file->addObjectiveCClass(
              objcClass.name, objcClass.superClass,
              objcClass.access == Access::Public, objcClass.availability);

          if (objcClass.methods) {
            for (const auto &method : *objcClass.methods)
              file->addObjectiveCMethod(
                  entry, method.name, method.kind == MethodKind::Instance,
                  method.access == Access::Public, method.availability);
          }
        }
      }

      if (categories) {
        for (const auto &category : *categories) {
          auto *entry = file->addObjectiveCCategory(
              category.name, category.className,
              category.access == Access::Public, category.availability);

          if (category.methods) {
            for (const auto &method : *category.methods)
              file->addObjectiveCMethod(
                  entry, method.name, method.kind == MethodKind::Instance,
                  method.access == Access::Public, method.availability);
          }
        }
      }

      if (protocols) {
        for (const auto &protocol : *protocols) {
          auto *entry = file->addObjectiveCProtocol(
              protocol.name, protocol.access == Access::Public,
              protocol.availability);

          if (protocol.methods) {
            for (const auto &method : *protocol.methods)
              file->addObjectiveCMethod(
                  entry, method.name, method.kind == MethodKind::Instance,
                  method.access == Access::Public, method.availability);
          }
        }
      }

      return file;
    }

    StringRef installName;
    Access access;
    Optional<SymbolSeq> symbols;
    Optional<ClassSeq> classes;
    Optional<CategorySeq> categories;
    Optional<ProtocolSeq> protocols;
  };

  static void mappingSDKDB1(IO &io, const SDKDBFile *&file) {
    MappingNormalization<NormalizedSDKDB1, const SDKDBFile *> keys(io, file);
    io.mapTag("!tapi-sdkdb-v1", true);
    io.mapRequired("install-name", keys->installName);
    io.mapRequired("access", keys->access);
    io.mapOptional("symbols", keys->symbols);
    io.mapOptional("classes", keys->classes);
    io.mapOptional("categories", keys->categories);
    io.mapOptional("protocols", keys->protocols);
  }
};

} // end namespace yaml.
} // end namespace llvm.

TAPI_NAMESPACE_INTERNAL_BEGIN

namespace sdkdb {
namespace v1 {

bool YAMLDocumentHandler::canRead(MemoryBufferRef memBufferRef,
                                  FileType types) const {
  if (!(types & FileType::SDKDB_V1))
    return false;

  auto str = memBufferRef.getBuffer().trim();
  if (!str.startswith("--- !tapi-sdkdb-v1\n") || !str.endswith("..."))
    return false;

  return true;
}

FileType YAMLDocumentHandler::getFileType(MemoryBufferRef memBufferRef) const {
  if (canRead(memBufferRef))
    return FileType::SDKDB_V1;

  return FileType::Invalid;
}

bool YAMLDocumentHandler::canWrite(const File *file) const {
  auto *sdkdb = dyn_cast<SDKDBFile>(file);
  if (sdkdb == nullptr)
    return false;

  if (sdkdb->getFileType() != FileType::SDKDB_V1)
    return false;

  return true;
}

bool YAMLDocumentHandler::handleDocument(IO &io, const File *&file) const {
  if (io.outputting() && file->getFileType() != FileType::SDKDB_V1)
    return false;

  if (!io.outputting() && !io.mapTag("!tapi-sdkdb-v1"))
    return false;

  const auto *sdkdb = dyn_cast_or_null<SDKDBFile>(file);
  MappingTraits<const SDKDBFile *>::mappingSDKDB1(io, sdkdb);
  file = sdkdb;

  return true;
}

} // end namespace v1.
} // end namespace sdkdb.

TAPI_NAMESPACE_INTERNAL_END
