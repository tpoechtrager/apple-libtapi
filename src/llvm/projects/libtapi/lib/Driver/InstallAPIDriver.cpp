//===- lib/Driver/InstallAPIDriver.cpp - TAPI Driver ------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the driver for the tapi tool.
///
//===----------------------------------------------------------------------===//

#include "API2XPIConverter.h"
#include "tapi/Core/APIPrinter.h"
#include "tapi/Core/ExtendedInterfaceFile.h"
#include "tapi/Core/HeaderFile.h"
#include "tapi/Core/InterfaceFileManager.h"
#include "tapi/Core/JSONFile.h"
#include "tapi/Core/Path.h"
#include "tapi/Core/Registry.h"
#include "tapi/Core/STLExtras.h"
#include "tapi/Core/Utils.h"
#include "tapi/Core/XPI.h"
#include "tapi/Defines.h"
#include "tapi/Diagnostics/Diagnostics.h"
#include "tapi/Driver/DirectoryScanner.h"
#include "tapi/Driver/Driver.h"
#include "tapi/Driver/HeaderGlob.h"
#include "tapi/Driver/Options.h"
#include "tapi/Driver/Snapshot.h"
#include "tapi/Driver/StatRecorder.h"
#include "tapi/Frontend/Frontend.h"
#include "tapi/LinkerInterfaceFile.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/FileManager.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FileUtilities.h"
#include "llvm/Support/Program.h"
#include "llvm/Support/Regex.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <string>

using namespace llvm;
using namespace llvm::opt;
using namespace clang;

TAPI_NAMESPACE_INTERNAL_BEGIN

static bool verifySymbols(const ExtendedInterfaceFile *apiFile,
                          const ExtendedInterfaceFile *dylibFile,
                          DiagnosticsEngine &diag,
                          VerificationMode verificationMode, bool demangle) {
  diag.setWarningsAsErrors(verificationMode == VerificationMode::Pedantic);

  auto xpiCmp = [](const XPI *lhs, const XPI *rhs) {
    if (lhs->getKind() < rhs->getKind())
      return true;
    if (rhs->getKind() < lhs->getKind())
      return false;

    return lhs->getName() < rhs->getName();
  };

  std::vector<const XPI *> symbols;
  for (const auto *symbol : apiFile->symbols())
    symbols.emplace_back(symbol);
  sort(symbols, xpiCmp);

  for (const auto *hsymbol : symbols) {
    const XPI *dsymbol = nullptr;
    bool haveSymbol =
        dylibFile->contains(hsymbol->getKind(), hsymbol->getName(), &dsymbol);

    if (verificationMode != VerificationMode::ErrorsOnly) {
      if (hsymbol->isUnavailable() && !hsymbol->isObsolete() && haveSymbol)
        diag.report(diag::warn_symbol_unavailable)
            << hsymbol->getAnnotatedName(demangle);
    }

    if (!hsymbol->isExportedSymbol())
      continue;

    if (!haveSymbol) {
      if (hsymbol->isObsolete())
        continue;

      diag.report(diag::err_library_missing_symbol)
          << hsymbol->getAnnotatedName(demangle);
      continue;
    }

    if (hsymbol->isThreadLocalValue() != dsymbol->isThreadLocalValue()) {
      if (hsymbol->isThreadLocalValue())
        diag.report(diag::err_header_symbol_tlv_mismatch)
            << hsymbol->getAnnotatedName(demangle)
            << dsymbol->getAnnotatedName(demangle);
      else
        diag.report(diag::err_dylib_symbol_tlv_mismatch)
            << dsymbol->getAnnotatedName(demangle)
            << hsymbol->getAnnotatedName(demangle);
    }

    if (hsymbol->isWeakDefined() != dsymbol->isWeakDefined()) {
      if (hsymbol->isWeakDefined())
        diag.report(diag::err_header_symbol_weak_mismatch)
            << hsymbol->getAnnotatedName(demangle)
            << dsymbol->getAnnotatedName(demangle);
      else
        diag.report(diag::err_dylib_symbol_weak_mismatch)
            << dsymbol->getAnnotatedName(demangle)
            << hsymbol->getAnnotatedName(demangle);
    }

    if (hsymbol->getArchitectures() == dsymbol->getArchitectures())
      continue;

    diag.report(diag::err_availability_mismatch)
        << hsymbol->getAnnotatedName(demangle) << hsymbol->getArchitectures()
        << dsymbol->getArchitectures();
  }

  // Check for all special linker symbols. They can affect the runtime behavior
  // and are always required to match even for ErrorsOnly mode.
  symbols.clear();
  for (const auto *symbol : dylibFile->exports())
    symbols.emplace_back(symbol);
  sort(symbols, xpiCmp);
  for (const auto *dsymbol : symbols) {
    // Skip normal symbols. We only care about special linker symbols here.
    if (!dsymbol->getName().startswith("$ld$"))
      continue;

    bool hasSymbol = apiFile->contains(dsymbol->getKind(), dsymbol->getName());
    if (hasSymbol)
      continue;

    diag.report(diag::err_header_symbol_missing)
        << dsymbol->getAnnotatedName(demangle);
  }

  if (verificationMode == VerificationMode::ErrorsOnly)
    return !diag.hasErrorOccurred();

  for (const auto *dsymbol : symbols) {
    // Skip special linker symbols. We already checked them.
    if (dsymbol->getName().startswith("$ld$"))
      continue;

    bool hasSymbol = apiFile->contains(dsymbol->getKind(), dsymbol->getName());
    if (hasSymbol)
      continue;

    // The existence of weak-defined RTTI can not always be inferred from the
    // header files, because they can be generated as part of an implementation
    // file.
    // We do not warn about weak-defined RTTI, because this doesn't affect
    // linking and can be ignored.
    if (dsymbol->isWeakDefined() && (dsymbol->getName().startswith("__ZTI") ||
                                     dsymbol->getName().startswith("__ZTS")))
      continue;

    // Do not warn about fragile ObjC classes. Even hidden classes are exported
    // and are required to be exported. They are not needed for linking, so we
    // can ignore them.
    if (dsymbol->getKind() == XPIKind::ObjectiveCClass &&
        dsymbol->getArchitectures() == Architecture::i386)
      continue;

    diag.report(diag::warn_header_symbol_missing)
        << dsymbol->getAnnotatedName(demangle);
  }

  return !diag.hasErrorOccurred();
}

static bool verifyFramework(const ExtendedInterfaceFile *apiFile,
                            const ExtendedInterfaceFile *dylibFile,
                            DiagnosticsEngine &diag,
                            VerificationMode verificationMode, bool demangle) {
  if (apiFile->getPlatform() != dylibFile->getPlatform()) {
    diag.report(diag::err_platform_mismatch)
        << apiFile->getPlatform() << dylibFile->getPlatform();
    return false;
  }

  if (apiFile->getArchitectures() != dylibFile->getArchitectures()) {
    diag.report(diag::err_architecture_mismatch)
        << apiFile->getArchitectures() << dylibFile->getArchitectures();
    return false;
  }

  if (apiFile->getInstallName() != dylibFile->getInstallName()) {
    diag.report(diag::err_install_name_mismatch)
        << apiFile->getInstallName() << dylibFile->getInstallName();
    return false;
  }

  if (apiFile->getCurrentVersion() != dylibFile->getCurrentVersion()) {
    diag.report(diag::err_current_version_mismatch)
        << apiFile->getCurrentVersion() << dylibFile->getCurrentVersion();
    return false;
  }

  if (apiFile->getCompatibilityVersion() !=
      dylibFile->getCompatibilityVersion()) {
    diag.report(diag::err_compatibility_version_mismatch)
        << apiFile->getCompatibilityVersion()
        << dylibFile->getCompatibilityVersion();
    return false;
  }

  if (apiFile->isApplicationExtensionSafe() !=
      dylibFile->isApplicationExtensionSafe()) {
    diag.report(diag::err_appextension_safe_mismatch)
        << (apiFile->isApplicationExtensionSafe() ? "true" : "false")
        << (dylibFile->isApplicationExtensionSafe() ? "true" : "false");
    return false;
  }

  auto compareInterfaceFileRef = [&](const std::vector<InterfaceFileRef> &lhs,
                                     const std::vector<InterfaceFileRef> &rhs,
                                     unsigned diagID_missing,
                                     unsigned diagID_mismatch) {
    if (lhs.size() != rhs.size() || !equal(lhs, rhs)) {
      for (auto ref1 : lhs) {
        auto it = find_if(rhs, [&](const InterfaceFileRef &ref2) {
          return ref1.getInstallName() == ref2.getInstallName();
        });

        if (it == rhs.end()) {
          diag.report(diagID_missing) << "binary file" << ref1;
          return false;
        }

        if (*it != ref1) {
          diag.report(diagID_mismatch) << ref1 << *it;
          return false;
        }
      }
      for (auto ref2 : rhs) {
        auto it = find_if(lhs, [&](const InterfaceFileRef &ref1) {
          return ref1.getInstallName() == ref2.getInstallName();
        });

        if (it == lhs.end()) {
          diag.report(diagID_missing) << "tapi option" << ref2;
          return false;
        }

        if (*it != ref2) {
          llvm_unreachable("this case was already covered above.");
        }
      }
      llvm_unreachable("should have found a difference by now.");
    }
    return true;
  };

  if (!compareInterfaceFileRef(apiFile->reexportedLibraries(),
                               dylibFile->reexportedLibraries(),
                               diag::err_reexported_libraries_missing,
                               diag::err_reexported_libraries_mismatch))
    return false;

  if (!compareInterfaceFileRef(apiFile->allowableClients(),
                               dylibFile->allowableClients(),
                               diag::err_allowable_clients_missing,
                               diag::err_allowable_clients_mismatch))
    return false;

  if (apiFile->getParentUmbrella() != dylibFile->getParentUmbrella()) {
    diag.report(diag::warn_parent_umbrella_mismatch)
        << apiFile->getParentUmbrella() << dylibFile->getParentUmbrella();
  }

  if (dylibFile->isTwoLevelNamespace() == false) {
    diag.report(diag::err_no_twolevel_namespace);
    return false;
  }

  return verifySymbols(apiFile, dylibFile, diag, verificationMode, demangle);
}

static Expected<std::unique_ptr<ExtendedInterfaceFile>>
getCodeCoverageSymbols(DiagnosticsEngine &diag,
                       const std::vector<Triple> &targets,
                       const std::string &isysroot) {
  static int staticSymbol;
  // Try to find clang first in the toolchain. If that fails, then fall-back to
  // the default search PATH.
  auto mainExecutable = sys::fs::getMainExecutable("tapi", &staticSymbol);
  StringRef toolchainBinDir = sys::path::parent_path(mainExecutable);
  auto clangBinary =
      sys::findProgramByName("clang", makeArrayRef(toolchainBinDir));
  if (clangBinary.getError()) {
    diag.report(diag::warn) << "cannot find 'clang' in toolchain directory. "
                               "Looking for 'clang' in PATH instead.";
    clangBinary = sys::findProgramByName("clang");
    if (auto ec = clangBinary.getError())
      return make_error<StringError>("unable to find 'clang' in PATH", ec);
  }

  // Create temporary input and output files.
  SmallString<PATH_MAX> inputFile;
  if (auto ec = sys::fs::createTemporaryFile("code_coverage", "c", inputFile))
    return make_error<StringError>("unable to create temporary input file", ec);
  FileRemover removeInputFile(inputFile);

  SmallString<PATH_MAX> outputFile;
  if (auto ec =
          sys::fs::createTemporaryFile("libcodecoverage", "dylib", outputFile))
    return make_error<StringError>("unable to create temporary output file",
                                   ec);
  FileRemover removeOutputFile(outputFile);

  std::error_code ec;
  raw_fd_ostream input(inputFile, ec, sys::fs::F_None);
  if (ec)
    return make_error<StringError>("cannot open input file", ec);
  input << "static int foo() { return 0; }\n";
  input.close();

  Registry registry;
  registry.addBinaryReaders();

  std::string installDir = toolchainBinDir;
  std::vector<std::unique_ptr<ExtendedInterfaceFile>> files;
  for (const auto &target : targets) {
    const char *clangArgs[] = {"clang",
                               "-target",
                               target.str().c_str(),
                               "-dynamiclib",
                               "-fprofile-instr-generate",
                               "-fcoverage-mapping",
                               "-ccc-install-dir",
                               installDir.c_str(),
                               "-isysroot",
                               isysroot.c_str(),
                               "-o",
                               outputFile.c_str(),
                               inputFile.c_str(),
                               "-v",
                               nullptr};

    SmallString<PATH_MAX> stderrFile;
    if (auto ec = sys::fs::createTemporaryFile("stderr", "txt", stderrFile))
      return make_error<StringError>("unable to create temporary stderr file",
                                     ec);
    FileRemover removeStderrFile(stderrFile);

    const Optional<StringRef> redirects[] = {/*STDIN=*/llvm::None,
                                             /*STDOUT=*/llvm::None,
                                             /*STDERR=*/StringRef(stderrFile)};

    bool failed = sys::ExecuteAndWait(clangBinary.get(), clangArgs,
                                      /*env=*/nullptr, redirects);

    if (failed) {
      auto bufferOr = MemoryBuffer::getFile(stderrFile);
      if (auto ec = bufferOr.getError())
        return make_error<StringError>("unable to read file", ec);

      std::string message = "'clang' invocation failed:\n";
      for (auto *arg : clangArgs) {
        if (arg == nullptr)
          continue;
        message.append(arg).append(1, ' ');
      }
      message.append(1, '\n');
      message.append(bufferOr.get()->getBuffer());

      return make_error<StringError>(
          message, std::make_error_code(std::errc::not_supported));
    }

    auto bufferOr = MemoryBuffer::getFile(outputFile.str());
    if (auto ec = bufferOr.getError())
      return make_error<StringError>("unable to read file", ec);
    auto file =
        registry.readFile(std::move(bufferOr.get()), ReadFlags::Symbols);
    if (!file)
      return file.takeError();
    files.emplace_back(cast<ExtendedInterfaceFile>(file.get().release()));
  }

  // Merge all the interface files into one.
  std::unique_ptr<ExtendedInterfaceFile> output;
  for (auto &file : files) {
    if (!output) {
      output = std::move(file);
      continue;
    }

    auto result = output->merge(file.get(), /*allowArchitectureMerges=*/true);
    if (!result)
      return result.takeError();
    output = std::move(result.get());
  }

  return std::move(output);
}

/// \brief Parses the headers and generate a text-based stub file.
bool Driver::InstallAPI::run(DiagnosticsEngine &diag, Options &opts) {
  auto &fm = opts.getFileManager();

  // Handle targets.
  if (opts.frontendOptions.targets.empty()) {
    diag.report(diag::err_no_target);
    return false;
  }

  // Handle install name.
  if (opts.linkerOptions.installName.empty()) {
    diag.report(diag::err_no_install_name);
    return false;
  }

  SmallString<PATH_MAX> name =
      sys::path::filename(opts.linkerOptions.installName);
  sys::path::replace_extension(name, "");
  globalSnapshot->setName(name);

  // Handle platform.
  if (mapToSinglePlatform(opts.frontendOptions.targets) == Platform::unknown) {
    diag.report(diag::err_no_deployment_target);
    return false;
  }

  diag.setErrorLimit(opts.diagnosticsOptions.errorLimit);

  // Lookup re-exported libraries.
  InterfaceFileManager manager(fm);
  PathSeq frameworkSearchPaths;
  std::vector<InterfaceFileRef> reexportedLibraries;
  std::vector<const InterfaceFileBase *> reexportedLibraryFiles;
  for (auto &path : opts.frontendOptions.systemFrameworkPaths)
    frameworkSearchPaths.emplace_back(path);
  for (auto &path : opts.frontendOptions.frameworkPaths)
    frameworkSearchPaths.emplace_back(path);

  for (auto &it : opts.linkerOptions.reexportedLibraries) {
    auto name = "lib" + it.first + ".dylib";
    auto path =
        findLibrary(name, fm, {}, opts.frontendOptions.libraryPaths, {});
    if (path.empty()) {
      diag.report(diag::err_cannot_find) << "re-exported library" << it.first;
      return false;
    }

    auto file = manager.readFile(path);
    if (!file) {
      diag.report(diag::err_cannot_read_file)
          << path << toString(file.takeError());
      return false;
    }

    reexportedLibraries.emplace_back(file.get()->getInstallName(), it.second);
    reexportedLibraryFiles.emplace_back(file.get());
  }

  for (auto &it : opts.linkerOptions.reexportedLibraryPaths) {
    auto file = manager.readFile(it.first);
    if (!file) {
      diag.report(diag::err_cannot_read_file)
          << it.first << toString(file.takeError());
      return false;
    }

    reexportedLibraries.emplace_back(file.get()->getInstallName(), it.second);
    reexportedLibraryFiles.emplace_back(file.get());
  }

  for (auto &it : opts.linkerOptions.reexportedFrameworks) {
    auto name = it.first + ".framework/" + it.first;
    auto path = findLibrary(name, fm, frameworkSearchPaths, {}, {});
    if (path.empty()) {
      diag.report(diag::err_cannot_find) << "re-exported framework" << it.first;
      return false;
    }

    auto file = manager.readFile(path);
    if (!file) {
      diag.report(diag::err_cannot_read_file)
          << path << toString(file.takeError());
      return false;
    }

    reexportedLibraries.emplace_back(file.get()->getInstallName(), it.second);
    reexportedLibraryFiles.emplace_back(file.get());
  }

  FrontendJob job;
  job.workingDirectory = globalSnapshot->getWorkingDirectory();
  job.cacheFactory = newFileSystemStatCacheFactory<StatRecorder>();
  job.vfs = fm.getVirtualFileSystem();
  job.language = opts.frontendOptions.language;
  job.language_std = opts.frontendOptions.language_std;
  job.useRTTI = opts.frontendOptions.useRTTI;
  job.visibility = opts.frontendOptions.visibility;
  job.isysroot = opts.frontendOptions.isysroot;
  job.macros = opts.frontendOptions.macros;
  job.systemFrameworkPaths = opts.frontendOptions.systemFrameworkPaths;
  job.systemIncludePaths = opts.frontendOptions.systemIncludePaths;
  job.frameworkPaths = opts.frontendOptions.frameworkPaths;
  job.includePaths = opts.frontendOptions.includePaths;
  job.clangExtraArgs = opts.frontendOptions.clangExtraArgs;
  job.enableModules = opts.frontendOptions.enableModules;
  job.moduleCachePath = opts.frontendOptions.moduleCachePath;
  job.validateSystemHeaders = opts.frontendOptions.validateSystemHeaders;
  job.clangResourcePath = opts.frontendOptions.clangResourcePath;
  job.useObjectiveCARC = opts.frontendOptions.useObjectiveCARC;
  job.useObjectiveCWeakARC = opts.frontendOptions.useObjectiveCWeakARC;

  if (opts.driverOptions.inputs.empty()) {
    diag.report(clang::diag::err_drv_no_input_files);
    return false;
  }

  const FileEntry *jsonFile = nullptr;
  for (const auto &path : opts.driverOptions.inputs) {
    if (sys::path::extension(path) == ".json") {
      if (auto *file = fm.getFile(path)) {
        jsonFile = file;
        break;
      }
    }
  }

  HeaderSeq headerFiles;
  std::string frameworkName;
  if (jsonFile == nullptr) {
    //
    // Scan through the directories and create a list of all found frameworks.
    //
    DirectoryScanner scanner(fm, diag);
    scanner.setScanDylibLocations(opts.linkerOptions.isDynamicLibrary);

    std::vector<Framework> frameworks;
    for (const auto &path : opts.driverOptions.inputs) {
      if (fm.isDirectory(path, /*CacheFailure=*/false)) {
        SmallString<PATH_MAX> normalizedPath(path);
        fm.getVirtualFileSystem()->makeAbsolute(normalizedPath);
        sys::path::remove_dots(normalizedPath, /*remove_dot_dot=*/true);
        if (!scanner.scan(normalizedPath, frameworks))
          return false;
      } else {
        diag.report(diag::err_no_directory) << path;
        return false;
      }
    }

    if (frameworks.empty()) {
      diag.report(diag::err_no_framework);
      return false;
    }

    if (frameworks.size() > 1) {
      diag.report(diag::err_more_than_one_framework);
      return false;
    }

    auto *framework = &frameworks.back();

    // Only infer framework path when modules are enabled.
    if (opts.frontendOptions.enableModules) {
      job.frameworkPaths.insert(job.frameworkPaths.begin(),
                                sys::path::parent_path(framework->getPath()));
    }

    if (!framework->_versions.empty())
      framework = &framework->_versions.back();

    frameworkName = sys::path::stem(framework->getName());
    for (const auto &header : framework->_headerFiles) {
      auto *file = fm.getFile(header.fullPath);
      if (!file) {
        if (header.type == HeaderType::Public)
          diag.report(diag::err_no_such_public_header_file) << header.fullPath;
        else
          diag.report(diag::err_no_such_private_header_file) << header.fullPath;
        return false;
      }
      headerFiles.emplace_back(header);
    }

    // Only use system style includes when modules are enabled.
    if (opts.frontendOptions.enableModules) {
      if (!framework->isDynamicLibrary) {
        for (auto &header : headerFiles)
          header.includeName = "<" + frameworkName + header.relativePath + ">";
      }
    }
  } else {
    Registry registry;
    auto reader = make_unique<YAMLReader>();
    reader->add(std::unique_ptr<DocumentHandler>(
        new json_file::v1::YAMLDocumentHandler));
    registry.add(std::move(reader));

    auto bufferOr = fm.getBufferForFile(jsonFile);
    if (auto ec = bufferOr.getError()) {
      diag.report(diag::err_cannot_read_file)
          << jsonFile->getName() << ec.message();
      return false;
    }

    auto file = registry.readFile(std::move(bufferOr.get()));
    if (!file) {
      diag.report(diag::err_cannot_read_file)
          << jsonFile->getName() << toString(file.takeError());
      return false;
    }
    auto *json = cast<JSONFile>(file.get().get());

    for (const auto &header : json->headers) {
      if (!fm.exists(header.path)) {
        if (header.type == HeaderType::Public)
          diag.report(diag::err_no_such_public_header_file) << header.path;
        else
          diag.report(diag::err_no_such_private_header_file) << header.path;
        return false;
      }

      headerFiles.emplace_back(header.path, header.type);
    }
  }

  for (const auto &path : opts.tapiOptions.extraPublicHeaders) {
    if (fm.exists(path)) {
      SmallString<PATH_MAX> fullPath(path);
      fm.makeAbsolutePath(fullPath);
      headerFiles.emplace_back(fullPath, HeaderType::Public);
      headerFiles.back().isExtra = true;
    } else {
      diag.report(diag::err_no_such_public_header_file) << path;
      return false;
    }
  }

  for (const auto &path : opts.tapiOptions.extraPrivateHeaders) {
    if (fm.exists(path)) {
      SmallString<PATH_MAX> fullPath(path);
      fm.makeAbsolutePath(fullPath);
      headerFiles.emplace_back(fullPath, HeaderType::Private);
      headerFiles.back().isExtra = true;
    } else {
      diag.report(diag::err_no_such_private_header_file) << path;
      return false;
    }
  }

  std::vector<std::unique_ptr<HeaderGlob>> excludeHeaderGlobs;
  std::set<const FileEntry *> excludeHeaderFiles;
  auto parseGlobs = [&](const PathSeq &paths, HeaderType type,
                        unsigned diagID) {
    for (const auto &str : paths) {
      auto glob = HeaderGlob::create(str, type);
      if (glob)
        excludeHeaderGlobs.emplace_back(std::move(glob.get()));
      else {
        consumeError(glob.takeError());
        if (auto file = fm.getFile(str))
          excludeHeaderFiles.emplace(file);
        else {
          diag.report(diagID) << str;
          return false;
        }
      }
    }
    return true;
  };

  if (!parseGlobs(opts.tapiOptions.excludePublicHeaders, HeaderType::Public,
                  diag::err_no_such_public_header_file))
    return false;

  if (!parseGlobs(opts.tapiOptions.excludePrivateHeaders, HeaderType::Private,
                  diag::err_no_such_private_header_file))
    return false;

  for (auto &header : headerFiles) {
    for (auto &glob : excludeHeaderGlobs)
      if (glob->match(header))
        header.isExcluded = true;
  }

  if (!excludeHeaderFiles.empty()) {
    for (auto &header : headerFiles) {
      const auto *file = fm.getFile(header.fullPath);
      if (excludeHeaderFiles.count(file))
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

  const auto &publicUmbrellaHeaderPath =
      opts.tapiOptions.publicUmbrellaHeaderPath;
  if (!publicUmbrellaHeaderPath.empty()) {
    auto escapedString = Regex::escape(publicUmbrellaHeaderPath);
    Regex umbrellaRegex(escapedString);

    if (!matchAndMarkUmbrella(headerFiles, umbrellaRegex, HeaderType::Public)) {
      diag.report(diag::err_no_such_public_umbrella_header_file)
          << publicUmbrellaHeaderPath;
      return false;
    }
  } else if (!frameworkName.empty()) {
    auto umbrellaName = Regex::escape(frameworkName) + "\\.h";
    Regex umbrellaRegex(umbrellaName);

    matchAndMarkUmbrella(headerFiles, umbrellaRegex, HeaderType::Public);
  }

  const auto &privateUmbrellaHeaderPath =
      opts.tapiOptions.privateUmbrellaHeaderPath;
  if (!privateUmbrellaHeaderPath.empty()) {
    auto escapedString = Regex::escape(privateUmbrellaHeaderPath);
    Regex umbrellaRegex(escapedString);

    if (!matchAndMarkUmbrella(headerFiles, umbrellaRegex,
                              HeaderType::Private)) {
      diag.report(diag::err_no_such_private_umbrella_header_file)
          << privateUmbrellaHeaderPath;
      return false;
    }
  } else if (!frameworkName.empty()) {
    auto umbrellaName = Regex::escape(frameworkName) + "[_]?Private\\.h";
    Regex umbrellaRegex(umbrellaName);

    matchAndMarkUmbrella(headerFiles, umbrellaRegex, HeaderType::Private);
  }

  // Infer additional include paths.
  std::set<std::string> inferredIncludePaths;
  if (opts.tapiOptions.inferIncludePaths) {
    for (const auto &header : headerFiles) {
      if (header.isExcluded)
        continue;
      inferredIncludePaths.insert(sys::path::parent_path(header.fullPath));

      auto n = header.fullPath.rfind("/include/");
      if (n == std::string::npos)
        continue;
      auto path = header.fullPath.substr(0, n + 8);
      inferredIncludePaths.insert(path);
    }
  }

  job.includePaths.insert(job.includePaths.begin(),
                          inferredIncludePaths.begin(),
                          inferredIncludePaths.end());

  // Only sort the headers for framework that didn't have a json input file.
  // Fixme: Need to fix all projects that still depend on this behavior.
  if (jsonFile == nullptr) {
    std::stable_sort(headerFiles.begin(), headerFiles.end());
  }
  job.headerFiles = headerFiles;

  std::vector<Triple> allTargets;
  allTargets.insert(allTargets.end(), opts.frontendOptions.targets.begin(),
                    opts.frontendOptions.targets.end());

  std::vector<FrontendContext> frontendResults;
  for (auto &target : allTargets) {
    job.target = target;
    for (auto type : {HeaderType::Public, HeaderType::Private}) {
      job.type = type;
      auto result = runFrontend(job);
      if (!result)
        return false;
      frontendResults.emplace_back(std::move(result.getValue()));
    }
  }

  if (opts.tapiOptions.printAfter == "frontend") {
    APIPrinter printer(errs());
    for (auto &result : frontendResults) {
      errs() << "triple:" << result.target.str() << "\n";
      result.visit(printer);
      errs() << "\n";
    }
  }

  auto headerSymbols = make_unique<XPISet>();
  for (auto &result : frontendResults) {
    API2XPIConverter converter(headerSymbols.get(), result.target);
    result.visit(converter);
  }

  auto scanFile = make_unique<ExtendedInterfaceFile>(std::move(headerSymbols));

  if (opts.tapiOptions.printAfter == "xpi")
    scanFile->printSymbols(ArchitectureSet::All());

  scanFile->setFileType(opts.tapiOptions.fileType);
  scanFile->setPlatform(mapToSinglePlatform(allTargets));
  scanFile->setArchitectures(mapToArchitectureSet(allTargets));
  scanFile->setInstallName(opts.linkerOptions.installName);
  scanFile->setCurrentVersion(opts.linkerOptions.currentVersion);
  scanFile->setCompatibilityVersion(opts.linkerOptions.compatibilityVersion);
  scanFile->setTwoLevelNamespace();
  scanFile->setApplicationExtensionSafe(
      opts.linkerOptions.isApplicationExtensionSafe);
  scanFile->setInstallAPI();
  for (const auto &lib : opts.linkerOptions.allowableClients)
    scanFile->addAllowableClient(lib.getInstallName(), lib.getArchitectures());
  for (const auto &lib : opts.linkerOptions.reexportInstallNames)
    scanFile->addReexportedLibrary(lib.getInstallName(),
                                   lib.getArchitectures());
  for (const auto &lib : reexportedLibraries)
    scanFile->addReexportedLibrary(lib.getInstallName(),
                                   lib.getArchitectures());
  scanFile->setParentUmbrella(opts.frontendOptions.umbrella);

  // Remove symbols that come from re-exported frameworks.
  for (const auto &it : reexportedLibraryFiles) {
    if (const auto *file = dyn_cast<ExtendedInterfaceFile>(it)) {
      for (const auto &sym : file->exports())
        scanFile->removeSymbol(sym->getKind(), sym->getName());
    } else if (const auto *file = dyn_cast<InterfaceFile>(it)) {
      for (const auto &sym : file->exports())
        scanFile->removeSymbol(sym->getKind(), sym->getName());
    }
  }

  // When code coverage is enabled we need to generate extra symbols manually.
  // These symbols are defined in libclang_rt.profile_*.a and are pulled in by
  // clang when -fprofile-instr-generate is specified on the command line.
  //
  // This needs to happen after we removed the re-exported library symbols, or
  // we will remove the code coverage symbols too.
  if (opts.tapiOptions.generateCodeCoverageSymbols) {
    auto file =
        getCodeCoverageSymbols(diag, allTargets, opts.frontendOptions.isysroot);
    if (!file) {
      diag.report(diag::err) << "could not generate coverage symbols"
                             << toString(file.takeError());
      return false;
    }
    for (const auto *symbol : file.get()->exports())
      scanFile->addSymbol(symbol->getKind(), symbol->getName(),
                          symbol->getArchitectures(), symbol->getSymbolFlags(),
                          symbol->getAccess());
  }

  if (!opts.tapiOptions.verifyAgainst.empty()) {
    auto file = manager.readFile(opts.tapiOptions.verifyAgainst);
    if (!file) {
      diag.report(diag::err_cannot_read_file)
          << opts.tapiOptions.verifyAgainst << toString(file.takeError());
      return false;
    }

    auto *dylib = cast<ExtendedInterfaceFile>(file.get());
    if (!verifyFramework(scanFile.get(), dylib, diag,
                         opts.tapiOptions.verificationMode,
                         opts.tapiOptions.demangle))
      return false;

    // Clear the installapi flag.
    scanFile->setInstallAPI(false);

    // Record the UUIDs from the dynamic library.
    if (opts.tapiOptions.recordUUIDs)
      for (auto &uuid : dylib->uuids())
        scanFile->addUUID(uuid.first, uuid.second);
  }

  if (opts.driverOptions.outputPath.empty()) {
    SmallString<PATH_MAX> path;
    if (auto ec = sys::fs::current_path(path)) {
      diag.report(diag::err) << path << ec.message();
      return false;
    }
    auto targetName = sys::path::stem(opts.linkerOptions.installName);
    sys::path::append(path, targetName);
    TAPI_INTERNAL::replace_extension(path, ".tbd");
    opts.driverOptions.outputPath = path.str();
  }

  SmallString<PATH_MAX> outputDir(opts.driverOptions.outputPath);
  sys::path::remove_filename(outputDir);
  auto ec = sys::fs::create_directories(outputDir);
  if (ec) {
    diag.report(diag::err_cannot_create_directory) << outputDir << ec.message();
    return false;
  }

  auto interface = make_unique<InterfaceFile>(std::move(*scanFile.get()));
  auto result =
      manager.writeFile(interface.get(), opts.driverOptions.outputPath);
  if (result) {
    diag.report(diag::err_cannot_write_file)
        << opts.driverOptions.outputPath << toString(std::move(result));
    return false;
  }
  globalSnapshot->recordFile(opts.driverOptions.outputPath);

  return true;
}

TAPI_NAMESPACE_INTERNAL_END
