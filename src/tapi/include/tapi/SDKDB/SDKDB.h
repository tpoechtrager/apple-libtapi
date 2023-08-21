//===- tapi/SDKDB/SDKDB.h - TAPI SDKDB --------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief SDKDB interface.
///
//===----------------------------------------------------------------------===//
#ifndef TAPI_SDKDB_SDKDB_H
#define TAPI_SDKDB_SDKDB_H

#include "tapi/Core/API.h"
#include "tapi/Diagnostics/Diagnostics.h"
#include "llvm/ADT/MapVector.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/Error.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

class SDKDBBuilder;

class SDKDB {
public:
  SDKDB(const llvm::Triple &triple, SDKDBBuilder *builder)
      : triple(triple), builder(builder) {}

  /// ObjCContainer type for lookup
  enum ObjCContainerKind : unsigned {
    ObjCClass = 0,
    ObjCCategory = 1,
    ObjCProtocol = 2
  };

  const llvm::Triple &getTargetTriple() const { return triple; }

  API &recordAPI(API &&api);
  void insertGlobal(GlobalRecord *record);
  void insertObjCInterface(ObjCInterfaceRecord *record);
  void insertObjCCategory(ObjCCategoryRecord *record);
  void insertObjCProtocol(ObjCProtocolRecord *record);

  void annotateGlobal(const GlobalRecord *record);
  void annotateObjCInterface(const ObjCInterfaceRecord *record);
  void annotateObjCCategory(const ObjCCategoryRecord *record);
  void annotateObjCProtocol(const ObjCProtocolRecord *record);

  const std::vector<API> &api() const { return apiCache; }

  llvm::Error finalize();

private:
  friend class SDKDBBuilder;

  static const uint8_t GlobalRecordsCapacity{6};
  /// lookup map from global to its api record.
  llvm::StringMap<llvm::SmallPtrSet<GlobalRecord *, GlobalRecordsCapacity>>
      globalMap;
  /// lookup map from objc interface to its api record.
  llvm::StringMap<ObjCInterfaceRecord *> interfaceMap;
  /// lookup map from objc interface to all its categories' api record.
  llvm::StringMap<llvm::StringMap<ObjCCategoryRecord *>> categoryMap;
  /// lookup map from objc protocol to its api record.
  llvm::StringMap<ObjCProtocolRecord *> protocolMap;

  /// get super class record.
  ObjCInterfaceRecord *getSuperclass(const ObjCInterfaceRecord *record);

  /// get objc method access.
  APIAccess getAccessForObjCMethod(APIAccess access, StringRef name,
                                   bool isInstanceMethod,
                                   ObjCContainerRecord *container);
  APIAccess getAccessForObjCMethod(APIAccess access, StringRef name,
                                   bool isInstanceMethod,
                                   ObjCInterfaceRecord *interface);

  /// find and update the global.
  bool findAndUpdateGlobal(Twine name, const APIRecord &record);

  /// find objc method from an interface.
  /// If nothing found in record, it will search all categories for current
  /// record interfaces to find the method.
  /// For categories, it will search fallbackInterface to find other categories
  /// might defined the method.
  ObjCMethodRecord *findMethod(StringRef name, bool isInstanceMethod,
                               ObjCContainerRecord &record,
                               ObjCContainerKind kind,
                               StringRef fallbackInterfaceName = "");

  /// get objc runtime version.
  bool isObjC1() const { return triple.isMacOSX() && triple.isArch32Bit(); }

  const llvm::Triple triple;
  SDKDBBuilder *builder;
  std::vector<API> apiCache;

  llvm::BumpPtrAllocator danglingAPIAllocator;
};

// 16 bits, higher bits reserved for future use.
// This enum is streamed into bitcode so the existing entries cannot be changed.
enum class SDKDBBuilderOptions : uint16_t {
  defaultOpt = 0,
  isPublicOnly = 1,           // only has Public API, default no.
  noObjCMetadata = 1 << 1,    // no objc metadata, default no.
  preserveSourceLoc = 1 << 2, // contain source location, defualt no.
  hasUUID = 1 << 3,           // contains UUID, defualt no.
  excludeBundles = 1 << 4,     // exclude bundles from SDKDB, default no.
  LLVM_MARK_AS_BITMASK_ENUM(excludeBundles)
};

class SDKDBBuilder {
public:
  SDKDBBuilder(DiagnosticsEngine &diag,
               SDKDBBuilderOptions options = SDKDBBuilderOptions::defaultOpt,
               StringRef buildVersion = "")
      : diag(diag), options(options), buildVersion(buildVersion.str()) {}

  /// Add API from binary interface.
  llvm::Error addBinaryAPI(API &&api);

  /// Add API from header interface. It is used to annotate the binary from 
  /// binary interface. All the binary interfaces must be added before calling
  /// this method.
  llvm::Error addHeaderAPI(const API &api);

  /// Finalize SDKDB.
  llvm::Error finalize();

  /// Parse SDKDB JSON.
  llvm::Error parse(StringRef JSON);

  /// Set build verison.
  void setBuildVersion(StringRef version) {
    buildVersion = version.str();
  }

  /// Get build version.
  StringRef getBuildVersion() const { return buildVersion; }

  /// Write output.
  void serialize(raw_ostream &os, bool compact) const;

  /// SDKDB private interface.
  SDKDB &getSDKDBForTarget(const llvm::Triple &triple);

  void updateAPIRecord(APIRecord &base, const APIRecord &record);
  void updateGlobal(GlobalRecord &base, const GlobalRecord &record);

  /// Update ObjCContainer. For categories, it need to pass interfaceName
  /// as a fallback for method searching.
  void updateObjCContainer(SDKDB &sdkdb, ObjCContainerRecord &base,
                           const ObjCContainerRecord &record,
                           SDKDB::ObjCContainerKind kind,
                           StringRef fallbackInterfaceName = "");
  void updateObjCInterface(SDKDB &sdkdb, ObjCInterfaceRecord &base,
                           const ObjCInterfaceRecord &record);
  void updateObjCCategory(SDKDB &sdkdb, ObjCCategoryRecord &base,
                          const ObjCCategoryRecord &record);
  void updateObjCProtocol(SDKDB &sdkdb, ObjCProtocolRecord &base,
                          const ObjCProtocolRecord &record);

  clang::DiagnosticBuilder report(unsigned diagID) {
    return diag.report(diagID);
  }

  const llvm::SmallVectorImpl<SDKDB> &getDatabases() const {
    return databases;
  }

  bool isMaybePublicSelector(StringRef selector) const {
    return maybePublicSelector.count(selector);
  }

  void addProjectWithError(StringRef project) {
    projectWithError.emplace_back(project.data(), project.size());
  }

  const std::vector<std::string> getProjectWithError() const {
    return projectWithError;
  }

  bool isPublicOnly() const {
    return (bool)(options & SDKDBBuilderOptions::isPublicOnly);
  }

  bool noObjCMetadata() const {
    return ((bool)(options & SDKDBBuilderOptions::noObjCMetadata));
  }

  bool hasUUID() const {
    return (bool)(options & SDKDBBuilderOptions::hasUUID);
  }

  bool preserveLocation() const {
    return (bool)(options & SDKDBBuilderOptions::preserveSourceLoc);
  }

  void setBuilderOptions(SDKDBBuilderOptions opts) { options = opts; }
  uint16_t getRawOptionEncoding() const { return (uint16_t)options; }

  void setRemoveObjCMetadata() {
    options |= SDKDBBuilderOptions::noObjCMetadata;
  }

  bool excludeBundles() const {
    return (bool)(options & SDKDBBuilderOptions::excludeBundles);
  }

  void setRemoveBundles() {
    options |= SDKDBBuilderOptions::excludeBundles;
  }

  bool compare(const SDKDBBuilder &baseline);

private:
  DiagnosticsEngine &diag;
  SDKDBBuilderOptions options;
  std::string buildVersion;
  llvm::SmallVector<SDKDB, 4> databases;
  llvm::StringSet<> maybePublicSelector;
  // Projects that contributes to the SDKDB but has errors when scanning roots.
  std::vector<std::string> projectWithError;
};

TAPI_NAMESPACE_INTERNAL_END

#endif
