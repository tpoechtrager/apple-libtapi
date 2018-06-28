//===--- Options.cpp - Options --------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "tapi/Driver/Options.h"
#include "tapi/Core/Path.h"
#include "tapi/Defines.h"
#include "tapi/Driver/Diagnostics.h"
#include "tapi/Driver/DriverOptions.h"
#include "tapi/Driver/Snapshot.h"
#include "tapi/Driver/SnapshotFileSystem.h"
#include "tapi/LinkerInterfaceFile.h"
#include "clang/Basic/Version.inc"
#include "clang/Basic/VirtualFileSystem.h"
#include "clang/Config/config.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Option/ArgList.h"
#include "llvm/Option/OptSpecifier.h"
#include "llvm/Support/raw_ostream.h"
#include <string>
#include <utility>

using namespace llvm;
using namespace llvm::opt;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

constexpr char toolName[] = "Text-based Stubs Tool";

static TAPICommand getTAPICommand(StringRef value) {
  // Accept both command options (with and without "-") to not break existing
  // tools.
  return StringSwitch<TAPICommand>(value.ltrim("-"))
      .Case("archive", TAPICommand::Archive)
      .Case("scan", TAPICommand::Scan)
      .Case("stubify", TAPICommand::Stubify)
      .Case("installapi", TAPICommand::InstallAPI)
      .Case("reexport", TAPICommand::Reexport)
      .Case("sdkdb", TAPICommand::SDKDB)
      .Case("sdkdb-verify", TAPICommand::SDKDBVerifier)
      .Case("generate-api-tests", TAPICommand::GenerateAPITests)
      .Default(TAPICommand::Driver);
}

static StringRef getNameFromTAPICommand(TAPICommand command) {
  switch (command) {
  case TAPICommand::Driver:
    return "";
  case TAPICommand::Archive:
    return "archive";
  case TAPICommand::Scan:
    return "scan";
  case TAPICommand::Stubify:
    return "stubify";
  case TAPICommand::InstallAPI:
    return "installapi";
  case TAPICommand::Reexport:
    return "reexport";
  case TAPICommand::SDKDB:
    return "sdkdb";
  case TAPICommand::SDKDBVerifier:
    return "sdkdb-verify";
  case TAPICommand::GenerateAPITests:
    return "generate-api-tests";
  }
}

static InputArgList parseArgString(DiagnosticsEngine &diags,
                                   ArrayRef<const char *> argString,
                                   OptTable *optTable, unsigned includedFlags,
                                   unsigned excludedFlags) {
  unsigned missingArgIndex, missingArgCount;
  auto args = optTable->ParseArgs(argString, missingArgIndex, missingArgCount,
                                  includedFlags, excludedFlags);

  // Print errors and warnings for the parsed arguments.
  if (missingArgCount) {
    diags.report(clang::diag::err_drv_missing_argument)
        << args.getArgString(missingArgIndex) << missingArgCount;
    return args;
  }

  for (auto unknownArg : args.filtered(OPT_UNKNOWN)) {
    diags.report(clang::diag::err_drv_unknown_argument)
        << unknownArg->getAsString(args);
  }

  return args;
}

static unsigned getIncludeOptionFlagMasks(TAPICommand command) {
  unsigned flags = TapiFlags::DriverOption;

  switch (command) {
  case TAPICommand::Driver:
    break;
  case TAPICommand::Archive:
    flags |= TapiFlags::ArchiveOption;
    break;
  case TAPICommand::Scan:
    flags |= TapiFlags::ScanOption;
    break;
  case TAPICommand::Stubify:
    flags |= TapiFlags::StubOption;
    break;
  case TAPICommand::InstallAPI:
    flags |= TapiFlags::InstallAPIOption;
    break;
  case TAPICommand::Reexport:
    flags |= TapiFlags::ReexportOption;
    break;
  case TAPICommand::SDKDB:
    flags |= TapiFlags::SDKDBOption;
    break;
  case TAPICommand::SDKDBVerifier:
    flags |= TapiFlags::SDKDBVerifyOption;
    break;
  case TAPICommand::GenerateAPITests:
    flags |= TapiFlags::GenerateAPITestsOption;
    break;
  }

  return flags;
}

static std::string getClangResourcesPath(FileManager &fm) {
  // Exists solely for the purpose of lookup of the resource path.
  // This just needs to be some symbol in the binary.
  static int staticSymbol;
  // The driver detects the builtin header path based on the path of the
  // executable.
  auto mainExecutable = sys::fs::getMainExecutable("tapi", &staticSymbol);
  StringRef dir = llvm::sys::path::parent_path(mainExecutable);

  // Compute the path to the resource directory.
  StringRef clangResourceDir(CLANG_RESOURCE_DIR);
  if (!clangResourceDir.empty()) {
    SmallString<PATH_MAX> path(dir);
    llvm::sys::path::append(path, clangResourceDir);

    if (fm.exists(path))
      return path.str();
  }

  {
    // Try the default clang path.
    SmallString<PATH_MAX> path(dir);
    llvm::sys::path::append(path, "..", Twine("lib") + CLANG_LIBDIR_SUFFIX,
                            "clang", CLANG_VERSION_STRING);
    if (fm.exists(path))
      return path.str();
  }

  // Try the default tapi path.
  SmallString<PATH_MAX> path(dir);
  llvm::sys::path::append(path, "../lib/clang");
  if (!fm.exists(path))
    return std::string();

  // Get the last entry.
  std::error_code ec;
  std::string result;
  auto &fs = *fm.getVirtualFileSystem();
  vfs::directory_iterator file = fs.dir_begin(path, ec), endDir;
  while (!ec && file != endDir) {
    result = file->getName();
    file.increment(ec);
  }

  if (result.empty())
    return std::string();

  return result;
}

bool DriverOptions::operator==(const DriverOptions &other) const {
  return std::tie(printVersion, printHelp, printHelpHidden, inputs,
                  outputPath) == std::tie(other.printVersion, other.printHelp,
                                          other.printHelpHidden, other.inputs,
                                          other.outputPath);
}

bool ArchiveOptions::operator==(const ArchiveOptions &other) const {
  return std::tie(action, arch) == std::tie(other.action, other.arch);
}

bool LinkerOptions::operator==(const LinkerOptions &other) const {
  return std::tie(architectures, installName, currentVersion,
                  compatibilityVersion, isDynamicLibrary, allowableClients,
                  reexportedLibraries, isApplicationExtensionSafe) ==
         std::tie(other.architectures, other.installName, other.currentVersion,
                  other.compatibilityVersion, other.isDynamicLibrary,
                  other.allowableClients, other.reexportedLibraries,
                  other.isApplicationExtensionSafe);
}

bool FrontendOptions::operator==(const FrontendOptions &other) const {
  return std::tie(platform, osVersion, language, language_std, isysroot,
                  systemFrameworkPaths, frameworkPaths, libraryPaths,
                  systemIncludePaths, includePaths, macros, useRTTI, visibility,
                  enableModules, moduleCachePath, validateSystemHeaders,
                  clangExtraArgs, clangResourcePath, useObjectiveCARC,
                  useObjectiveCWeakARC) ==
         std::tie(other.platform, other.osVersion, other.language,
                  other.language_std, other.isysroot,
                  other.systemFrameworkPaths, other.frameworkPaths,
                  other.libraryPaths, other.systemIncludePaths,
                  other.includePaths, other.macros, other.useRTTI,
                  other.visibility, other.enableModules, other.moduleCachePath,
                  other.validateSystemHeaders, other.clangExtraArgs,
                  other.clangResourcePath, other.useObjectiveCARC,
                  other.useObjectiveCWeakARC);
}

bool DiagnosticsOptions::operator==(const DiagnosticsOptions &other) const {
  return std::tie(serializeDiagnosticsFile, errorLimit) ==
         std::tie(other.serializeDiagnosticsFile, other.errorLimit);
}

bool TAPIOptions::operator==(const TAPIOptions &other) const {
  return std::tie(generateCodeCoverageSymbols, publicUmbrellaHeaderPath,
                  privateUmbrellaHeaderPath, extraPublicHeaders,
                  extraPrivateHeaders, excludePublicHeaders,
                  excludePrivateHeaders, verifyAgainst, verificationMode,
                  demangle, configurationFile, generateAPI, scanPublicHeaders,
                  scanPrivateHeaders, deleteInputFile, inlinePrivateFrameworks,
                  deletePrivateFrameworks, recordUUIDs, setInstallAPIFlag,
                  print, scanAll) ==
         std::tie(other.generateCodeCoverageSymbols,
                  other.publicUmbrellaHeaderPath,
                  other.privateUmbrellaHeaderPath, other.extraPublicHeaders,
                  other.extraPrivateHeaders, other.excludePublicHeaders,
                  other.excludePrivateHeaders, other.verifyAgainst,
                  other.verificationMode, other.demangle,
                  other.configurationFile, other.generateAPI,
                  other.scanPublicHeaders, other.scanPrivateHeaders,
                  other.deleteInputFile, other.inlinePrivateFrameworks,
                  other.deletePrivateFrameworks, other.recordUUIDs,
                  other.setInstallAPIFlag, other.print, other.scanAll);
}

bool Options::processSnapshotOptions(DiagnosticsEngine &diag,
                                     InputArgList &args) {
  // Handle --snapshot.
  auto *snapshotArg = args.getLastArg(OPT_snapshot);
  if (snapshotArg)
    snapshotOptions.snapshotMode = SnapshotMode::ForceCreate;

  // Handle --snapshot-dir=<dir>.
  if (auto *snapshotDirArg = args.getLastArg(OPT_snapshot_dir))
    snapshotOptions.snapshotOutputDir = snapshotDirArg->getValue();

  if (auto *arg = args.getLastArg(OPT_load_snapshot)) {
    if (snapshotArg) {
      diag.report(clang::diag::err_drv_argument_not_allowed_with)
          << snapshotArg->getAsString(args) << arg->getAsString(args);
      return false;
    }

    snapshotOptions.snapshotMode = SnapshotMode::Load;
    snapshotOptions.snapshotInputPath = arg->getValue();
  }

  if (::getenv("TAPI_SNAPSHOT_CREATE") != nullptr)
    snapshotOptions.snapshotMode = SnapshotMode::ForceCreate;

  if (auto *path = ::getenv("TAPI_SNAPSHOT_DIR"))
    snapshotOptions.snapshotOutputDir = path;

  if (args.getLastArg(OPT_snapshot_use_own_resource_dir))
    snapshotOptions.useOwnResourceDir = true;

  return true;
}

/// \brief Process driver related options.
bool Options::processDriverOptions(DiagnosticsEngine &diag,
                                   InputArgList &args) {
  // Handle -version.
  if (args.hasArg(OPT_version))
    driverOptions.printVersion = true;

  // Handle help options.
  if (args.hasArg(OPT_help_hidden))
    driverOptions.printHelp = driverOptions.printHelpHidden = true;

  if (args.hasArg(OPT_help))
    driverOptions.printHelp = true;

  // Handle output file.
  SmallString<PATH_MAX> outputPath;
  if (auto *arg = args.getLastArg(OPT_output)) {
    outputPath = arg->getValue();
    fm->makeAbsolutePath(outputPath);
    driverOptions.outputPath = outputPath.str();
  }

  // Handle -output-dir (scan mode).
  if (auto *arg = args.getLastArg(OPT_output_dir)) {
    assert(driverOptions.outputPath.empty() &&
           "unexpected value in output path");
    driverOptions.outputPath = arg->getValue();
  }

  // Handle input files.
  if (args.hasArgNoClaim(OPT_INPUT))
    driverOptions.inputs.clear();

  for (const auto &path : args.getAllArgValues(OPT_INPUT)) {
    if (!fm->exists(path)) {
      diag.report(clang::diag::err_drv_no_such_file) << path;
      return false;
    }

    SmallString<PATH_MAX> absolutePath(path);
    fm->makeAbsolutePath(absolutePath);
    driverOptions.inputs.emplace_back(absolutePath.str());
  }

  return true;
}

/// \brief Process archive related options.
bool Options::processArchiveOptions(DiagnosticsEngine &diag,
                                    InputArgList &args) {
  Arg *lastArg = nullptr;

  // Handle --info.
  if ((lastArg = args.getLastArg(OPT_info)))
    archiveOptions.action = ArchiveAction::ShowInfo;

  // Handle --extract <architecture>
  if (auto *arg = args.getLastArg(OPT_extract)) {
    if (lastArg) {
      diag.report(clang::diag::err_drv_argument_not_allowed_with)
          << lastArg->getAsString(args) << arg->getAsString(args);
      return false;
    }

    auto arch = getArchType(arg->getValue());
    if (arch == Architecture::unknown) {
      diag.report(clang::diag::err_drv_invalid_arch_name) << arg->getValue();
      return false;
    }

    archiveOptions.action = ArchiveAction::ExtractArchitecture;
    archiveOptions.arch = arch;
    lastArg = arg;
  }

  // Handle --verify-arch <architecture>.
  if (auto *arg = args.getLastArg(OPT_verify_arch)) {
    if (lastArg) {
      diag.report(clang::diag::err_drv_argument_not_allowed_with)
          << lastArg->getAsString(args) << arg->getAsString(args);
      return false;
    }

    auto arch = getArchType(arg->getValue());
    if (arch == Architecture::unknown) {
      diag.report(clang::diag::err_drv_invalid_arch_name) << arg->getValue();
      return false;
    }

    archiveOptions.action = ArchiveAction::VerifyArchitecture;
    archiveOptions.arch = arch;
    lastArg = arg;
  }

  // Handle --merge.
  if (auto *arg = args.getLastArg(OPT_merge)) {
    if (lastArg) {
      diag.report(clang::diag::err_drv_argument_not_allowed_with)
          << lastArg->getAsString(args) << arg->getAsString(args);
      return false;
    }

    archiveOptions.action = ArchiveAction::Merge;
  }

  // Handle --list-symbols.
  if (auto *arg = args.getLastArg(OPT_listSymbols)) {
    if (lastArg) {
      diag.report(clang::diag::err_drv_argument_not_allowed_with)
          << lastArg->getAsString(args) << arg->getAsString(args);
      return false;
    }

    archiveOptions.action = ArchiveAction::ListSymbols;
  }

  return true;
}

/// \brief Process linker related options.
bool Options::processLinkerOptions(DiagnosticsEngine &diag,
                                   InputArgList &args) {
  // Handle architectures. We need to clear out the set, because it might have
  // been initialized by a snapshot and we want to override the architectures.
  if (args.hasArgNoClaim(OPT_arch))
    linkerOptions.architectures = Architecture::unknown;

  for (auto *arg : args.filtered(OPT_arch)) {
    auto arch = getArchType(arg->getValue());
    if (arch == Architecture::unknown) {
      diag.report(clang::diag::err_drv_invalid_arch_name) << arg->getValue();
      return false;
    }
    linkerOptions.architectures.set(arch);
  }

  // Handle dynamic lib.
  if (args.hasArg(OPT_dynamiclib))
    linkerOptions.isDynamicLibrary = true;

  // Handle install name.
  if (auto *arg = args.getLastArg(OPT_install_name))
    linkerOptions.installName = arg->getValue();

  // Handle current version.
  if (auto *arg = args.getLastArg(OPT_current_version)) {
    auto result = linkerOptions.currentVersion.parse64(arg->getValue());
    if (!result.first) {
      diag.report(diag::err_invalid_current_version) << arg->getValue();
      return false;
    }
    if (result.second)
      diag.report(diag::warn_truncating_current_version) << arg->getValue();
  }

  // Handle compatibility version.
  if (auto *arg = args.getLastArg(OPT_compatibility_version))
    if (!linkerOptions.compatibilityVersion.parse32(arg->getValue())) {
      diag.report(diag::err_invalid_compatibility_version) << arg->getValue();
      return false;
    }

  // Handle allowable clients.
  if (args.hasArgNoClaim(OPT_allowable_client))
    linkerOptions.allowableClients.clear();

  for (auto *arg : args.filtered(OPT_allowable_client))
    linkerOptions.allowableClients.emplace_back(arg->getValue(),
                                                linkerOptions.architectures);
  sort(linkerOptions.allowableClients);

  // Handle reexported libraries.
  if (args.hasArgNoClaim(OPT_reexport_install_name))
    linkerOptions.reexportedLibraries.clear();

  for (auto *arg : args.filtered(OPT_reexport_install_name))
    linkerOptions.reexportedLibraries.emplace_back(arg->getValue(),
                                                   linkerOptions.architectures);
  sort(linkerOptions.reexportedLibraries);

  // Handle application extension safe flag.
  if (::getenv("LD_NO_ENCRYPT") != nullptr)
    linkerOptions.isApplicationExtensionSafe = true;

  if (::getenv("LD_APPLICATION_EXTENSION_SAFE") != nullptr)
    linkerOptions.isApplicationExtensionSafe = true;

  if (auto *arg = args.getLastArg(OPT_fapplication_extension,
                                  OPT_fno_application_extension)) {
    if (arg->getOption().matches(OPT_fapplication_extension))
      linkerOptions.isApplicationExtensionSafe = true;
    else
      linkerOptions.isApplicationExtensionSafe = false;
  }

  return true;
}

/// \brief Process frontend related options.
bool Options::processFrontendOptions(DiagnosticsEngine &diag,
                                     InputArgList &args) {
  // Handle isysroot.
  if (auto *arg = args.getLastArg(OPT_isysroot)) {
    SmallString<PATH_MAX> path(arg->getValue());
    fm->makeAbsolutePath(path);
    if (!fm->exists(path)) {
      diag.report(diag::err_missing_sysroot) << path;
      return false;
    }
    frontendOptions.isysroot = path.str();
  } else if (frontendOptions.isysroot.empty()) {
    // Mirror CLANG and obtain the isysroot from the SDKROOT environment
    // variable, if it wasn't defined by the snapshot or command line.
    if (auto *env = ::getenv("SDKROOT")) {
      // Only use the SDKROOT as the default if it is an absolute path, exists,
      // and it is not the root path.
      if (llvm::sys::path::is_absolute(env) && fm->exists(env) &&
          StringRef(env) != "/")
        frontendOptions.isysroot = env;
    }
  }

  // Handle SYSTEM framework paths.
  if (args.hasArgNoClaim((OPT_iframework)))
    frontendOptions.systemFrameworkPaths.clear();

  for (auto *arg : args.filtered(OPT_iframework))
    frontendOptions.systemFrameworkPaths.emplace_back(arg->getValue());

  // Handle framework paths.
  std::vector<std::string> frameworkPaths;
  for (auto *arg : args.filtered(OPT_F))
    frameworkPaths.emplace_back(arg->getValue());

  // Handle library paths.
  std::vector<std::string> libraryPaths;
  for (auto *arg : args.filtered(OPT_L))
    libraryPaths.emplace_back(arg->getValue());

  /// Construct the search paths for libraries and frameworks.
  // Add default framework/library paths.
  std::vector<std::string> defaultLibraryPaths = {"/usr/lib", "/usr/local/lib"};
  std::vector<std::string> defaultFrameworkPaths = {
      "/Library/Frameworks", "/System/Library/Frameworks"};

  if (!libraryPaths.empty())
    frontendOptions.libraryPaths = libraryPaths;

  for (const auto &libPath : defaultLibraryPaths) {
    SmallString<PATH_MAX> path(frontendOptions.isysroot);
    sys::path::append(path, libPath);
    frontendOptions.libraryPaths.emplace_back(path.str());
  }

  if (!frameworkPaths.empty())
    frontendOptions.frameworkPaths = frameworkPaths;

  for (const auto &frameworkPath : defaultFrameworkPaths) {
    SmallString<PATH_MAX> path(frontendOptions.isysroot);
    sys::path::append(path, frameworkPath);
    frontendOptions.frameworkPaths.emplace_back(path.str());
  }

  // Handle deployment target.
  const std::pair<unsigned, Platform> targets[] = {
      {OPT_macosx_version_min, Platform::OSX},
      {OPT_ios_version_min, Platform::iOS},
      {OPT_tvos_version_min, Platform::tvOS},
      {OPT_watchos_version_min, Platform::watchOS},
      {OPT_bridgeos_version_min, Platform::bridgeOS}};

  const Arg *first = nullptr;
  for (const auto &target : targets) {
    auto *arg = args.getLastArg(target.first);
    if (arg == nullptr)
      continue;

    if (first != nullptr) {
      diag.report(clang::diag::err_drv_argument_not_allowed_with)
          << first->getAsString(args) << arg->getAsString(args);
      return false;
    }

    first = arg;
    frontendOptions.platform = target.second;
    frontendOptions.osVersion = arg->getValue();
  }

  if (frontendOptions.platform == Platform::Unknown) {
    // If no deployment target was specified on the command line, check for
    // environment defines.

    const std::pair<const char *, Platform> targets[] = {
        {"MACOSX_DEPLOYMENT_TARGET", Platform::OSX},
        {"IPHONEOS_DEPLOYMENT_TARGET", Platform::iOS},
        {"TVOS_DEPLOYMENT_TARGET", Platform::tvOS},
        {"WATCHOS_DEPLOYMENT_TARGET", Platform::watchOS},
        {"BRIDGEOS_DEPLOYMENT_TARGET", Platform::bridgeOS}};

    const char *first = nullptr;
    for (const auto &target : targets) {
      auto *env = ::getenv(target.first);
      if (env == nullptr)
        continue;

      if (first != nullptr) {
        diag.report(clang::diag::err_drv_conflicting_deployment_targets)
            << first << target.first;
        return false;
      }

      first = target.first;
      frontendOptions.platform = target.second;
      frontendOptions.osVersion = env;
    }
  }

  // Handle language option.
  if (auto *arg = args.getLastArg(OPT_x)) {
    frontendOptions.language =
        StringSwitch<clang::InputKind::Language>(arg->getValue())
            .Case("c", clang::InputKind::C)
            .Case("c++", clang::InputKind::CXX)
            .Case("objective-c", clang::InputKind::ObjC)
            .Case("objective-c++", clang::InputKind::ObjCXX)
            .Default(clang::InputKind::Unknown);

    if (frontendOptions.language == clang::InputKind::Unknown) {
      diag.report(clang::diag::err_drv_invalid_value) << arg->getAsString(args)
                                                      << arg->getValue();
      return false;
    }
  }

  // Handle ObjC/ObjC++ switch.
  for (auto *arg : args.filtered(OPT_ObjC, OPT_ObjCXX)) {
    if (arg->getOption().matches(OPT_ObjC))
      frontendOptions.language = clang::InputKind::ObjC;
    else
      frontendOptions.language = clang::InputKind::ObjCXX;
  }

  // Handle language std.
  if (auto *arg = args.getLastArg(OPT_std_EQ))
    frontendOptions.language_std = arg->getValue();

  // Handle SYSTEM include paths.
  if (args.hasArgNoClaim(OPT_isystem))
    frontendOptions.systemIncludePaths.clear();

  for (auto *arg : args.filtered(OPT_isystem))
    frontendOptions.systemIncludePaths.emplace_back(arg->getValue());

  // Handle include paths.
  if (args.hasArgNoClaim(OPT_I))
    frontendOptions.includePaths.clear();

  for (auto *arg : args.filtered(OPT_I))
    frontendOptions.includePaths.emplace_back(arg->getValue());

  // Add macros from the command line.
  if (args.hasArgNoClaim(OPT_D) || args.hasArgNoClaim(OPT_U))
    frontendOptions.macros.clear();

  for (auto *arg : args.filtered(OPT_D, OPT_U)) {
    if (arg->getOption().matches(OPT_D))
      frontendOptions.macros.emplace_back(arg->getValue(), /*isUndef=*/false);
    else
      frontendOptions.macros.emplace_back(arg->getValue(), /*isUndef=*/true);
  }

  // Handle RTTI generation.
  if (args.hasArg(OPT_fno_rtti))
    frontendOptions.useRTTI = false;

  // Handle visibility.
  if (auto *arg = args.getLastArg(OPT_fvisibility_EQ))
    frontendOptions.visibility = arg->getValue();

  // Handle module related options.
  if (args.hasArg(OPT_fmodules))
    frontendOptions.enableModules = true;

  if (auto *arg = args.getLastArg(OPT_fmodules_cache_path))
    frontendOptions.moduleCachePath = arg->getValue();

  if (args.hasArg(OPT_fmodules_validate_system_headers))
    frontendOptions.validateSystemHeaders = true;

  // Handle extra arguments for the parser.
  if (args.hasArgNoClaim(OPT_Xparser))
    frontendOptions.clangExtraArgs.clear();

  for (auto *arg : args.filtered(OPT_Xparser))
    frontendOptions.clangExtraArgs.emplace_back(arg->getValue());

  // Handle clang resource path.
  if (frontendOptions.clangResourcePath.empty())
    frontendOptions.clangResourcePath = getClangResourcesPath(*fm);

  // Handle Objective-C ARC
  if (args.hasArg(OPT_fobjc_arc))
    frontendOptions.useObjectiveCARC = true;

  if (args.hasArg(OPT_fobjc_weak))
    frontendOptions.useObjectiveCWeakARC = true;

  return true;
}

/// \brief Process frontend related options.
bool Options::processDiagnosticsOptions(DiagnosticsEngine &diag,
                                        InputArgList &args) {
  // Handle diagnostics file.
  if (auto *arg = args.getLastArg(OPT__serialize_diags))
    diagnosticsOptions.serializeDiagnosticsFile = arg->getValue();

  if (auto *arg = args.getLastArg(OPT_ferror_limit)) {
    if (StringRef(arg->getValue())
            .getAsInteger(10, diagnosticsOptions.errorLimit)) {
      diag.report(clang::diag::err_drv_invalid_int_value)
          << arg->getAsString(args) << arg->getValue();
      return false;
    }
  }

  return true;
}

/// \brief Handle TAPI related options.
bool Options::processTAPIOptions(DiagnosticsEngine &diag, InputArgList &args) {
  // Check if need to generate extra symbols for code coverage.
  if (args.hasArg(OPT_fprofile_instr_generate))
    tapiOptions.generateCodeCoverageSymbols = true;

  // Handle public/private umbrella header.
  if (auto *arg = args.getLastArg(OPT_public_umbrella_header))
    tapiOptions.publicUmbrellaHeaderPath = arg->getValue();

  if (auto *arg = args.getLastArg(OPT_private_umbrella_header))
    tapiOptions.privateUmbrellaHeaderPath = arg->getValue();

  auto &fm = getFileManager();
  auto addHeaderFiles = [&fm, &diag, &args](std::vector<std::string> &headers,
                                            OptSpecifier optID) {
    for (const auto &path : args.getAllArgValues(optID)) {
      if (fm.isDirectory(path, /*CacheFailure=*/false)) {
        auto result = enumerateHeaderFiles(fm, path);
        if (!result) {
          diag.report(diag::err) << path << toString(result.takeError());
          return false;
        }
        for (auto &path : *result)
          headers.emplace_back(path);
      } else
        headers.emplace_back(path);
    }

    return true;
  };

  // Handle extra header directories/files.
  if (args.hasArgNoClaim(OPT_extra_public_header))
    tapiOptions.extraPublicHeaders.clear();

  if (!addHeaderFiles(tapiOptions.extraPublicHeaders, OPT_extra_public_header))
    return false;

  if (args.hasArgNoClaim(OPT_extra_private_header))
    tapiOptions.extraPrivateHeaders.clear();

  if (!addHeaderFiles(tapiOptions.extraPrivateHeaders,
                      OPT_extra_private_header))
    return false;

  // Handle excluded header files.
  if (args.hasArgNoClaim(OPT_exclude_public_header))
    tapiOptions.excludePublicHeaders.clear();

  if (!addHeaderFiles(tapiOptions.excludePublicHeaders,
                      OPT_exclude_public_header))
    return false;

  if (args.hasArgNoClaim(OPT_exclude_private_header))
    tapiOptions.excludePrivateHeaders.clear();

  if (!addHeaderFiles(tapiOptions.excludePrivateHeaders,
                      OPT_exclude_private_header))
    return false;

  // Handle verify against.
  if (auto *arg = args.getLastArg(OPT_verify_against))
    tapiOptions.verifyAgainst = arg->getValue();

  // Handle verification mode.
  if (auto *arg = args.getLastArg(OPT_verify_mode_EQ)) {
    tapiOptions.verificationMode =
        StringSwitch<VerificationMode>(arg->getValue())
            .Case("ErrorsOnly", VerificationMode::ErrorsOnly)
            .Case("ErrorsAndWarnings", VerificationMode::ErrorsAndWarnings)
            .Case("Pedantic", VerificationMode::Pedantic)
            .Default(VerificationMode::Invalid);

    if (tapiOptions.verificationMode == VerificationMode::Invalid) {
      diag.report(clang::diag::err_drv_invalid_value) << arg->getAsString(args)
                                                      << arg->getValue();
      return false;
    }
  }

  // Handel demangling.
  if (args.hasArg(OPT_demangle))
    tapiOptions.demangle = true;

  // Handle -gen.
  if (args.hasArg(OPT_gen))
    tapiOptions.generateAPI = true;

  // Handle configuration file.
  if (auto *arg = args.getLastArg(OPT_config_file))
    tapiOptions.configurationFile = arg->getValue();

  // Handle disabling of header scanning.
  if (args.hasArg(OPT_no_public_headers))
    tapiOptions.scanPublicHeaders = false;

  if (args.hasArg(OPT_no_private_headers))
    tapiOptions.scanPrivateHeaders = false;

  if (!tapiOptions.scanPublicHeaders && !tapiOptions.scanPrivateHeaders) {
    diag.report(diag::err_parsing_disabled);
    return false;
  }

  if (args.hasArg(OPT_deleteInputFile))
    tapiOptions.deleteInputFile = true;

  if (::getenv("TAPI_DELETE_INPUT_FILE") != nullptr)
    tapiOptions.deleteInputFile = true;

  if (args.hasArg(OPT_inlinePrivateFrameworks))
    tapiOptions.inlinePrivateFrameworks = true;

  if (args.hasArg(OPT_deletePrivateFrameworks))
    tapiOptions.deletePrivateFrameworks = true;

  if (args.hasArg(OPT_noUUIDs))
    tapiOptions.recordUUIDs = false;

  if (args.hasArg(OPT_setInstallAPI)) {
    tapiOptions.setInstallAPIFlag = true;
    tapiOptions.recordUUIDs = false;
  }


  if (args.hasArg(OPT_print))
    tapiOptions.print = true;

  if (args.hasArg(OPT_dylibs_only))
    tapiOptions.scanAll = false;

  return true;
}

/// \brief Handle SDKDB Verify related options.
bool Options::processVerifyOptions(DiagnosticsEngine &diag,
                                   InputArgList &args) {
  if (auto *arg = args.getLastArg(OPT_baseline)) {
    verifyOptions.baselinePath = arg->getValue();
  }
  return true;
}

void Options::initOptionsFromSnapshot(const Snapshot &snapshot) {
  command = snapshot.command;
  driverOptions = snapshot.driverOptions;
  archiveOptions = snapshot.archiveOptions;
  linkerOptions = snapshot.linkerOptions;
  frontendOptions = snapshot.frontendOptions;
  diagnosticsOptions = snapshot.diagnosticsOptions;
  tapiOptions = snapshot.tapiOptions;
}

static void updateClangResourceDirFiles(DiagnosticsEngine &diag,
                                        FileManager &fm,
                                        StringRef originalClangResourcePath,
                                        SnapshotFileSystem *vfs) {
  auto clangResourcePath = getClangResourcesPath(fm);
  if (clangResourcePath.empty())
    return;

  auto headers = enumerateHeaderFiles(fm, clangResourcePath);
  if (!headers) {
    diag.report(diag::err) << clangResourcePath
                           << toString(headers.takeError());
    return;
  }

  SmallString<PATH_MAX> snapshotRessourcePath(originalClangResourcePath);
  // Normalize path.
  if (vfs->makeAbsolute(snapshotRessourcePath))
    return;
  sys::path::remove_dots(snapshotRessourcePath, /*remove_dot_dot=*/true);

  // Replace all files in the ressource directory from the snapshot with our
  // own files.
  for (auto &header : *headers) {
    SmallString<PATH_MAX> externalPath(header);
    SmallString<PATH_MAX> srcPath(header);

    // Normalize path.
    if (fm.getVirtualFileSystem()->makeAbsolute(externalPath))
      return;
    sys::path::remove_dots(externalPath, /*remove_dot_dot=*/true);
    sys::path::replace_path_prefix(srcPath, clangResourcePath,
                                   snapshotRessourcePath);

    vfs->addFile(srcPath, externalPath);
  }
}

Options::Options(DiagnosticsEngine &diag, ArrayRef<const char *> argString) {
  // Create the default file manager for all file operations.
  fm = new FileManager(clang::FileSystemOptions());

  // Record the raw arguments.
  globalSnapshot->recordRawArguments(argString);

  table.reset(createDriverOptTable());

  // Program name
  programName = sys::path::stem(argString.front());
  argString = argString.slice(1);

  // Show the umbrella help when no command was specified and no other
  // arguments were passed to tapi.
  if (argString.empty()) {
    driverOptions.printHelp = true;
    return;
  }

  command = getTAPICommand(argString.front());
  if (command != TAPICommand::Driver)
    argString = argString.slice(1);
  auto args = parseArgString(diag, argString, table.get(),
                             getIncludeOptionFlagMasks(command), 0);

  if (diag.hasErrorOccurred())
    return;

  // Process the snaphot options first. They can affect the results of the
  // following options.
  if (!processSnapshotOptions(diag, args))
    goto recordOptions;

  if (!snapshotOptions.snapshotOutputDir.empty())
    globalSnapshot->setRootPath(snapshotOptions.snapshotOutputDir);

  if (snapshotOptions.snapshotMode == SnapshotMode::Load) {
    if (!globalSnapshot->loadSnapshot(snapshotOptions.snapshotInputPath))
      return;
    initOptionsFromSnapshot(*globalSnapshot);

    // The snapshot creates a special mapping file system that we need to use to
    // access the files that are recorded in the snapshot.
    auto fs = globalSnapshot->getVirtualFileSystem();

    if (snapshotOptions.useOwnResourceDir)
      updateClangResourceDirFiles(diag, *fm.get(),
                                  frontendOptions.clangResourcePath, fs.get());

    fm = new FileManager(
        clang::FileSystemOptions{globalSnapshot->getWorkingDirectory()}, fs);
  } else {
    if (snapshotOptions.snapshotMode == SnapshotMode::ForceCreate)
      globalSnapshot->requestSnapshot();

    globalSnapshot->setWorkingDirectory(
        fm->getVirtualFileSystem()->getCurrentWorkingDirectory().get());
  }

  if (!processDriverOptions(diag, args))
    goto recordOptions;

  if (!processArchiveOptions(diag, args))
    goto recordOptions;

  if (!processLinkerOptions(diag, args))
    goto recordOptions;

  if (!processFrontendOptions(diag, args))
    goto recordOptions;

  if (!processDiagnosticsOptions(diag, args))
    goto recordOptions;

  if (!processTAPIOptions(diag, args))
    goto recordOptions;

  if (!processVerifyOptions(diag, args))
    goto recordOptions;

recordOptions:
  globalSnapshot->recordOptions(*this);
}

/// \brief Print umbrella help for tapi.
static void printDriverHelp(bool hidden = false) {
  outs() << "OVERVIEW: " << toolName
         << "\n\n"
            "USAGE: tapi [--version][--help]\n"
            "       tapi <command> [<args>]\n\n"
            "Commands:\n"
            "  archive     Merge or thin text-based stub files\n"
            "  stubify     Create a text-based stub file from a library\n"
            "  installapi  Create a text-based stub file by scanning the "
            "header files\n"
            "  reexport    Create a linker reexport file by scanning the "
            "header files\n"
            "\n";

  if (hidden) {
    outs()
        << "Experimental Commands:\n"
           "  scan          Scan, verify, and generate text-based stub files "
           "for frameworks\n"
           "  sdkdb         Generate SDKDB from SDKContent\n"
           "  sdkdb-verify  Verify SDKDB with a baseline version\n"
           "\n";
  }
  outs()
      << "See 'tapi <command> --help' to read more about a specific command.\n";
}

void Options::printHelp() const {
  if (command == TAPICommand::Driver) {
    printDriverHelp(driverOptions.printHelpHidden);
    return;
  }

  table->PrintHelp(
      outs(),
      (programName + " " + getNameFromTAPICommand(command)).str().c_str(),
      toolName, /*FlagsToInclude=*/getIncludeOptionFlagMasks(command),
      /*FlagsToExclude=*/0, /*ShowAllAliases=*/false);
}

TAPI_NAMESPACE_INTERNAL_END
