//===--- Snapshot.cpp - Snapshot ------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "tapi/Driver/Snapshot.h"
#include "tapi/Config/Version.h"
#include "tapi/Core/LLVM.h"
#include "tapi/Core/YAML.h"
#include "tapi/Defines.h"
#include "clang/Frontend/FrontendOptions.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/Config/config.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/xxhash.h"
#include <sstream>

using namespace llvm;
using namespace TAPI_INTERNAL;
using clang::InputKind;

using Mapping = std::pair<std::string, uint64_t>;
using Reexports = std::pair<std::string, ArchitectureSet>;
LLVM_YAML_IS_SEQUENCE_VECTOR(InterfaceFileRef)
LLVM_YAML_IS_SEQUENCE_VECTOR(Reexports)
LLVM_YAML_IS_FLOW_SEQUENCE_VECTOR(Macro)
LLVM_YAML_IS_FLOW_SEQUENCE_VECTOR(Triple)
LLVM_YAML_IS_STRING_MAP(uint64_t)

namespace llvm {
namespace yaml {

template <> struct ScalarEnumerationTraits<TAPICommand> {
  static void enumeration(IO &io, TAPICommand &command) {
    io.enumCase(command, "driver", TAPICommand::Driver);
    io.enumCase(command, "archive", TAPICommand::Archive);
    io.enumCase(command, "stubify", TAPICommand::Stubify);
    io.enumCase(command, "installapi", TAPICommand::InstallAPI);
    io.enumCase(command, "reexport", TAPICommand::Reexport);
  }
};

template <> struct ScalarEnumerationTraits<ArchiveAction> {
  static void enumeration(IO &io, ArchiveAction &action) {
    io.enumCase(action, "unknown", ArchiveAction::Unknown);
    io.enumCase(action, "show-info", ArchiveAction::ShowInfo);
    io.enumCase(action, "extract-architecture",
                ArchiveAction::ExtractArchitecture);
    io.enumCase(action, "remove-architecture",
                ArchiveAction::RemoveArchitecture);
    io.enumCase(action, "verify-architecture",
                ArchiveAction::VerifyArchitecture);
    io.enumCase(action, "merge", ArchiveAction::Merge);
  }
};

template <> struct MappingTraits<InterfaceFileRef> {
  static void mapping(IO &io, InterfaceFileRef &ref) {
    io.mapRequired("install-name", ref._installName);
    io.mapOptional("architectures", ref._architectures);
  }
};

template <> struct MappingTraits<Reexports> {
  static void mapping(IO &io, Reexports &ref) {
    io.mapRequired("name", ref.first);
    io.mapOptional("architectures", ref.second);
  }
};

template <> struct ScalarTraits<Triple> {
  static void output(const Triple &value, void * /*unused*/, raw_ostream &os) {
    os << value.str();
  }

  static StringRef input(StringRef scalar, void * /*unused*/, Triple &value) {
    value.setTriple(scalar);
    return {};
  }

  static QuotingType mustQuote(StringRef /*unused*/) {
    return QuotingType::None;
  }
};

template <> struct ScalarTraits<Macro> {
  static void output(const Macro &value, void * /*unused*/, raw_ostream &os) {
    if (value.second)
      os << "-U" << value.first;
    else
      os << "-D" << value.first;
  }

  static StringRef input(StringRef scalar, void * /*unused*/, Macro &value) {
    if (scalar.startswith("-D"))
      value = Macro(scalar.drop_front(2), /*isUndef=*/false);
    else if (scalar.startswith("-U"))
      value = Macro(scalar.drop_front(2), /*isUndef=*/true);
    else
      return "invalid macro";
    return {};
  }

  static QuotingType mustQuote(StringRef /*unused*/) {
    return QuotingType::Single;
  }
};

template <> struct ScalarEnumerationTraits<VerificationMode> {
  static void enumeration(IO &io, VerificationMode &mode) {
    io.enumCase(mode, "invalid", VerificationMode::Invalid);
    io.enumCase(mode, "errors-only", VerificationMode::ErrorsOnly);
    io.enumCase(mode, "errors-and-warnings",
                VerificationMode::ErrorsAndWarnings);
    io.enumCase(mode, "pedantic", VerificationMode::Pedantic);
  }
};

template <> struct MappingTraits<DriverOptions> {
  static void mapping(IO &io, DriverOptions &opts) {
    io.mapOptional("print-version", opts.printVersion, false);
    io.mapOptional("print-help", opts.printHelp, false);
    io.mapOptional("inputs", opts.inputs, {});
    io.mapOptional("output-path", opts.outputPath, std::string());
  }
};

template <> struct MappingTraits<ArchiveOptions> {
  static void mapping(IO &io, ArchiveOptions &opts) {
    io.mapOptional("action", opts.action, ArchiveAction::Unknown);
    io.mapOptional("architecture", opts.arch, Architecture::unknown);
  }
};

template <>
struct MappingContextTraits<LinkerOptions, Snapshot::MappingContext> {
  static void mapping(IO &io, LinkerOptions &opts,
                      Snapshot::MappingContext &ctx) {
    io.mapOptional("architectures", ctx.architectures, ArchitectureSet());
    io.mapOptional("install-name", opts.installName, std::string());
    io.mapOptional("current-version", opts.currentVersion, PackedVersion());
    io.mapOptional("compatibility-version", opts.compatibilityVersion,
                   PackedVersion());
    io.mapOptional("is-dynamic-library", opts.isDynamicLibrary, false);
    io.mapOptional("allowable-clients", opts.allowableClients, {});
    io.mapOptional("reexported-libraries", opts.reexportInstallNames, {});
    io.mapOptional("reexported-libraries2", opts.reexportedLibraries, {});
    io.mapOptional("reexported-library-paths", opts.reexportedLibraryPaths, {});
    io.mapOptional("reexported-frameworks", opts.reexportedFrameworks, {});
    io.mapOptional("is-application-extension-safe",
                   opts.isApplicationExtensionSafe, false);
  }
};

template <>
struct MappingContextTraits<tapi::internal::FrontendOptions,
                            Snapshot::MappingContext> {
  static void mapping(IO &io, tapi::internal::FrontendOptions &opts,
                      Snapshot::MappingContext &ctx) {
    io.mapOptional("platform", ctx.platform, Platform::unknown);
    io.mapOptional("os-version", ctx.osVersion, std::string());
    io.mapOptional("targets", opts.targets, {});
    io.mapOptional("language", opts.language, InputKind::ObjC);
    io.mapOptional("language-std", opts.language_std, std::string());
    io.mapOptional("isysroot", opts.isysroot, std::string());
    io.mapOptional("umbrella", opts.umbrella, std::string());
    io.mapOptional("system-framework-paths", opts.systemFrameworkPaths, {});
    io.mapOptional("system-include-paths", opts.systemIncludePaths, {});
    io.mapOptional("framework-paths", opts.frameworkPaths, {});
    io.mapOptional("library-paths", opts.libraryPaths, {});
    io.mapOptional("include-paths", opts.includePaths, {});
    io.mapOptional("macros", opts.macros, {});
    io.mapOptional("use-rtti", opts.useRTTI, true);
    io.mapOptional("visibility", opts.visibility, std::string());
    io.mapOptional("enable-modules", opts.enableModules, false);
    io.mapOptional("module-cache-path", opts.moduleCachePath, std::string());
    io.mapOptional("validate-system-headers", opts.validateSystemHeaders,
                   false);
    io.mapOptional("use-objc-arc", opts.useObjectiveCARC, false);
    io.mapOptional("use-objc-weak", opts.useObjectiveCWeakARC, false);
    io.mapOptional("clang-extra-args", opts.clangExtraArgs, {});
    io.mapOptional("clang-resource-path", opts.clangResourcePath,
                   std::string());
  }
};

template <> struct MappingTraits<DiagnosticsOptions> {
  static void mapping(IO &io, DiagnosticsOptions &opts) {
    io.mapOptional("serialize-diagnostics-file", opts.serializeDiagnosticsFile,
                   std::string());
    io.mapOptional("error-limit", opts.errorLimit, 0U);
  }
};

template <> struct MappingTraits<TAPIOptions> {
  static void mapping(IO &io, TAPIOptions &opts) {
    io.mapOptional("generate-code-coverage-symbols",
                   opts.generateCodeCoverageSymbols, false);
    io.mapOptional("public-umbrella-header-path", opts.publicUmbrellaHeaderPath,
                   std::string());
    io.mapOptional("private-umbrella-header-path",
                   opts.privateUmbrellaHeaderPath, std::string());
    io.mapOptional("extra-public-headers", opts.extraPublicHeaders, {});
    io.mapOptional("extra-private-headers", opts.extraPrivateHeaders, {});
    io.mapOptional("exclude-public-headers", opts.excludePublicHeaders, {});
    io.mapOptional("exclude-private-headers", opts.excludePrivateHeaders, {});
    io.mapOptional("verify-against", opts.verifyAgainst, std::string());
    io.mapOptional("verification-mode", opts.verificationMode,
                   VerificationMode::ErrorsOnly);
    io.mapOptional("demangle", opts.demangle, false);
    io.mapOptional("scan-public-headers", opts.scanPublicHeaders, true);
    io.mapOptional("scan-private-headers", opts.scanPrivateHeaders, true);
    io.mapOptional("delete-input-file", opts.deleteInputFile, false);
    io.mapOptional("inline-private-frameworks", opts.inlinePrivateFrameworks,
                   false);
    io.mapOptional("delete-private-frameworks", opts.deletePrivateFrameworks,
                   false);
    io.mapOptional("record-uuids", opts.recordUUIDs, true);
    io.mapOptional("set-installapi-flag", opts.setInstallAPIFlag, false);
    io.mapOptional("infer-include-paths", opts.inferIncludePaths, true);
  }
};

template <> struct MappingTraits<Snapshot> {
  static void mapping(IO &io, Snapshot &snapshot) {
    io.mapRequired("tapi-version", snapshot.tapiVersion);
    io.mapRequired("name", snapshot.name);
    io.mapRequired("command", snapshot.command);
    io.mapRequired("working-directory", snapshot.workingDirectory);
    io.mapOptional("raw-args", snapshot.rawArgs);
    io.mapOptional("driver-options", snapshot.driverOptions, DriverOptions());
    io.mapOptional("archive-options", snapshot.archiveOptions,
                   ArchiveOptions());
    io.mapOptionalWithContext("linker-options", snapshot.linkerOptions,
                              LinkerOptions(), snapshot.context);
    io.mapOptionalWithContext("frontend-options", snapshot.frontendOptions,
                              FrontendOptions(), snapshot.context);
    io.mapOptional("diagnostics-options", snapshot.diagnosticsOptions,
                   DiagnosticsOptions());
    io.mapOptional("tapi-options", snapshot.tapiOptions, TAPIOptions());
    io.mapOptional("directories", snapshot.normalizedDirectories);
    io.mapOptional("file-mapping", snapshot.pathToHash);
  }
};

} // end namespace yaml.
} // end namespace llvm.

TAPI_NAMESPACE_INTERNAL_BEGIN

llvm::ManagedStatic<Snapshot> globalSnapshot;

static constexpr const char *filesDirectory = "Files";
static constexpr const char *runScriptsDirectory = "RunScript";

Snapshot::Snapshot() : tapiVersion(getTAPIFullVersion()) {}

Snapshot::~Snapshot() {
  if (!wantSnapshot)
    return;

  writeSnapshot(/*isCrash=*/false);
}

void Snapshot::writeSnapshot(bool isCrash) {
  if (snapshotWritten)
    return;
  SmallString<PATH_MAX> root(rootPath);
  if (auto ec = sys::fs::make_absolute(root)) {
    outs() << root << ": " << ec.message() << "\n";
    return;
  }
  sys::path::remove_dots(root, /*remove_dot_dot=*/true);

  auto perms = sys::fs::perms::owner_all | sys::fs::perms::group_read |
               sys::fs::perms::group_exe | sys::fs::perms::others_read |
               sys::fs::perms::others_exe;

  if (auto ec =
          sys::fs::create_directory(root, /*IgnoreExisting=*/true, perms)) {
    outs() << root << ": " << ec.message() << "\n";
    return;
  }

  sys::path::append(root, name);
  if (auto ec = sys::fs::createUniqueDirectory(root, root)) {
    outs() << root << ": " << ec.message() << "\n";
    return;
  }
  if (auto ec = sys::fs::setPermissions(root, perms)) {
    outs() << root << ": " << ec.message() << "\n";
    return;
  }

  for (auto *directory : {filesDirectory, runScriptsDirectory}) {
    SmallString<PATH_MAX> path = root;
    sys::path::append(path, directory);
    if (auto ec =
            sys::fs::create_directory(path, /*IgnoreExisting=*/true, perms)) {
      outs() << path << ": " << ec.message() << "\n";
      return;
    }
  }

  for (auto &path : files) {
    // Normalize all paths.
    SmallString<PATH_MAX> normalizedPathStorage(path);
    if (auto ec = sys::fs::make_absolute(normalizedPathStorage)) {
      outs() << normalizedPathStorage << ": " << ec.message() << "\n";
      continue;
    }

    sys::path::remove_dots(normalizedPathStorage, /*remove_dot_dot=*/true);
    auto normalizedPath = normalizedPathStorage.str();

    // Skip the file if we have it already hashed and copied.
    if (pathToHash.count(normalizedPath))
      continue;

    if (!sys::fs::exists(normalizedPath))
      continue;

    auto bufferOrErr = MemoryBuffer::getFile(normalizedPath, /*FileSize=*/-1,
                                             /*RequiresNullTerminator=*/false);
    if (auto ec = bufferOrErr.getError())
      continue;

    auto &buffer = bufferOrErr.get();
    auto hash = xxHash64(buffer->getBuffer());

    std::stringstream stream;
    stream << std::hex << hash;
    std::string fileName(stream.str());

    SmallString<PATH_MAX> filePath = root;
    sys::path::append(filePath, filesDirectory, fileName);
    if (auto ec = sys::fs::copy_file(normalizedPath, filePath))
      continue;

    pathToHash[normalizedPath] = hash;
  }

  for (auto &path : directories) {
    // Normalize all paths.
    SmallString<PATH_MAX> normalizedPathStorage(path);
    if (auto ec = sys::fs::make_absolute(normalizedPathStorage)) {
      outs() << normalizedPathStorage << ": " << ec.message() << "\n";
      continue;
    }

    sys::path::remove_dots(normalizedPathStorage, /*remove_dot_dot=*/true);
    auto normalizedPath = normalizedPathStorage.str();

    if (!sys::fs::exists(normalizedPath))
      continue;

    normalizedDirectories.emplace_back(normalizedPath);
  }
  sort(normalizedDirectories);
  auto last =
      std::unique(normalizedDirectories.begin(), normalizedDirectories.end());
  normalizedDirectories.erase(last, normalizedDirectories.end());

  SmallString<PATH_MAX> runScript(root);
  sys::path::append(runScript, runScriptsDirectory, name);
  sys::path::replace_extension(runScript, ".yaml");
  if (auto ec = sys::fs::createUniqueFile(runScript, runScript)) {
    outs() << runScript << ": " << ec.message() << "\n";
    return;
  }

  std::error_code ec;
  raw_fd_ostream out(runScript, ec, sys::fs::OpenFlags::F_Text);
  yaml::Output yout(out);
  yout << *this;
  if (ec) {
    outs() << runScript << ": " << ec.message() << "\n";
    return;
  }

  if (isCrash) {
    outs() << "PLEASE submit a bug report to " BUG_REPORT_URL
              " and include the "
              "crash backtrace and snapshot.\n\n"
              "********************************************************\n\n"
              "PLEASE ATTACH THE FOLLOWING DIRECTORY TO THE BUG REPORT:\n"
           << root << "\n"
           << "********************************************************\n";
  } else {
    outs() << "Snapshot written to " << root << "\n";
  }

  snapshotWritten = true;
}

bool Snapshot::loadSnapshot(StringRef path_) {
  fs = new SnapshotFileSystem();

  SmallString<PATH_MAX> path(path_);
  if (auto ec = sys::fs::make_absolute(path)) {
    outs() << path_ << ": " << ec.message() << "\n";
    return false;
  }

  SmallString<PATH_MAX> runScript;
  SmallString<PATH_MAX> inputPath;
  if (path.endswith(".yaml") &&
      sys::path::parent_path(path).endswith(runScriptsDirectory)) {
    runScript = path;
    inputPath = sys::path::parent_path(sys::path::parent_path(path));
  } else {
    runScript = path;
    sys::path::append(runScript, runScriptsDirectory);
    auto findRunScript = [](StringRef directory) {
      std::error_code ec;
      for (sys::fs::directory_iterator
               it = sys::fs::directory_iterator(directory, ec),
               e;
           it != e; it.increment(ec)) {
        if (ec) {
          outs() << ec.message() << "\n";
          return std::string();
        }
        auto path = StringRef(it->path());
        if (path.endswith(".yaml"))
          return it->path();
      }
      return std::string();
    };
    runScript = findRunScript(runScript);
    inputPath = path;
  }

  if (runScript.empty()) {
    outs() << path << ": "
           << "no such runscript or snapshot directory\n";
    return false;
  }

  auto bufferOr = MemoryBuffer::getFile(runScript);
  if (auto ec = bufferOr.getError()) {
    outs() << runScript << ": " << ec.message() << "\n";
    return false;
  }

  auto buffer = (*bufferOr)->getBuffer();
  yaml::Input yin(buffer);

  yin >> *this;

  if (auto ec = yin.error()) {
    outs() << runScript << ": " << ec.message() << "\n";
    return false;
  }

  for (auto arch : context.architectures) {
    Triple target;
    target.setArchName(getArchName(arch));
    target.setVendor(Triple::Apple);
    auto platform = mapToSim(context.platform, context.architectures.hasX86());
    target.setOSName(getOSAndEnvironmentName(platform, context.osVersion));
    frontendOptions.targets.push_back(target);
  }

  // Create a separate directory for the output files.
  SmallString<PATH_MAX> root(rootPath);
  if (auto ec = sys::fs::make_absolute(root)) {
    outs() << root << ": " << ec.message() << "\n";
    return false;
  }
  sys::path::remove_dots(root, /*remove_dot_dot=*/true);

  if (auto ec = sys::fs::create_directory(root)) {
    outs() << root << ": " << ec.message() << "\n";
    return false;
  }

  sys::path::append(root, name);
  if (auto ec = sys::fs::createUniqueDirectory(root, root)) {
    outs() << root << ": " << ec.message() << "\n";
    return false;
  }
  sys::path::append(root, driverOptions.outputPath);
  driverOptions.outputPath = root.str();

  sys::path::append(inputPath, filesDirectory);
  for (const auto &mapping : pathToHash) {
    SmallString<PATH_MAX> filePath = inputPath;

    std::stringstream stream;
    stream << std::hex << mapping.second;
    std::string hash(stream.str());

    sys::path::append(filePath, hash);

    fs->addFile(mapping.first, filePath);
  }

  for (const auto &path : normalizedDirectories)
    fs->addDirectory(path);

  fs->setCurrentWorkingDirectory(workingDirectory);

  return true;
}

void Snapshot::recordRawArguments(ArrayRef<const char *> args) {
  for (const auto *arg : args)
    rawArgs.emplace_back(arg);
}

void Snapshot::recordOptions(const Options &options) {
  command = options.command;
  driverOptions = options.driverOptions;
  archiveOptions = options.archiveOptions;
  linkerOptions = options.linkerOptions;
  frontendOptions = options.frontendOptions;
  diagnosticsOptions = options.diagnosticsOptions;
  tapiOptions = options.tapiOptions;
}

void Snapshot::recordFile(StringRef path) { files.emplace_back(path); }

void Snapshot::recordDirectory(StringRef path) {
  directories.emplace_back(path);
}

TAPI_NAMESPACE_INTERNAL_END
