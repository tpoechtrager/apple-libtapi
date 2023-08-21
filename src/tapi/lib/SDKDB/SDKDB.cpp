//===- tapi/SDKDB/SDKDB.cpp - TAPI SDKDB ------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the SDKDB
///
//===----------------------------------------------------------------------===//

#include "tapi/SDKDB/SDKDB.h"

#include "tapi/Core/APIVisitor.h"
#include "tapi/Core/APIJSONSerializer.h"
#include "tapi/Core/Utils.h"
#include "llvm/Support/JSON.h"

using namespace llvm;

TAPI_NAMESPACE_INTERNAL_BEGIN

class LookupMapBuilder : public APIMutator {
public:
  LookupMapBuilder(SDKDB &sdkdb) : sdkdb(sdkdb) {}

  void visitGlobal(GlobalRecord &record) override {
    sdkdb.insertGlobal(&record);
  }
  void visitObjCInterface(ObjCInterfaceRecord &record) override {
    sdkdb.insertObjCInterface(&record);
  }
  void visitObjCCategory(ObjCCategoryRecord &record) override {
    sdkdb.insertObjCCategory(&record);
  }
  void visitObjCProtocol(ObjCProtocolRecord &record) override {
    sdkdb.insertObjCProtocol(&record);
  }

private:
  SDKDB &sdkdb;
};

class APIAnnotator : public APIVisitor {
public:
  APIAnnotator(SDKDB &sdkdb) : sdkdb(sdkdb) {}

  void visitGlobal(const GlobalRecord &record) override {
    sdkdb.annotateGlobal(&record);
  }
  void visitObjCInterface(const ObjCInterfaceRecord &record) override {
    sdkdb.annotateObjCInterface(&record);
  }
  void visitObjCCategory(const ObjCCategoryRecord &record) override {
    sdkdb.annotateObjCCategory(&record);
  }
  void visitObjCProtocol(const ObjCProtocolRecord &record) override {
    sdkdb.annotateObjCProtocol(&record);
  }

private:
  SDKDB &sdkdb;
};

API &SDKDB::recordAPI(API &&api) {
  apiCache.emplace_back(std::move(api));
  return apiCache.back();
}

void SDKDB::insertGlobal(GlobalRecord *record) {
  // only insert exported and re-exported symbols into map.
  if (record->linkage < APILinkage::Reexported)
    return;

  auto key = record->name;
  auto &value = globalMap[key];
  value.insert(record);

  // Check and ignore duplicated symbol.
  // FIXME: duplicated weak symbols?
  if (value.size() > 1 && !record->isWeakDefined())
    builder->report(diag::warn_sdkdb_duplicated_global) << key;
}

void SDKDB::insertObjCInterface(ObjCInterfaceRecord *record) {
  auto key = record->name;
  auto value = interfaceMap.insert({key, record});

  // Check for duplicated base class. It is a warning for now..
  if (!value.second)
    builder->report(diag::warn_sdkdb_duplicated_objc_class) << key;
}

void SDKDB::insertObjCCategory(ObjCCategoryRecord *record) {
  auto &catMap = categoryMap[record->interfaceName];

  auto value = catMap.insert({record->name, record});
  if (!value.second)
    builder->report(diag::warn_sdkdb_duplicated_objc_category)
        << record->interfaceName << record->name;
}

static ObjCMethodRecord *findMethodFromContainer(StringRef name,
                                                 bool isInstanceMethod,
                                                 ObjCContainerRecord &record) {
  auto result = find_if(record.methods, [&](ObjCMethodRecord *method) {
    return method->name == name && method->isInstanceMethod == isInstanceMethod;
  });
  if (result == record.methods.end())
    return nullptr;
  else
    return *result;
}

static ObjCPropertyRecord *findProperty(StringRef name,
                                        ObjCContainerRecord &record) {
  auto result = find_if(record.properties, [&](ObjCPropertyRecord *property) {
    return property->name == name;
  });
  if (result == record.properties.end())
    return nullptr;
  else
    return *result;
}

void SDKDB::insertObjCProtocol(ObjCProtocolRecord *record) {
  auto key = record->name;
  auto value = protocolMap.insert({key, record});

  if (!value.second) {
    // If there exists protocol definition already, check they are the same.
    // no additional methods.
    auto *base = value.first->getValue();
    for (auto *method : record->methods) {
      if (!findMethod(method->name, method->isInstanceMethod, *base,
                      SDKDB::ObjCProtocol)) {
        builder->report(diag::warn_sdkdb_conflict_objc_protocol) << key;
        return;
      }
    }
    // no additional protocol conformance.
    for (auto protocolName : record->protocols) {
      if (std::find(base->protocols.begin(), base->protocols.end(),
                    protocolName) == std::end(base->protocols)) {
        builder->report(diag::warn_sdkdb_conflict_objc_protocol) << key;
        return;
      }
    }
  }
}

void SDKDB::annotateGlobal(const GlobalRecord *record) {
  if (record->linkage < APILinkage::Reexported)
    return;

  auto entry = globalMap.find(record->name);
  if (entry == globalMap.end()) {
    if (!record->availability._unavailable)
      builder->report(diag::warn_sdkdb_missing_global) << record->name;
    return;
  }

  for (auto base: entry->getValue())
    builder->updateGlobal(*base, *record);
}

void SDKDB::annotateObjCInterface(const ObjCInterfaceRecord *record) {
  auto entry = interfaceMap.find(record->name);
  if (entry == interfaceMap.end()) {
    if (!record->availability._unavailable)
      builder->report(diag::warn_sdkdb_missing_objc_class) << record->name;
    return;
  }

  auto *base = entry->getValue();
  builder->updateObjCInterface(*this, *base, *record);

  // Update symbols for objc interfaces.
  if (!record->isExported())
    return;
  auto name = record->name;
  if (isObjC1())
    findAndUpdateGlobal(".objc_class_name_" + name, *record);
  else {
    findAndUpdateGlobal("_OBJC_CLASS_$_" + name, *record);
    findAndUpdateGlobal("_OBJC_METACLASS_$_" + name, *record);
    if (record->hasExceptionAttribute)
      findAndUpdateGlobal("_OBJC_EHTYPE_$_" + name, *record);
  }

  // sync ivars. ivars are not recorded in objc containers. sync ivars with
  // the symbols.
  for (auto *ivar : record->ivars) {
    if (!isObjC1() &&
        (ivar->accessControl ==
             ObjCInstanceVariableRecord::AccessControl::Public ||
         ivar->accessControl ==
             ObjCInstanceVariableRecord::AccessControl::Protected))
      findAndUpdateGlobal("_OBJC_IVAR_$_" + name + "." + ivar->name, *record);
  }
}

void SDKDB::annotateObjCProtocol(const ObjCProtocolRecord *record) {
  auto entry = protocolMap.find(record->name);
  if (entry == protocolMap.end()) {
    if (!record->availability._unavailable)
      builder->report(diag::warn_sdkdb_missing_objc_protocol) << record->name;
    return;
  }

  auto *base = entry->getValue();
  builder->updateObjCProtocol(*this, *base, *record);
}

void SDKDB::annotateObjCCategory(const ObjCCategoryRecord *record) {
  // For categories, search for category in the binary first.
  auto entry = categoryMap.find(record->interfaceName);
  if (entry != categoryMap.end()) {
    auto cat = entry->getValue().find(record->name);
    if (cat != entry->getValue().end()) {
      builder->updateObjCCategory(*this, *cat->getValue(), *record);
      return;
    }
  }

  // If the category is not found from mapping, annotate the base class
  // because linker might merge the category into the base class already.
  auto cls = interfaceMap.find(record->interfaceName);
  if (cls == interfaceMap.end()) {
    if (!record->availability._unavailable)
      builder->report(diag::warn_sdkdb_missing_objc_category)
          << record->interfaceName << record->name;
    return;
  }
  builder->updateObjCContainer(*this, *cls->getValue(), *record,
                               SDKDB::ObjCClass);

  // sync ivars. ivars are not recorded in objc containers. sync ivars with
  // the symbols.
  for (auto *ivar : record->ivars) {
    if (!isObjC1() &&
        (ivar->accessControl ==
             ObjCInstanceVariableRecord::AccessControl::Public ||
         ivar->accessControl ==
             ObjCInstanceVariableRecord::AccessControl::Protected))
      findAndUpdateGlobal(
          "_OBJC_IVAR_$_" + record->interfaceName + "." + ivar->name, *record);
  }
}

class APIFinalizer : public APIMutator {
public:
  APIFinalizer(SDKDBBuilder *builder,
               const llvm::StringMap<ObjCProtocolRecord *> &protocolMap)
      : builder(builder), protocolMap(protocolMap) {}

  void visitObjCProtocol(ObjCProtocolRecord &record) override {
    auto protocol = protocolMap.find(record.name);
    assert(protocol != protocolMap.end() &&
           "Procotol must be in the map already");
    // Patch up protocol list from all the binaries.
    builder->updateAPIRecord(record, *protocol->getValue());
    for (auto *method : record.methods) {
      if (auto *result = findMethodFromContainer(
              method->name, method->isInstanceMethod, *protocol->getValue()))
        builder->updateAPIRecord(*method, *result);
    }
    for (auto *prop : record.properties) {
      if (auto *property = findProperty(prop->name, *protocol->getValue()))
        builder->updateAPIRecord(*prop, *property);
    }
  }

private:
  SDKDBBuilder *builder;
  const llvm::StringMap<ObjCProtocolRecord *> &protocolMap;
};

Error SDKDB::finalize() {
  // Finalize SDKDB.
  // Update the access of methods to public if there exists super class/protocol
  // which declares the method to be public.
  // 1. Update Protocol methods.
  for (auto &entry : protocolMap) {
    auto *protocol = entry.getValue();
    for (auto *method : protocol->methods) {
      if (method->access != APIAccess::Public &&
          builder->isMaybePublicSelector(method->name))
        method->access = getAccessForObjCMethod(
            method->access, method->name, method->isInstanceMethod, protocol);
    }
  }

  // 2. Update Interface method.
  for (auto &entry : interfaceMap) {
    auto *interface = entry.getValue();
    for (auto *method : interface->methods) {
      if (method->access != APIAccess::Public &&
          builder->isMaybePublicSelector(method->name))
        method->access = getAccessForObjCMethod(
            method->access, method->name, method->isInstanceMethod, interface);
    }
  }

  // 3. Update Category method.
  for (auto &catEntry : categoryMap) {
    auto categories = catEntry.getValue();
    ObjCInterfaceRecord *interface = nullptr;
    auto cls = interfaceMap.find(catEntry.getKey());
    if (cls != interfaceMap.end())
      interface = cls->getValue();
    for (auto &entry : categories) {
      auto *category = entry.getValue();
      for (auto *method : category->methods) {
        if (method->access != APIAccess::Public &&
          builder->isMaybePublicSelector(method->name)) {
          method->access =
              getAccessForObjCMethod(method->access, method->name,
                                     method->isInstanceMethod, category);
          // Look at base class if exists.
          if (method->access != APIAccess::Public && interface)
            method->access =
                getAccessForObjCMethod(method->access, method->name,
                                       method->isInstanceMethod, interface);
        }
      }
    }
  }

  // Perform SDKDB finalize.
  // 1. Fixup all the protocols in the SDKDB.
  // 2. Apply dylib promote to promote to public listt to the entry.
  for (auto &api: apiCache) {
    if (api.getBinaryInfo().fileType == FileType::MachO_Bundle)
      continue;

    APIFinalizer updater(builder, protocolMap);
    api.visit(updater);
  }

  return Error::success();
}

ObjCInterfaceRecord *SDKDB::getSuperclass(const ObjCInterfaceRecord *record) {
  if (record->superClassName.empty())
    return nullptr;
  auto super = interfaceMap.find(record->superClassName);
  if (super == interfaceMap.end())
    return nullptr;
  return super->getValue();
}

APIAccess SDKDB::getAccessForObjCMethod(APIAccess access, StringRef name,
                                        bool isInstanceMethod,
                                        ObjCContainerRecord *container) {
  // check current container for the access.
  for (auto *method : container->methods) {
    if (method->name == name && method->isInstanceMethod == isInstanceMethod)
      access = method->access > access ? method->access : access;
  }
  if (access == APIAccess::Public)
    return access; // return since it is already public.

  // walk protocol hierarchy.
  for (auto protocolName : container->protocols) {
    auto protocol = protocolMap.find(protocolName);
    if (protocol != protocolMap.end())
      access = getAccessForObjCMethod(access, name, isInstanceMethod,
                                      protocol->getValue());
    if (access == APIAccess::Public)
      return access;
  }

  return access;
}

APIAccess SDKDB::getAccessForObjCMethod(APIAccess access, StringRef name,
                                        bool isInstanceMethod,
                                        ObjCInterfaceRecord *interface) {
  // walk the common container part first.
  access = getAccessForObjCMethod(access, name, isInstanceMethod,
                                  (ObjCContainerRecord *)interface);
  if (access == APIAccess::Public)
    return access; // return since it is already public.

  // check super class.
  if (auto *super = getSuperclass(interface))
    access = getAccessForObjCMethod(access, name, isInstanceMethod, super);
  if (access == APIAccess::Public)
    return access; // return since it is already public.

  // check category. category should not overwrite the methods from interface
  // but it can introduce new procotol conformance.
  auto categories = categoryMap.find(interface->name);
  if (categories == categoryMap.end())
    return access;

  for (auto &category : categories->getValue()) {
    for (auto protocolName : category.getValue()->protocols) {
      auto protocol = protocolMap.find(protocolName);
      if (protocol != protocolMap.end())
        access = getAccessForObjCMethod(access, name, isInstanceMethod,
                                        protocol->getValue());
      if (access == APIAccess::Public)
        return access;
    }
  }

  return access;
}

bool SDKDB::findAndUpdateGlobal(Twine name, const APIRecord &record) {
  auto sym = globalMap.find(name.str());
  if (sym == globalMap.end())
    return false;
  for (auto value: sym->getValue())
    builder->updateAPIRecord(*value, record);
  return true;
}

ObjCMethodRecord *SDKDB::findMethod(StringRef name, bool isInstanceMethod,
                                    ObjCContainerRecord &record,
                                    ObjCContainerKind kind,
                                    StringRef fallbackInterfaceName) {
  auto *result = findMethodFromContainer(name, isInstanceMethod, record);
  if (result)
    return result;

  if (kind == SDKDB::ObjCCategory) {
    // Find the base class and recursively search base class.
    auto cls = interfaceMap.find(fallbackInterfaceName);
    if (cls != interfaceMap.end())
      return findMethod(name, isInstanceMethod, *(cls->getValue()),
                        SDKDB::ObjCClass);
  } else if (kind == SDKDB::ObjCClass) {
    // Only do global search for objc classes.
    auto categories = categoryMap.find(record.name);
    if (categories == categoryMap.end())
      return result;

    for (auto &category : categories->getValue()) {
      result = findMethodFromContainer(name, isInstanceMethod,
                                       *(category.getValue()));
      if (result)
        return result;
    }
  }

  return nullptr;
}

Error SDKDBBuilder::addBinaryAPI(API &&api) {
  auto &db = getSDKDBForTarget(api.getTarget());
  auto &current = db.recordAPI(std::move(api));

  // No need to put bundle into lookup map.
  if (api.getBinaryInfo().fileType == FileType::MachO_Bundle)
    return Error::success();

  // Build lookup map.
  LookupMapBuilder builder(db);
  current.visit(builder);

  return Error::success();
}

Error SDKDBBuilder::addHeaderAPI(const API &api) {
  auto &db = getSDKDBForTarget(api.getTarget());
  APIAnnotator annotator(db);
  api.visit(annotator);

  return Error::success();
}

void SDKDBBuilder::updateAPIRecord(APIRecord &base, const APIRecord &record) {
  // update the APLoc if the original location is invalid.
  // also moving away from clang::PresumedLoc because it is not in the same
  // context.
  if (base.loc.isInvalid() && preserveLocation())
    base.loc = APILoc(record.loc.getFilename(), record.loc.getLine(),
                      record.loc.getColumn());

  if (base.availability.isDefault())
    base.availability = record.availability;
  else if (base.availability != record.availability &&
           !record.availability.isDefault()) {
    diag.report(diag::warn_sdkdb_conflict_availability)
        << base.name << base.availability.str() << record.availability.str();
    // Pick the newer availability to be consistent.
    if (base.availability < record.availability)
      base.availability = record.availability;
  }

  if (base.access < record.access)
    base.access = record.access;
}

void SDKDBBuilder::updateObjCContainer(SDKDB &sdkdb, ObjCContainerRecord &base,
                                       const ObjCContainerRecord &record,
                                       SDKDB::ObjCContainerKind kind,
                                       StringRef fallbackInterfaceName) {
  updateAPIRecord(base, record);

  auto handleMissingMethod = [&](StringRef selectorName,
                                 APIRecord &selectorInfo, bool isInstanceMethod,
                                 bool isOptional, bool isDynamic) {
    diag.report(diag::warn_sdkdb_missing_objc_method)
        << selectorName << kind << record.name;

    APILoc location;
    if (preserveLocation())
      location = selectorInfo.loc;
    auto *m = ObjCMethodRecord::create(
        sdkdb.danglingAPIAllocator, selectorName, location,
        selectorInfo.availability, selectorInfo.access, isInstanceMethod,
        isOptional, isDynamic, selectorInfo.decl);
    base.methods.push_back(m);
  };

  for (auto *method : record.methods) {
    if (auto *baseMethod =
            sdkdb.findMethod(method->name, method->isInstanceMethod, base, kind,
                             fallbackInterfaceName))
      updateAPIRecord(*baseMethod, *method);
    else if (!method->availability._unavailable)
      handleMissingMethod(method->name, *method, method->isInstanceMethod,
                          method->isOptional, method->isDynamic);

    if (method->access == APIAccess::Public)
      maybePublicSelector.insert(method->name);
  }

  // base does not have properties because it is coming from binary interface.
  // use the property in header to annotate getter and setter inside base.
  for (auto *prop : record.properties) {
    bool available = !prop->availability._unavailable;
    // dynamic is only known to implementation. Default to false.
    bool isDynamic = false;
    // use the class property attributes from header.
    bool isClassProperty = prop->isClassProperty();
    if (auto *property = findProperty(prop->name, base)) {
      updateAPIRecord(*property, *prop);
      isDynamic = property->isDynamic();
    } else if (available)
      diag.report(diag::warn_sdkdb_missing_objc_property)
          << prop->name << kind << base.name;

    // dynamic property doesn't have synthesized methods in class.
    if (isDynamic)
      continue;

    if (auto *baseMethod = sdkdb.findMethod(prop->getterName,
                                            /*instanceMethod*/ !isClassProperty,
                                            base, kind, fallbackInterfaceName))
      updateAPIRecord(*baseMethod, *prop);
    else if (!prop->isOptional && available)
      handleMissingMethod(prop->getterName, *prop,
                          /* isInstanceMethod */ !isClassProperty,
                          /* isOptional */ false, /* isDynamic */ false);

    if (prop->access == APIAccess::Public)
      maybePublicSelector.insert(prop->getterName);

    if (prop->isReadOnly())
      continue;

    if (auto *baseMethod = sdkdb.findMethod(prop->setterName,
                                            /*instanceMethod*/ !isClassProperty,
                                            base, kind, fallbackInterfaceName))
      updateAPIRecord(*baseMethod, *prop);
    else if (!prop->isOptional && available)
      handleMissingMethod(prop->setterName, *prop,
                          /* isInstanceMethod */ !isClassProperty,
                          /* isOptional */ false, /* isDynamic */ false);

    if (prop->access == APIAccess::Public)
      maybePublicSelector.insert(prop->setterName);
  }

  // TODO: Now we just sync all the conformed protocol from header to API.
  // Need to teach MachOReader to read them from binary in the future.
  for (auto protocol : record.protocols) {
    if (std::find(base.protocols.begin(), base.protocols.end(), protocol) ==
        std::end(base.protocols))
      base.protocols.push_back(protocol);
  };
}

void SDKDBBuilder::updateGlobal(GlobalRecord &base,
                                const GlobalRecord &record) {
  updateAPIRecord(base, record);

  if (base.kind == GVKind::Unknown)
    base.kind = record.kind;
  else if (base.kind != record.kind)
    diag.report(diag::warn_sdkdb_conflict_gvkind) << base.name;
}

void SDKDBBuilder::updateObjCInterface(SDKDB &sdkdb, ObjCInterfaceRecord &base,
                                       const ObjCInterfaceRecord &record) {
  updateObjCContainer(sdkdb, base, record, SDKDB::ObjCClass);
  if (base.superClassName != record.superClassName)
    diag.report(diag::warn_sdkdb_conflict_superclass)
        << base.name << base.superClassName << record.superClassName;
  base.hasExceptionAttribute |= record.hasExceptionAttribute;
}

void SDKDBBuilder::updateObjCCategory(SDKDB &sdkdb, ObjCCategoryRecord &base,
                                      const ObjCCategoryRecord &record) {
  updateObjCContainer(sdkdb, base, record, SDKDB::ObjCCategory,
                      record.interfaceName);
}

void SDKDBBuilder::updateObjCProtocol(SDKDB &sdkdb, ObjCProtocolRecord &base,
                                      const ObjCProtocolRecord &record) {
  updateObjCContainer(sdkdb, base, record, SDKDB::ObjCProtocol);
}

SDKDB &SDKDBBuilder::getSDKDBForTarget(const Triple &triple) {
  for (auto &entry : databases) {
    if (entry.getTargetTriple() == triple)
      return entry;
  }
  databases.emplace_back(triple, this);
  return databases.back();
}

Error SDKDBBuilder::finalize() {
  for (auto &entry : databases) {
    if (auto err = entry.finalize())
      return err;
  }
  return Error::success();
}

llvm::Error SDKDBBuilder::parse(StringRef JSON) {
  auto inputValue = json::parse(JSON);
  if (!inputValue)
    return inputValue.takeError();

  auto *root = inputValue->getAsObject();
  if (!root)
    return make_error<APIJSONError>("SDKDB is not a JSON Object");

  for (auto &target : *root) {
    auto triple = Triple(target.first.str());
    auto payload = target.second.getAsArray();
    if (!payload)
      return make_error<APIJSONError>("Target Payload is not a JSON Array");

    for (auto &value : *payload) {
      auto obj = value.getAsObject();
      if (!obj)
        return make_error<APIJSONError>(
            "SDKDB doesn't include correct API format");
      auto api = APIJSONSerializer::parse(obj, isPublicOnly(), &triple);
      if (!api)
        return api.takeError();

      if (auto err = addBinaryAPI(std::move(*api)))
        return err;
    }
  }

  return Error::success();
}

void SDKDBBuilder::serialize(raw_ostream &os, bool compact) const {
  json::Object root;
  APIJSONOption serializeOpts = {compact,
                                 !hasUUID(),
                                 /*no target*/ true,
                                 /*external only*/ true,
                                 isPublicOnly(),
                                 /*ignore line and col*/ true};

  for (auto &entry : databases) {
    auto &target = entry.getTargetTriple();
    json::Array apiList;
    for (auto &api : entry.api()) {
      APIJSONSerializer serializer(api, serializeOpts);
      apiList.emplace_back(serializer.getJSONObject());
    }
    root[target.normalize()] = std::move(apiList);
  }
  if (isPublicOnly())
    root["public"] = true;

  json::Array projects;
  for (auto &proj : projectWithError)
    projects.push_back(proj);
  if (!projects.empty())
    root["projectWithError"] = std::move(projects);

  if (compact)
    os << formatv("{0}", json::Value(std::move(root))) << "\n";
  else
    os << formatv("{0:2}", json::Value(std::move(root))) << "\n";
}

class APICompare : public APIVisitor {
public:
  APICompare(const Triple &target, StringRef installName, const API &api,
             SDKDBBuilder &builder)
      : target(target), installName(installName), api(api), builder(builder) {}

  void visitGlobal(const GlobalRecord &record) override {
    if (record.access != APIAccess::Public)
      return;

    const GlobalRecord *global = nullptr;
    if (record.kind == GVKind::Variable)
      global = api.findGlobalVariable(record.name);
    else if (record.kind == GVKind::Function)
      global = api.findFunction(record.name);
    else
      return;

    if (!global) {
      builder.report(diag::err_sdkdb_missing_global)
          << (unsigned)record.kind << record.name << installName
          << target.str();
      regression = true;
      return;
    }

    if (!checkAPIRecord(*global, record, [&](StringRef error) {
          builder.report(diag::err_sdkdb_global_regression)
              << record.name << installName << target.str() << error;
        }))
      regression = true;
  }

  void visitObjCInterface(const ObjCInterfaceRecord &record) override {
    if (record.access != APIAccess::Public)
      return;

    auto *cls = api.findObjCInterface(record.name);
    if (!cls) {
      builder.report(diag::err_sdkdb_missing_objc)
          << 0 << record.name << installName << target.str();
      regression = true;
      return;
    }
    if (!checkObjCContainer(
            *cls, record,
            [&](StringRef error) {
              builder.report(diag::err_sdkdb_objc_container_regression)
                  << 0 << record.name << installName << target.str() << error;
            },
            [&](StringRef selector, StringRef error) {
              builder.report(diag::err_sdkdb_objc_selector_regression)
                  << selector << 0 << record.name << installName << target.str()
                  << error;
            }))
      regression = true;
  }

  void visitObjCCategory(const ObjCCategoryRecord &record) override {
    if (record.access != APIAccess::Public)
      return;

    auto *cat = api.findObjCCategory(record.interfaceName, record.name);
    std::string name =
        record.name.str() + "(" + record.interfaceName.str() + ")";
    if (!cat) {
      builder.report(diag::err_sdkdb_missing_objc)
          << 1 << name << installName << target.str();
      regression = true;
      return;
    }
    if (!checkObjCContainer(
            *cat, record,
            [&](StringRef error) {
              builder.report(diag::err_sdkdb_objc_container_regression)
                  << 1 << name << installName << target.str() << error;
            },
            [&](StringRef selector, StringRef error) {
              builder.report(diag::err_sdkdb_objc_selector_regression)
                  << selector << 1 << name << installName << target.str()
                  << error;
            }))
      regression = true;
  }

  void visitObjCProtocol(const ObjCProtocolRecord &record) override {
    if (record.access != APIAccess::Public)
      return;

    auto *proto = api.findObjCProtocol(record.name);
    if (!proto) {
      builder.report(diag::err_sdkdb_missing_objc)
          << 2 << record.name << installName << target.str();
      regression = true;
      return;
    }
    if (!checkObjCContainer(
            *proto, record,
            [&](StringRef error) {
              builder.report(diag::err_sdkdb_objc_container_regression)
                  << 2 << record.name << installName << target.str() << error;
            },
            [&](StringRef selector, StringRef error) {
              builder.report(diag::err_sdkdb_objc_selector_regression)
                  << selector << 2 << record.name << installName << target.str()
                  << error;
            }))
      regression = true;
  }

  bool hasRegression() const { return regression; }

private:
  bool checkAPIRecord(const APIRecord &record, const APIRecord &base,
                      std::function<void(StringRef)> handler) const {
    assert(record.name == base.name && "record names are not equal");
    if (base.access != APIAccess::Public)
      return true; // allow regression for non-public API.

    if (record.access != APIAccess::Public) {
      handler("api access regression");
      return false;
    }

    if (record.linkage != base.linkage) {
      handler("linkage type is not equal");
      return false;
    }

    if (!base.availability.isDefault() &&
        record.availability != base.availability) {
      handler("availability changed");
      return false;
    }
    return true;
  }

  bool checkObjCContainer(
      const ObjCContainerRecord &record, const ObjCContainerRecord &base,
      std::function<void(StringRef)> handlerForContainer,
      std::function<void(StringRef, StringRef)> handlerForSelector) const {
    bool regression = checkAPIRecord(record, base, handlerForContainer);
    for (auto *method : base.methods) {
      if (method->access != APIAccess::Public)
        continue;

      auto m = llvm::find_if(record.methods, [&](ObjCMethodRecord *record) {
        return record->name == method->name &&
               record->isInstanceMethod == method->isInstanceMethod;
      });
      if (m == record.methods.end()) {
        handlerForSelector(method->name, "selector is missing");
        regression = true;
        continue;
      }

      regression &= checkAPIRecord(**m, *method, [&](StringRef error) {
        handlerForSelector(method->name, error);
      });
    }
    return regression;
  }

  const Triple target;
  StringRef installName;
  const API &api;
  SDKDBBuilder &builder;
  bool regression = false;
};

bool SDKDBBuilder::compare(const SDKDBBuilder &baseline) {
  bool equal = true;
  for (const auto &base : baseline.databases) {
    // Check to see if the target exists.
    auto db = llvm::find_if(
        databases, [&](const SDKDB &db) { return db.triple == base.triple; });
    if (db == databases.end()) {
      report(diag::err_sdkdb_missing_target) << base.triple.str();
      equal = false;
      continue;
    }
    // Diff APIs.
    for (auto &dylib : base.apiCache) {
      if (!dylib.hasBinaryInfo()) {
        report(diag::err_sdkdb_missing_binary_info) << base.triple.str();
        continue;
      }
      auto installName = dylib.getBinaryInfo().installName;
      // skip empty install_name.
      if (installName.empty())
        continue;

      // Look for the dylib of the same installName.
      auto api = llvm::find_if(db->apiCache, [&](const API &api) {
        if (api.hasBinaryInfo())
          return api.getBinaryInfo().installName == installName;
        return false;
      });
      if (api == db->apiCache.end()) {
        if (!installName.startswith("@") && isPublicDylib(installName)) {
          report(diag::err_sdkdb_missing_dylib)
              << dylib.getBinaryInfo().installName << base.triple.str();
          equal = false;
        }
        continue;
      }
      APICompare comp(base.triple, dylib.getBinaryInfo().installName, *api,
                      *this);
      dylib.visit(comp);
      equal &= !comp.hasRegression();
    }
  }
  return equal;
}


TAPI_NAMESPACE_INTERNAL_END
