//===- lib/Driver/SDKDBDriver.cpp - TAPI SDKDB Driver -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the SDKDB driver for the tapi tool.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/API.h"
#include "tapi/Core/APIJSONSerializer.h"
#include "tapi/Core/Context.h"
#include "tapi/Core/FileSystem.h"
#include "tapi/Core/Framework.h"
#include "tapi/Core/HeaderFile.h"
#include "tapi/Core/MachOReader.h"
#include "tapi/Core/Path.h"
#include "tapi/Core/Registry.h"
#include "tapi/Core/Utils.h"
#include "tapi/Defines.h"
#include "tapi/Diagnostics/Diagnostics.h"
#include "tapi/Driver/Configuration.h"
#include "tapi/Driver/ConfigurationFileReader.h"
#include "tapi/Driver/DirectoryScanner.h"
#include "tapi/Driver/Driver.h"
#include "tapi/Driver/DriverUtils.h"
#include "tapi/Driver/HeaderGlob.h"
#include "tapi/Driver/Options.h"
#include "tapi/Driver/StatRecorder.h"
#include "tapi/Frontend/Frontend.h"
#include "tapi/SDKDB/PartialSDKDB.h"
#include "tapi/SDKDB/SDKDB.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/FileUtilities.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Process.h"
#include "llvm/Support/VirtualFileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/TextAPI/MachO/ArchitectureSet.h"
#include <system_error>

using namespace llvm;
using namespace llvm::MachO;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

// Scan Driver Context.
namespace sdkdb {

class APIPromoter : public APIMutator {
public:
  APIPromoter() = default;

  void visitGlobal(GlobalRecord &record) override {
    // If this a promoted APISet, make all exported symbols to be public.
    if (record.isExported())
      record.access = APIAccess::Public;
  }

  void visitObjCInterface(ObjCInterfaceRecord &record) override {
    // If this a promoted APISet, make all objc classes to be public.
    record.access = APIAccess::Public;
    updateObjCContainerRecordToPublic(record);
  }

  void visitObjCCategory(ObjCCategoryRecord &record) override {
    record.access = APIAccess::Public;
    updateObjCContainerRecordToPublic(record);
  }

  void updateObjCContainerRecordToPublic(ObjCContainerRecord &record) {
    for (auto *method : record.methods)
      method->access = APIAccess::Public;
  }
};

class Context : public tapi::internal::Context {
public:
  // Make the context not copyable.
  Context(const Context &) = delete;
  Context &operator=(const Context &) = delete;

  Configuration config;
  Registry registry;
  std::vector<API> publicBinaryResults;
  std::vector<API> internalBinaryResults;
  std::vector<FrontendContext> publicSDKResults;
  std::vector<FrontendContext> internalSDKResults;
  std::vector<API> extraPublicSDKResults;
  std::vector<API> extraInternalSDKResults;
  DirectoryScanner::FileMap publicVFSOverlay;
  DirectoryScanner::FileMap internalVFSOverlay;
  SDKDBAction action;
  std::string outputPath;
  std::string installAPISDKDBPath;
  std::string internalSDKPath;
  std::string publicSDKPath;
  std::string partialSDKDBFilelist;
  std::string diagnosticsFile;
  PathSeq systemIncludePaths;
  PathSeq systemFrameworkPaths;
  bool verbose;
  PlatformKind platform;
  std::string version;
  bool verifyAPI;
  bool verifyAPISkipExternalHeaders;
  std::string verifyAllowlistFileName;
  std::unique_ptr<MemoryBuffer> verifyAllowlist;

  std::string projectName;
  bool hasSDKDBError = false;
  bool hasWrittenPartialOutput = false;
  bool scannedSwiftInterface = false;

  Context(Options &opt, DiagnosticsEngine &diag)
      : tapi::internal::Context(opt, diag), config(*this)  {
    registry.addYAMLReaders();
    outputPath = opt.driverOptions.outputPath;
    installAPISDKDBPath = opt.sdkdbOptions.installAPISDKDBDirectory;
    internalSDKPath = opt.sdkdbOptions.sdkContentRoot;
    publicSDKPath = opt.sdkdbOptions.publicSDKContentRoot;
    partialSDKDBFilelist = opt.sdkdbOptions.partialSDKDBFileList;
    action = opt.sdkdbOptions.action;
    diagnosticsFile = opt.sdkdbOptions.diagnosticsFile;
    verbose = opt.frontendOptions.verbose;
    // infer the triples from environment.
    StringRef versionStr;
    auto versionMinArg = sys::Process::GetEnv("RC_MIN_VERSION_OPTION");
    if (versionMinArg) {
      versionStr = *versionMinArg;
      version = versionStr.split('=').second.str();
    }
    verifyAPI = opt.tapiOptions.verifyAPI;
    verifyAPISkipExternalHeaders = opt.tapiOptions.verifyAPISkipExternalHeaders;

    if (!opt.tapiOptions.verifyAPIAllowlist.empty()) {
      verifyAllowlistFileName = opt.tapiOptions.verifyAPIAllowlist;
      auto inputBuf =
          MemoryBuffer::getFile(opt.tapiOptions.verifyAPIAllowlist);
      // load the file if it can be opened, otherwise, just ignore.
      if (inputBuf)
        verifyAllowlist = std::move(*inputBuf);
    }

    systemIncludePaths = opt.frontendOptions.systemIncludePaths;
    systemFrameworkPaths =
        getAllPaths(opt.frontendOptions.systemFrameworkPaths);
    // set project name.
    auto project = llvm::sys::Process::GetEnv("RC_ProjectName");
    if (!project)
      return;
    projectName = *project;
    config.setProjectName(projectName);
  }

  Error scanBinaryFile(StringRef path, std::vector<Triple> &triples,
                       bool isPublic) {
    if (!_fm->exists(path))
      return make_error<StringError>(
          "binary file doesn't exist", inconvertibleErrorCode());

    auto bufferOrErr = _fm->getBufferForFile(path);
    if (auto ec = bufferOrErr.getError())
      return errorCodeToError(ec);

    MachOParseOption option;
    option.arches = config.getArchitectures();
    auto results = readMachOFile(bufferOrErr->get()->getMemBufferRef(), option);
    if (!results) {
      getDiag().report(diag::warn_sdkdb_skip_file)
          << path << toString(results.takeError());
      return Error::success();
    }

    for (auto &result : *results) {
      const auto &target = result.second.getTarget();
      if (std::find(triples.begin(), triples.end(), target) ==
          std::end(triples))
        triples.push_back(target);

      // Update all interfaces to public if configuration suggests.
      if (config.isPromotedToPublicDylib(
              result.second.getBinaryInfo().installName)) {
        APIPromoter promoter;
        result.second.visit(promoter);
      }

      if (isPublic)
        publicBinaryResults.emplace_back(std::move(result.second));
      else
        internalBinaryResults.emplace_back(std::move(result.second));
    }

    return Error::success();
  }

  Error performOutput(StringRef path,
                      const std::function<Error(raw_ostream &)> &func) const {
    if (outputPath.empty())
      return make_error<StringError>(
          "output path is empty",
          std::make_error_code(std::errc::invalid_argument));

    // if outputPath is "-" redirect all to stdout.
    if (outputPath == "-")
      return func(outs());

    // Otherwise, outputPath is a directory.
    std::error_code err;
    if (!sys::fs::exists(outputPath)) {
      if (auto err = sys::fs::create_directory(outputPath))
        return errorCodeToError(err);
    } else if (!sys::fs::is_directory(outputPath))
      return make_error<StringError>(
          "output path is not a directory",
          std::make_error_code(std::errc::invalid_argument));
    // create output file stream.
    SmallString<PATH_MAX> output(outputPath);
    sys::path::append(output, path);
    raw_fd_ostream os(output, err);
    if (err)
      return errorCodeToError(err);
    return func(os);
  }
};

// Helper to update APILoc to remove Root path.
class APILocUpdater : public APIMutator {
public:
  APILocUpdater(StringRef rootPath, API &api)
      : root(rootPath.str()), api(api) {}

  void visitGlobal(GlobalRecord &record) override {
    updateAPILoc(record);
  }

  void visitEnumConstant(EnumConstantRecord &record) override {
    updateAPILoc(record);
  }

  void visitObjCInterface(ObjCInterfaceRecord &record) override {
    updateAPILoc(record);
    updateContainer(record);
  }

  void visitObjCCategory(ObjCCategoryRecord &record) override {
    updateAPILoc(record);
    updateContainer(record);
  }

  void visitObjCProtocol(ObjCProtocolRecord &record) override {
    updateAPILoc(record);
    updateContainer(record);
  }

  void visitTypeDef(APIRecord & record) override {
    updateAPILoc(record);
  }

private:
  void updateAPILoc(APIRecord &record) {
    auto path = record.loc.getFilename();
    if (path.startswith(root)) {
      SmallString<PATH_MAX> newPath(path);
      sys::path::replace_path_prefix(newPath, root, "");
      auto filename = api.copyString(newPath);
      record.loc =
          APILoc(filename, record.loc.getLine(), record.loc.getColumn());
    }
  }

  void updateContainer(ObjCContainerRecord &record) {
    for (auto *method : record.methods)
      updateAPILoc(*method);
    for (auto *prop : record.properties)
      updateAPILoc(*prop);
    for (auto *ivar : record.ivars)
      updateAPILoc(*ivar);
  }
  std::string root;
  API &api;
};

} // namespace

static Expected<std::string>
findSwiftAPIExtractExecutable(DiagnosticsEngine &diag) {
  static int staticSymbol;
  // If environmental variable "_TAPI_TEST_SWIFT_API_EXTRACT" is set, just
  // return that.
  if (auto swiftFromEnv = sys::Process::GetEnv("_TAPI_TEST_SWIFT_API_EXTRACT"))
    return *swiftFromEnv;
  // Try to find swift-api-extract first in the toolchain. If that fails, then
  // fall-back to the default search PATH.
  auto mainExecutable = sys::fs::getMainExecutable("tapi", &staticSymbol);
  StringRef toolchainBinDir = sys::path::parent_path(mainExecutable);
  auto swiftBinary = sys::findProgramByName("swift-api-extract",
                                            makeArrayRef(toolchainBinDir));
  if (swiftBinary.getError()) {
    diag.report(diag::warn)
        << "cannot find 'swift-api-extract' in toolchain directory. "
           "Looking for 'swift-api-extract' in PATH instead.";
    swiftBinary = sys::findProgramByName("swift-api-extract");
    if (auto ec = swiftBinary.getError())
      return make_error<StringError>(
          "unable to find 'swift-api-extract' in PATH", ec);
  }
  return swiftBinary.get();
}

static void inferTriplesFromEnvironment(sdkdb::Context &context,
                                        std::vector<Triple> &triples) {
  if (context.platform == PlatformKind::unknown)
    return;

  auto archStr = sys::Process::GetEnv("RC_ARCHS");
  if (!archStr)
    return;
  SmallVector<StringRef, 2> archs;
  StringRef(*archStr).split(archs, ' ');
  for (auto &archName : archs) {
    auto arch = getArchitectureFromName(archName);
    if (!context.config.getArchitectures().contains(arch))
      continue;
    // FIXME: catalyst and driverkit versions are hard coded.
    if (context.config.isiOSMacProject())
      triples.emplace_back(getArchitectureName(arch), "apple",
                           getOSAndEnvironmentName(PlatformKind::iOS, "13.0"),
                           "macabi");
    else if (getPlatformName(context.platform).endswith("Simulator"))
      triples.emplace_back(
          getArchitectureName(arch), "apple",
          getOSAndEnvironmentName(context.platform, context.version),
          "simulator");
    else
      triples.emplace_back(
          getArchitectureName(arch), "apple",
          getOSAndEnvironmentName(context.platform, context.version));
  }
}

static bool computeFrontendResultFromFramework(
    sdkdb::Context &context, const Framework &framework,
    std::vector<Triple> &triples, bool isPublic) {
  // bail out of there is no triples.
  if (triples.empty())
    return true;

  auto &fm = context.getFileManager();
  auto &diag = context.getDiag();
  auto job = std::make_unique<FrontendJob>();

  auto rootPath = (isPublic && !context.publicSDKPath.empty())
                      ? context.publicSDKPath
                      : context.internalSDKPath;
  auto frameworkPath = framework.getPath();
  SmallString<PATH_MAX> basePath(rootPath);
  sys::path::append(basePath, frameworkPath);
  job->workingDirectory = globalSnapshot->getWorkingDirectory().str();
  job->cacheFactory = newFileSystemStatCacheFactory<StatRecorder>();
  job->vfs = &fm.getVirtualFileSystem();
  job->language = context.config.getLanguage(frameworkPath);
  job->language_std = context.config.getCommandlineConfig().std;
  job->overwriteRTTI = context.config.getCommandlineConfig().useRTTI;
  job->overwriteNoRTTI = context.config.getCommandlineConfig().useNoRTTI;
  job->visibility = context.config.getCommandlineConfig().visibility;
  job->isysroot = context.config.getSysRoot();
  job->macros = context.config.getMacros(frameworkPath);
  job->includePaths = context.config.getIncludePaths(frameworkPath);
  job->frameworkPaths = context.config.getFrameworkPaths(frameworkPath);
  job->clangExtraArgs = context.config.getCommandlineConfig().clangExtraArgs;
  job->enableModules = context.config.getCommandlineConfig().enableModules;
  job->moduleCachePath = context.config.getCommandlineConfig().moduleCachePath;
  job->validateSystemHeaders =
      context.config.getCommandlineConfig().validateSystemHeaders;
  job->clangResourcePath =
      context.config.getCommandlineConfig().clangResourcePath;
  job->verbose = context.verbose;

  if (!context.diagnosticsFile.empty()) {
    job->clangExtraArgs.emplace_back("-Xclang");
    job->clangExtraArgs.emplace_back("-diagnostic-log-file");
    job->clangExtraArgs.emplace_back("-Xclang");
    job->clangExtraArgs.emplace_back(context.diagnosticsFile);
  }

  HeaderSeq preIncludes;
  for (auto &header : context.config.getPreIncludedHeaders(
           frameworkPath, HeaderType::Public)) {
    preIncludes.emplace_back(header, HeaderType::Public);
    preIncludes.back().isExtra = true;
    preIncludes.back().isPreInclude = true;
  }

  for (auto &header : context.config.getPreIncludedHeaders(
           frameworkPath, HeaderType::Private)) {
    preIncludes.emplace_back(header, HeaderType::Private);
    preIncludes.back().isExtra = true;
    preIncludes.back().isPreInclude = true;
  }

  // Create a sorted list of framework headers.
  HeaderSeq headerFiles;
  for (const auto &header : framework._headerFiles) {
    if (!fm.exists(header.fullPath))
      diag.report(diag::warn_no_such_header_file)
          << (header.type == HeaderType::Private) << header.fullPath;
    headerFiles.emplace_back(header);
  }

  auto sysroot = context.config.getSysRoot();
  findAndAddHeaderFiles(
      headerFiles, fm, diag,
      context.config.getExtraHeaders(frameworkPath, HeaderType::Public),
      HeaderType::Public, sysroot, basePath);

  findAndAddHeaderFiles(
      headerFiles, fm, diag,
      context.config.getExtraHeaders(frameworkPath, HeaderType::Private),
      HeaderType::Private, sysroot, basePath);

  // Create the excluded headers list.
  std::set<const FileEntry *> excludeHeaderFiles;
  std::vector<std::unique_ptr<HeaderGlob>> excludeHeaderGlobs;
  auto parseGlobs = [&](HeaderType type) {
    for (const auto &str :
         context.config.getExcludedHeaders(frameworkPath, type)) {
      auto glob = HeaderGlob::create(str, type);
      if (glob)
        excludeHeaderGlobs.emplace_back(std::move(glob.get()));
      else {
        consumeError(glob.takeError());
        if (auto file = fm.getFile(str))
          excludeHeaderFiles.emplace(*file);
        else {
          diag.report(diag::warn_no_such_header_file)
              << (type == HeaderType::Private) << str;
        }
      }
    }
    return true;
  };

  if (!parseGlobs(HeaderType::Public))
    return false;

  if (!parseGlobs(HeaderType::Private))
    return false;

  for (auto &header : headerFiles) {
    for (auto &glob : excludeHeaderGlobs)
      if (glob->match(header))
        header.isExcluded = true;
  }

  if (!excludeHeaderFiles.empty()) {
    for (auto &header : headerFiles) {
      auto file = fm.getFile(header.fullPath);
      if (!file)
        continue;
      if (excludeHeaderFiles.count(*file))
        header.isExcluded = true;
    }
  }

  for (const auto &glob : excludeHeaderGlobs)
    if (!glob->didMatch())
      diag.report(diag::warn_glob_did_not_match) << glob->str();

  // Check if the framework has an umbrella header and move that to the
  // beginning.
  auto matchAndMarkUmbrella = [](HeaderSeq &array, Regex &regex,
                                 HeaderType type) -> bool {
    auto it = find_if(array, [&regex, type](const HeaderFile &header) {
      return (header.type == type) && regex.match(header.fullPath);
    });

    if (it == array.end())
      return false;

    it->isUmbrellaHeader = true;
    return true;
  };
  auto publicUmbrellaHeaderPath =
      context.config.getUmbrellaHeader(frameworkPath, HeaderType::Public);
  if (!publicUmbrellaHeaderPath.empty()) {
    auto escapedString = Regex::escape(publicUmbrellaHeaderPath);
    Regex umbrellaRegex(escapedString);

    if (!matchAndMarkUmbrella(headerFiles, umbrellaRegex, HeaderType::Public))
      diag.report(diag::warn_no_such_umbrella_header_file)
          << false /* private */
          << publicUmbrellaHeaderPath;
  } else {
    auto frameworkName = sys::path::stem(framework.getName());
    auto umbrellaName = "/" + Regex::escape(frameworkName) + "\\.h";
    Regex umbrellaRegex(umbrellaName);

    matchAndMarkUmbrella(headerFiles, umbrellaRegex, HeaderType::Public);
  }

  auto privateUmbrellaHeaderPath =
      context.config.getUmbrellaHeader(frameworkPath, HeaderType::Private);
  if (!privateUmbrellaHeaderPath.empty()) {
    auto escapedString = Regex::escape(privateUmbrellaHeaderPath);
    Regex umbrellaRegex(escapedString);

    if (!matchAndMarkUmbrella(headerFiles, umbrellaRegex, HeaderType::Private))
      diag.report(diag::warn_no_such_umbrella_header_file)
          << true /* private */
          << privateUmbrellaHeaderPath;
  } else {
    auto frameworkName = sys::path::stem(framework.getName());
    auto umbrellaName = "/" + Regex::escape(frameworkName) + "[_]?Private\\.h";
    Regex umbrellaRegex(umbrellaName);

    matchAndMarkUmbrella(headerFiles, umbrellaRegex, HeaderType::Private);
  }

  if (diag.hasErrorOccurred())
    return false;

  std::stable_sort(headerFiles.begin(), headerFiles.end());
  job->headerFiles.insert(job->headerFiles.end(), preIncludes.begin(),
                          preIncludes.end());
  job->headerFiles.insert(job->headerFiles.end(), headerFiles.begin(),
                          headerFiles.end());

  // Skip frontend work if there are no headers.
  if (job->headerFiles.empty())
    return true;

  // If use overlay, map the headers onto sysroot.
  if (context.config.useOverlay(frameworkPath)) {
    auto &filemap =
        isPublic ? context.publicVFSOverlay : context.internalVFSOverlay;
    if (!filemap.empty()) {
      for (auto &header : job->headerFiles) {
        auto mappedPath = llvm::find_if(
            filemap, [&](DirectoryScanner::FileMap::const_reference entry) {
              return header.fullPath == entry.second;
            });
        if (mappedPath != filemap.end())
          header.fullPath = mappedPath->first;
      }
      IntrusiveRefCntPtr<vfs::RedirectingFileSystem> vfs(
          vfs::RedirectingFileSystem::create(filemap,
                                             /*UseExternalName=*/true,
                                             *job->vfs));
      job->vfs = vfs;
    }
  }

  if (framework.isSysRoot) {
    if (!context.config.useOverlay(frameworkPath)) {
      SmallString<PATH_MAX> prefixIncludes(rootPath);
      sys::path::append(prefixIncludes, "usr", "include");
      job->includePaths.insert(job->includePaths.begin(),
                               prefixIncludes.str().str());
      SmallString<PATH_MAX> prefixFrameworks(rootPath);
      sys::path::append(prefixFrameworks, "System", "Library", "Frameworks");
      job->frameworkPaths.insert(job->frameworkPaths.begin(),
                                 prefixFrameworks.str().str());
    }
  } else if (!context.config.useOverlay(frameworkPath)) {
    // Add the current framework directory as a system framework directory. This
    // will prevent it from being droped from the top of the list if there is
    // a matching system framework include path.
    auto currentFrameworkPath = sys::path::parent_path(basePath);
    // for macOS which doesn't use shallow framework layout, need more
    // adjustment.
    if (sys::path::filename(currentFrameworkPath) == "Versions")
      currentFrameworkPath =
          sys::path::parent_path(sys::path::parent_path(currentFrameworkPath));
    job->frameworkPaths.insert(job->frameworkPaths.begin(),
                               currentFrameworkPath.str());
  }

  // If we are scanning public headers, we use MaskingOverlayFileSystem to
  // masking out the private locations from the SDK.
  if (isPublic) {
    IntrusiveRefCntPtr<MaskingOverlayFileSystem> overlay(
        new MaskingOverlayFileSystem(job->vfs, job->isysroot));
    for (auto &mask : context.config.getMaskPaths())
      overlay->addExtraMaskingDirectory(mask);
    job->vfs = overlay;
  }

  auto &output =
      isPublic ? context.publicSDKResults : context.internalSDKResults;
  for (auto type : {HeaderType::Public, HeaderType::Private}) {
    std::vector<FrontendContext> results;
    for (auto &target : triples) {
      job->target = target;
      job->type = type;
      job->systemIncludePaths = context.systemIncludePaths;
      job->systemFrameworkPaths = context.systemFrameworkPaths;
      job->useUmbrellaHeaderOnly = context.config.useUmbrellaOnly();
      if (target.getEnvironment() == Triple::MacABI) {
        if (target.getArchName() == "i386")
          continue; // workaround for objc4, which builds i386 zippered.
        job->systemIncludePaths.push_back(job->isysroot +
                                          "/System/iOSSupport/usr/include");
        job->systemIncludePaths.push_back(
            job->isysroot + "/System/iOSSupport/usr/local/include");
        job->systemFrameworkPaths.push_back(
            job->isysroot + "/System/iOSSupport/System/Library/Frameworks");
        job->systemFrameworkPaths.push_back(
            job->isysroot +
            "/System/iOSSupport/System/Library/PrivateFrameworks");
        job->useUmbrellaHeaderOnly = context.config.useUmbrellaOnly();
      }

      auto result = runFrontend(*job);
      if (!result)
        return false;
      // If there are no files in the HeaderMaps, don't add the empty results.
      if (result->files.empty())
        continue;

      results.emplace_back(std::move(result.getValue()));
    }
    for (auto &r : results)
      output.emplace_back(std::move(r));
  }

  return true;
}

static Expected<API> createAPIsFromSwiftAPIJson(llvm::json::Object &input) {
  auto version = input.getString("version");
  if (*version != "1.0")
    return make_error<APIJSONError>("Unsupported version");

  if (auto result = APIJSONSerializer::parse(&input))
    return std::move(*result);
  else
    return result.takeError();
}

static Error extractSwiftAPI(sdkdb::Context &context, const SwiftModule &module,
                             std::vector<Triple> &triples,
                             StringRef swiftAPIExtract, StringRef vfsFile,
                             StringRef sdkRoot) {
  for (const auto &target : triples) {
    SmallString<PATH_MAX> outputFile;
    if (auto ec = sys::fs::createTemporaryFile("swiftapi", "json", outputFile))
      return make_error<StringError>("unable to create temporary output file",
                                     ec);
    FileRemover removeOutputFile(outputFile);

    auto targetStr = target.str();
    SmallString<PATH_MAX> frameworkPath(sdkRoot);
    sys::path::append(frameworkPath, "System", "Library", "PrivateFrameworks");
    SmallString<PATH_MAX> libraryPath(sdkRoot);
    sys::path::append(libraryPath, "usr", "local", "include");
    const StringRef swiftArgs[] = {
        swiftAPIExtract, "-target",   targetStr,     "-sdk",  sdkRoot,
        "-module-name",  module.name, "-vfsoverlay", vfsFile, "-F",
        frameworkPath,   "-I",        libraryPath,   "-o",    outputFile};

    SmallString<PATH_MAX> stderrFile;
    if (auto ec = sys::fs::createTemporaryFile("stderr", "txt", stderrFile))
      return make_error<StringError>("unable to create temporary stderr file",
                                     ec);
    FileRemover removeStderrFile(stderrFile);
    const Optional<StringRef> redirects[] = {/*STDIN=*/llvm::None,
                                             /*STDOUT=*/llvm::None,
                                             /*STDERR=*/StringRef(stderrFile)};
    if (context.verbose) {
      outs() << "\nswift-api-extract: ";
      for (const auto &arg : swiftArgs)
        outs() << arg << " ";
      outs() << "\n";
    }

    bool failed = sys::ExecuteAndWait(swiftAPIExtract, swiftArgs,
                                      /*env=*/llvm::None, redirects);

    if (failed) {
      auto bufferOr = MemoryBuffer::getFile(stderrFile);
      if (auto ec = bufferOr.getError())
        return make_error<StringError>("unable to read file", ec);

      std::string message = "'swift-api-extract' invocation failed:\n";
      for (auto arg : swiftArgs) {
        if (arg.empty())
          continue;
        message.append(arg.str()).append(1, ' ');
      }
      message.append(1, '\n');
      message.append(bufferOr.get()->getBuffer().str());

      return make_error<StringError>(
          message, std::make_error_code(std::errc::not_supported));
    }

    auto file = context.getFileManager().getFile(outputFile);
    if (!file)
      return make_error<StringError>(
          "cannot open swift-api-extract output file",
          inconvertibleErrorCode());

    auto buffer = context.getFileManager().getBufferForFile(*file);
    if (!buffer)
      return make_error<StringError>(
          "cannot open swift-api-extract output file",
          inconvertibleErrorCode());

    auto inputValue = json::parse((*buffer)->getBuffer());
    if (!inputValue)
      return inputValue.takeError();

    auto *root = inputValue->getAsObject();
    if (!root)
      return make_error<APIJSONError>("API is not a JSON Object");

    auto publicResult = createAPIsFromSwiftAPIJson(*root);
    if (!publicResult)
      return publicResult.takeError();

    sdkdb::APILocUpdater publicLocUpdater(sdkRoot, *publicResult);
    publicResult->visit(publicLocUpdater);
    context.extraPublicSDKResults.emplace_back(std::move(*publicResult));

    auto internalResult = createAPIsFromSwiftAPIJson(*root);
    if (!internalResult)
      return publicResult.takeError();

    sdkdb::APILocUpdater internalLocUpdater(sdkRoot, *internalResult);
    internalResult->visit(internalLocUpdater);
    context.extraInternalSDKResults.emplace_back(std::move(*internalResult));

    context.scannedSwiftInterface = true;
  }

  return Error::success();
}

static Error computeSwiftInterfacesFromFramework(sdkdb::Context &context,
                                                 const Framework &framework,
                                                 std::vector<Triple> &triples,
                                                 bool isPublic) {
  if (framework._swiftModules.empty())
    return Error::success();

  // skip private frameworks if public is requested.
  if (isPublic) {
    auto frameworkPath = framework.getPath();
    frameworkPath.consume_front(context.publicSDKPath);
    if (frameworkPath != "/" && !isWithinPublicLocation(frameworkPath))
      return Error::success();
  }

  auto swiftAPIExtract = findSwiftAPIExtractExecutable(context.getDiag());
  if (!swiftAPIExtract) {
    // Issue warning for missing `swift-api-extract` for now and proceed.
    context.getDiag().report(diag::warn)
        << toString(swiftAPIExtract.takeError());
    return Error::success();
  }

  SmallString<PATH_MAX> vfsFile;
  if (auto ec = sys::fs::createTemporaryFile("vfsoverlay", "yaml", vfsFile))
    return make_error<StringError>("unable to create temporary input file", ec);
  FileRemover removeVFSFile(vfsFile);
  std::error_code ec;
  raw_fd_ostream vfsMap(vfsFile, ec, sys::fs::F_None);
  if (ec)
    return make_error<StringError>("cannot create vfs file", ec);
  auto &vfsOverlay =
      isPublic ? context.publicVFSOverlay : context.internalVFSOverlay;
  vfs::YAMLVFSWriter vfsWriter;
  for (auto &entry : vfsOverlay)
    vfsWriter.addFileMapping(entry.first, entry.second);
  vfsWriter.write(vfsMap);
  vfsMap.close();

  for (auto &module : framework._swiftModules) {
    if (auto err = extractSwiftAPI(context, module, triples, *swiftAPIExtract,
                                   vfsFile, context.config.getSysRoot()))
      return err;
  }

  return Error::success();
}

static Error scanFramework(sdkdb::Context &context, Framework &framework,
                           bool isPublic, bool binaryOnly) {
  //
  // First scan all sub-frameworks, because we most likely will depend on them.
  //
  for (auto &F : framework._subFrameworks) {
    if (auto err = scanFramework(context, F, isPublic, binaryOnly))
      return err;
  }

  //
  // Second scan all versions ...
  //
  for (auto &F : framework._versions) {
    if (auto err = scanFramework(context, F, isPublic, binaryOnly))
      return err;
  }

  //
  // Now scan the framework binary.
  //
  std::vector<Triple> triples;
  for (const auto &path : framework._dynamicLibraryFiles) {
    if (auto err = context.scanBinaryFile(path, triples, isPublic))
      return err;
  }

  if (binaryOnly)
    return Error::success();

  // If there are no binaries, guess the triple from environment.
  if (triples.empty())
    inferTriplesFromEnvironment(context, triples);

  // Now scan the header files.
  if (!computeFrontendResultFromFramework(context, framework, triples,
        isPublic)) {
    context.hasSDKDBError = true;
    context.getDiag().report(diag::err_cannot_generate_sdkdb)
        << "Failed to scan header interface";
  }

  // Scan swift interfaces.
  if (auto err = computeSwiftInterfacesFromFramework(context, framework,
                                                     triples, isPublic)) {
    context.hasSDKDBError = true;
    context.getDiag().report(diag::err_cannot_generate_sdkdb)
        << toString(std::move(err));
  }

  return Error::success();
}

static bool interfaceScan(sdkdb::Context &context, Options &opts) {
  auto &config = context.config.getCommandlineConfig();
  auto &diag = context.getDiag();

  // FIXME: Copy the options for now to reduce the amount of change.
  config.language = opts.frontendOptions.language;
  config.std = opts.frontendOptions.language_std;
  config.isysroot = opts.frontendOptions.isysroot;
  config.frameworkPaths = opts.frontendOptions.frameworkPaths;
  config.includePaths = opts.frontendOptions.includePaths;
  config.macros = opts.frontendOptions.macros;
  config.useRTTI = opts.frontendOptions.useRTTI;
  config.useNoRTTI = opts.frontendOptions.useNoRTTI;
  config.visibility = opts.frontendOptions.visibility;
  config.enableModules = opts.frontendOptions.enableModules;
  config.moduleCachePath = opts.frontendOptions.moduleCachePath;
  config.validateSystemHeaders = opts.frontendOptions.validateSystemHeaders;
  config.clangExtraArgs = opts.frontendOptions.clangExtraArgs;
  config.clangResourcePath = opts.frontendOptions.clangResourcePath;

  if (opts.frontendOptions.targets.empty())
    context.config.setArchitectures(ArchitectureSet(std::numeric_limits<uint32_t>::max()));
  else
    context.config.setArchitectures(
        mapToArchitectureSet(opts.frontendOptions.targets));

  // Handle extra header directories/files.
  config.extraPublicHeaders = opts.tapiOptions.extraPublicHeaders;
  config.extraPrivateHeaders = opts.tapiOptions.extraPrivateHeaders;

  // Handle excluded header files.
  config.excludePublicHeaders = opts.tapiOptions.excludePublicHeaders;
  config.excludePrivateHeaders = opts.tapiOptions.excludePrivateHeaders;

  // Handle disabling of header scanning.
  config.scanPublicHeaders = opts.sdkdbOptions.scanPublicHeaders;
  config.scanPrivateHeaders = opts.sdkdbOptions.scanPrivateHeaders;

  // Handle public/private umbrella header.
  if (!opts.tapiOptions.publicUmbrellaHeaderPath.empty()) {
    if (!context.getFileManager().exists(
            opts.tapiOptions.publicUmbrellaHeaderPath)) {
      diag.report(clang::diag::err_drv_no_such_file)
          << opts.tapiOptions.publicUmbrellaHeaderPath;
      return false;
    }
    config.publicUmbrellaHeaderPath = opts.tapiOptions.publicUmbrellaHeaderPath;
  }

  if (!opts.tapiOptions.privateUmbrellaHeaderPath.empty()) {
    if (!context.getFileManager().exists(
            opts.tapiOptions.privateUmbrellaHeaderPath)) {
      diag.report(clang::diag::err_drv_no_such_file)
          << opts.tapiOptions.privateUmbrellaHeaderPath;
      return false;
    }
    config.privateUmbrellaHeaderPath =
        opts.tapiOptions.privateUmbrellaHeaderPath;
  }

  // Scan PublicSDKContent.
  if (config.scanPublicHeaders) {
    DirectoryScanner scanner(context.getFileManager(), diag,
                             ScannerMode::ScanRuntimeRoot);
    // Scan binary first.
    scanner.setConfiguration(&context.config);
    context.config.setRootPath(opts.sdkdbOptions.runtimeRoot);
    if (!scanner.scan(opts.sdkdbOptions.runtimeRoot))
      return false;

    // Scan headers if it exists.
    auto rootPath = opts.sdkdbOptions.publicSDKContentRoot.empty()
                        ? opts.sdkdbOptions.sdkContentRoot
                        : opts.sdkdbOptions.publicSDKContentRoot;
    scanner.setMode(ScannerMode::ScanPublicSDK);
    context.config.setRootPath(rootPath);
    if (!scanner.scan(rootPath))
      return false;

    context.publicVFSOverlay =
        scanner.getVFSFileMap(context.config.getSysRoot());

    auto frameworks = scanner.takeResult();
    assert(frameworks.size() == 1 &&
           "There should be only one top level framework");
    auto &framework = frameworks.front();
    if (auto err = scanFramework(context, framework, /*isPublic*/ true,
                                 /*binary only*/ false))
      diag.report(diag::err_cannot_generate_sdkdb) << toString(std::move(err));

    // Update APILoc for the header scan results.
    for (auto &result : context.publicSDKResults) {
      sdkdb::APILocUpdater locUpdater(rootPath, result.api);
      result.api.visit(locUpdater);
    }
  }

  // Scan SDKContentRoot.
  {
    DirectoryScanner scanner(context.getFileManager(), diag,
                             ScannerMode::ScanRuntimeRoot);
    scanner.setConfiguration(&context.config);

    context.config.setRootPath(opts.sdkdbOptions.runtimeRoot);
    if (!scanner.scan(opts.sdkdbOptions.runtimeRoot))
      return false;

    scanner.setMode(ScannerMode::ScanInternalSDK);
    context.config.setRootPath(opts.sdkdbOptions.sdkContentRoot);
    if (!scanner.scan(opts.sdkdbOptions.sdkContentRoot))
      return false;

    context.internalVFSOverlay =
        scanner.getVFSFileMap(context.config.getSysRoot());

    auto frameworks = scanner.takeResult();
    assert(frameworks.size() == 1 &&
           "There should be only one top level framework");
    auto &framework = frameworks.front();
    if (auto err = scanFramework(context, framework, /*isPublic*/ false,
                                 /*binary only*/ !config.scanPrivateHeaders))
      diag.report(diag::err_cannot_generate_sdkdb) << toString(std::move(err));

    for (auto &result : context.internalSDKResults) {
      sdkdb::APILocUpdater locUpdater(opts.sdkdbOptions.sdkContentRoot,
                                      result.api);
      result.api.visit(locUpdater);
    }
  }

  return true;
}

static bool writePartialSDKDB(sdkdb::Context &context) {
  auto partialSDKOutput = [&](raw_ostream &os) {
    context.hasWrittenPartialOutput = true;
    return PartialSDKDB::serialize(
        os, context.projectName, context.internalBinaryResults,
        context.publicSDKResults, context.extraPublicSDKResults,
        context.internalSDKResults, context.extraInternalSDKResults,
        context.hasSDKDBError);
  };
  if (auto err = context.performOutput("partial.sdkdb", partialSDKOutput)) {
    context.getDiag().report(diag::err_cannot_generate_sdkdb)
        << toString(std::move(err));
    return false;
  }

  return true;
}

static Error parsePartialSDKDB(sdkdb::Context &context, StringRef JSON) {
  auto inputValue = json::parse(JSON);
  if (!inputValue)
    return inputValue.takeError();

  auto *root = inputValue->getAsObject();
  if (!root)
    return make_error<APIJSONError>("API is not a JSON Object");

  if (context.action & SDKDBAction::SDKDBPublicGen) {
    auto partialResult = PartialSDKDB::createPublicAPIsFromJSON(*root);
    if (!partialResult)
      return partialResult.takeError();

    for (auto &result : partialResult->binaryInterfaces)
      context.publicBinaryResults.emplace_back(std::move(result));
    for (auto &result : partialResult->headerInterfaces)
      context.extraPublicSDKResults.emplace_back(std::move(result));
  }

  if (context.action & SDKDBAction::SDKDBPrivateGen) {
    auto partialResult = PartialSDKDB::createPrivateAPIsFromJSON(*root);
    if (!partialResult)
      return partialResult.takeError();

    for (auto &result : partialResult->binaryInterfaces)
      context.internalBinaryResults.emplace_back(std::move(result));
    for (auto &result : partialResult->headerInterfaces)
      context.extraInternalSDKResults.emplace_back(std::move(result));
  }

  return Error::success();
}

static bool readPartialSDKDBFile(sdkdb::Context &context, StringRef path) {
  auto file = context.getFileManager().getFile(path);
  if (!file) {
    context.getDiag().report(diag::err_cannot_open_file) << path;
    return false;
  }

  auto buffer = context.getFileManager().getBufferForFile(*file);
  if (!buffer) {
    context.getDiag().report(diag::err_cannot_read_file) << path;
    return false;
  }

  if (auto err = parsePartialSDKDB(context, (*buffer)->getBuffer())) {
    context.getDiag().report(diag::err_cannot_generate_sdkdb)
        << toString(std::move(err));
    return false;
  }

  return true;
}

static bool readPartialSDKDBInputs(sdkdb::Context &context, Options &opts) {
  for (const auto& input : opts.driverOptions.inputs) {
    if (!readPartialSDKDBFile(context, input))
      return false;
  }

  if (context.partialSDKDBFilelist.empty())
    return true;

  // read partial sdkdb from file list.
  auto file = context.getFileManager().getFile(context.partialSDKDBFilelist);
  if (!file) {
    context.getDiag().report(diag::err_cannot_open_file)
        << context.partialSDKDBFilelist;
    return false;
  }

  auto bufferOrErr = context.getFileManager().getBufferForFile(*file);
  if (!bufferOrErr) {
    context.getDiag().report(diag::err_cannot_read_file)
        << context.partialSDKDBFilelist;
    return false;
  }

  auto buffer = bufferOrErr.get()->getBuffer();
  SmallVector<StringRef, 16> lines;
  buffer.split(lines, "\n", /*MaxSplit=*/-1, /*KeepEmpty=*/false);
  for (const auto &line : lines) {
    auto l = line.trim();
    if (l.empty())
      continue;

    // Skip comments
    if (l.startswith("#"))
      continue;

    if (!readPartialSDKDBFile(context, l))
      return false;
  }

  return true;
}

static void readExistingPartialSDKDBFromDirectory(sdkdb::Context &context) {
  // Skip if no output is specified or output is stdout.
  if (context.installAPISDKDBPath.empty())
    return;

  auto &fm = context.getFileManager();
  // if output path is not a directory, skip.
  if (!fm.isDirectory(context.installAPISDKDBPath))
    return;

  // look into directory for partial SDKDB files.
  auto &fs = fm.getVirtualFileSystem();
  std::error_code ec;
  std::vector<std::string> inputFiles;
  for (vfs::directory_iterator
           i = fs.dir_begin(context.installAPISDKDBPath, ec),
           ie;
       i != ie; i.increment(ec)) {
    // ignore errors.
    if (ec) {
      ec.clear();
      continue;
    }

    auto path = i->path();
    if (!path.endswith(".sdkdb"))
      continue;
   
    // This is the output name, skip that.
    if (sys::path::filename(path) == "partial.sdkdb")
      continue;

    inputFiles.emplace_back(path.str());
  }

  // sort the path so the output is deterministic and not depending on the
  // traverse order of the file system.
  llvm::sort(inputFiles);

  for (auto &path : inputFiles) {
    // Read partial SDKDB output.
    auto file = context.getFileManager().getFile(path);
    if (!file)
      continue;
    auto buffer = context.getFileManager().getBufferForFile(*file);
    if (!buffer)
      continue;

    auto inputValue = json::parse((*buffer)->getBuffer());
    if (!inputValue) {
      consumeError(inputValue.takeError());
      continue;
    }

    auto *root = inputValue->getAsObject();
    if (!root)
      continue;

    {
      auto partialResult = PartialSDKDB::createPublicAPIsFromJSON(*root);
      if (!partialResult) {
        consumeError(partialResult.takeError());
        continue;
      }

      for (auto &result : partialResult->binaryInterfaces)
        context.publicBinaryResults.emplace_back(std::move(result));
      for (auto &result : partialResult->headerInterfaces)
        context.extraPublicSDKResults.emplace_back(std::move(result));
    }

    {
      auto partialResult = PartialSDKDB::createPrivateAPIsFromJSON(*root);
      if (!partialResult) {
        consumeError(partialResult.takeError());
        continue;
      }

      for (auto &result : partialResult->binaryInterfaces)
        context.internalBinaryResults.emplace_back(std::move(result));
      for (auto &result : partialResult->headerInterfaces)
        context.extraInternalSDKResults.emplace_back(std::move(result));
    }
  }
}

/// Scan the directory for header and dynamic libraries and generate
/// SDKDB files.
static bool runSDKDBDriver(sdkdb::Context &context, DiagnosticsEngine &diag,
                           Options &opts) {
  // setup diagnostics file.
  if (!opts.sdkdbOptions.diagnosticsFile.empty())
    diag.setupDiagnosticsFile(opts.sdkdbOptions.diagnosticsFile);

  // diagnose incompatible options.
  if (opts.sdkdbOptions.action & SDKDBAction::SDKDBInterfaceScan) {
    if (opts.sdkdbOptions.runtimeRoot.empty()) {
      diag.report(diag::err_expected_option) << "RuntimeRoot"
                                             << "--runtime-root";
      return false;
    }
    if (opts.sdkdbOptions.sdkContentRoot.empty()) {
      diag.report(diag::err_expected_option) << "SDKContentRoot"
                                             << "--sdk-content-root";
      return false;
    }
  }

  if (opts.sdkdbOptions.action == SDKDBAction::SDKDBNone) {
    // infer default action.
    if (!opts.sdkdbOptions.runtimeRoot.empty() &&
        !opts.sdkdbOptions.sdkContentRoot.empty())
      opts.sdkdbOptions.action = SDKDBAction::SDKDBInterfaceScan;
    else {
      // diagnose no action.
      diag.report(diag::err_expected_option) << "SDKDBAction"
                                             << "--action=";
      return false;
    }
  }

  // set configuraion file.
  if (!opts.sdkdbOptions.configurationFile.empty()) {
    auto file =
        context.getFileManager().getFile(opts.sdkdbOptions.configurationFile);
    if (!file) {
      diag.report(clang::diag::err_drv_no_such_file)
          << opts.sdkdbOptions.configurationFile;
      return false;
    }

    auto bufferOrErr = context.getFileManager().getBufferForFile(*file);
    if (auto ec = bufferOrErr.getError()) {
      diag.report(diag::err_cannot_read_file) << (*file)->getName()
                                              << ec.message();
      return false;
    }

    auto configurationFile =
        ConfigurationFileReader::get((*bufferOrErr)->getMemBufferRef());
    if (!configurationFile) {
      diag.report(diag::err_cannot_read_file)
          << (*file)->getName() << toString(configurationFile.takeError());
      return false;
    }

    auto configuration = configurationFile.get()->takeConfigurationFile();
    context.config.setConfiguration(std::move(configuration), context);
  }

  // Run InterfaceScan.
  if (opts.sdkdbOptions.action & SDKDBAction::SDKDBInterfaceScan) {
    // Scan the output directory to see if there are any partial outputs.
    readExistingPartialSDKDBFromDirectory(context);
    // Scan interface from roots.
    if (!interfaceScan(context, opts))
      return false;
  }

  // If all we need is to scan interface, write partialSDKDB and return.
  if (opts.sdkdbOptions.action == SDKDBAction::SDKDBInterfaceScan) {
    if (!writePartialSDKDB(context))
      return false;
    return !diag.hasErrorOccurred();
  }

  // Process extra inputs.
  if (!readPartialSDKDBInputs(context, opts))
    return false;

  // Combine SDKDB.
  if (opts.sdkdbOptions.action & SDKDBAction::SDKDBPublicGen) {
    SDKDBBuilderOptions options = SDKDBBuilderOptions::isPublicOnly;
    SDKDBBuilder builder(diag, options);
    for (auto &api : context.publicBinaryResults) {
      if (auto err = builder.addBinaryAPI(std::move(api)))
        diag.report(diag::err_cannot_generate_sdkdb)
            << toString(std::move(err));
    }

    for (const auto &result : context.publicSDKResults) {
      if (auto err = builder.addHeaderAPI(result.api))
        diag.report(diag::err_cannot_generate_sdkdb)
            << toString(std::move(err));
    }

    for (const auto &api : context.extraPublicSDKResults) {
      if (auto err = builder.addHeaderAPI(api))
        diag.report(diag::err_cannot_generate_sdkdb)
            << toString(std::move(err));
    }

    if (auto err = builder.finalize()) {
      diag.report(diag::err_cannot_generate_sdkdb) << toString(std::move(err));
      return false;
    }

    auto publicSDKOutput = [&](raw_ostream &os) {
      builder.serialize(os, /*compact*/ false);
      return Error::success();
    };
    if (auto err = context.performOutput("public.sdkdb", publicSDKOutput)) {
      context.getDiag().report(diag::err_cannot_generate_sdkdb)
          << toString(std::move(err));
      return false;
    }
  }

  if (opts.sdkdbOptions.action & SDKDBAction::SDKDBPrivateGen) {
    SDKDBBuilder builder(diag);
    for (auto &api : context.internalBinaryResults) {
      if (auto err = builder.addBinaryAPI(std::move(api)))
        diag.report(diag::err_cannot_generate_sdkdb)
            << toString(std::move(err));
    }

    auto &sdkResult = context.internalSDKResults.empty()
                         ? context.publicSDKResults
                         : context.internalSDKResults;
    for (const auto &result : sdkResult) {
      if (auto err = builder.addHeaderAPI(result.api))
        diag.report(diag::err_cannot_generate_sdkdb)
            << toString(std::move(err));
    }

    auto &extraSDKResult = context.extraInternalSDKResults.empty()
                         ? context.extraPublicSDKResults
                         : context.extraInternalSDKResults;
    for (const auto &api : extraSDKResult) {
      if (auto err = builder.addHeaderAPI(api))
        diag.report(diag::err_cannot_generate_sdkdb)
            << toString(std::move(err));
    }

    if (auto err = builder.finalize()) {
      diag.report(diag::err_cannot_generate_sdkdb) << toString(std::move(err));
      return false;
    }

    auto internalSDKOutput = [&](raw_ostream &os) {
      builder.serialize(os, /*compact*/ false);
      return Error::success();
    };
    if (auto err = context.performOutput("private.sdkdb", internalSDKOutput)) {
      context.getDiag().report(diag::err_cannot_generate_sdkdb)
          << toString(std::move(err));
      return false;
    }
  }

  return !diag.hasErrorOccurred();
}

bool Driver::SDKDB::run(DiagnosticsEngine &diag, Options &opts) {
  sdkdb::Context context(opts, diag);
  if (!runSDKDBDriver(context, diag, opts)) {
    // If doing interface scan only and the partial output is not written,
    // write an empty context.
    if (opts.sdkdbOptions.action == SDKDBAction::SDKDBInterfaceScan &&
        !context.hasWrittenPartialOutput) {
      sdkdb::Context emptyContext(opts, diag);
      emptyContext.hasSDKDBError = true;
      writePartialSDKDB(emptyContext);
    }
    // Do not return error unless TAPI_SDKDB_FORCE_ERROR is set.
    return !sys::Process::GetEnv("TAPI_SDKDB_FORCE_ERROR").hasValue();
  }

  return true;
}

TAPI_NAMESPACE_INTERNAL_END
