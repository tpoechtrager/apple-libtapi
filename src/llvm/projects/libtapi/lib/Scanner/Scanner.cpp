//===- lib/Scanner/Scanner.cpp - TAPI Scanner -------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the TAPI Scanner.
///
//===----------------------------------------------------------------------===//

#include "tapi/Scanner/Scanner.h"
#include "tapi/Core/Architecture.h"
#include "tapi/Core/FileManager.h"
#include "tapi/Core/HeaderFile.h"
#include "tapi/Core/XPISet.h"
#include "tapi/Defines.h"
#include "tapi/LinkerInterfaceFile.h"
#include "tapi/Scanner/APIScanner.h"
#include "clang/Basic/FileManager.h"
#include "clang/Frontend/FrontendOptions.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

using namespace llvm;
using namespace clang;
using namespace clang::tooling;

TAPI_NAMESPACE_INTERNAL_BEGIN

static SmallVectorImpl<char> &operator+=(SmallVectorImpl<char> &includes,
                                         StringRef rhs) {
  includes.append(rhs.begin(), rhs.end());
  return includes;
}

static bool addHeaderInclude(StringRef headerName,
                             clang::InputKind::Language lang,
                             SmallVectorImpl<char> &includes) {
  SmallString<PATH_MAX> name;
  if (!(headerName.startswith("\"") && headerName.endswith("\"")) &&
      !(headerName.startswith("<") && headerName.endswith(">"))) {
    name += "\"";
    name += headerName;
    name += "\"";
  } else
    name += headerName;

  if (lang == clang::InputKind::C || lang == clang::InputKind::CXX)
    includes += "#include ";
  else
    includes += "#import ";
  includes += name;
  includes += "\n";

  return true;
}

static bool addHeaderInclude(const FileEntry *header,
                             clang::InputKind::Language lang,
                             SmallVectorImpl<char> &includes) {
  if (sys::path::is_absolute(header->getName()))
    return addHeaderInclude(header->getName(), lang, includes);

  SmallString<PATH_MAX> absName(header->getName());
  if (sys::fs::make_absolute(absName))
    return false;
  return addHeaderInclude(absName, lang, includes);
}

static StringRef getLanguageOptions(clang::InputKind::Language lang) {
  switch (lang) {
  default:
    llvm_unreachable("Unexpected language option.");
  case clang::InputKind::C:
    return "-xc";
  case clang::InputKind::CXX:
    return "-xc++";
  case clang::InputKind::ObjC:
    return "-xobjective-c";
  case clang::InputKind::ObjCXX:
    return "-xobjective-c++";
  }
}

ParseContext parseHeaders(XPISet *xpi, std::vector<std::string> args,
                          const char *headerContent, FileManager *fm,
                          std::map<const FileEntry *, HeaderType> &files,
                          Architecture arch) {
  ParseContext ctx;
  ctx.xpi = xpi;
  ctx.files = files;
  ctx.arch = arch;

  ToolInvocation invocation(std::move(args), new APIScannerAction(ctx), fm);
  invocation.mapVirtualFile("tapi_autogen_header_includes.h", headerContent);

  ctx.ReturnValue = static_cast<int>(invocation.run());

  return ctx;
}

static StringRef getPlatformName(Platform platform) {
  switch (platform) {
  case Platform::Unknown:
    return "darwin";
  case Platform::OSX:
    return "macosx";
  case Platform::iOS:
    return "ios";
  case Platform::watchOS:
    return "watchos";
  case Platform::tvOS:
    return "tvos";
  case Platform::bridgeOS:
    return "bridgeos";
  }
}

static std::string makeTargetTriple(Architecture arch, Platform platform) {
  return (getArchName(arch) + "-apple-" + getPlatformName(platform)).str();
}

std::unique_ptr<XPISet> Scanner::run(std::unique_ptr<ParsingJob> job) {
  assert(job != nullptr && "Invalid job request");

  // Exists solely for the purpose of lookup of the resource path.
  // This just needs to be some symbol in the binary.
  static int staticSymbol;
  // The driver detects the builtin header path based on the path of the
  // executable.
  auto mainExecutable = sys::fs::getMainExecutable("tapi", &staticSymbol);

  if (job->clangResourcePath.empty()) {
    errs() << "error: couldn't find clang resources.\n";
    return nullptr;
  }

  SmallString<4096> publicHeaderContents;
  SmallString<4096> privateHeaderContents;
  std::map<const FileEntry *, HeaderType> files;

  for (const auto *file : job->publicPreIncludeFiles) {
    if (!addHeaderInclude(file, job->language, publicHeaderContents))
      return nullptr;
  }

  for (const auto *file : job->privatePreIncludeFiles) {
    if (!addHeaderInclude(file, job->language, privateHeaderContents))
      return nullptr;
  }

  for (const auto *file : job->publicHeaderFiles) {
    if (!addHeaderInclude(file, job->language, publicHeaderContents))
      return nullptr;
    files.emplace(file, HeaderType::Public);
  }

  for (const auto *file : job->privateHeaderFiles) {
    if (!addHeaderInclude(file, job->language, privateHeaderContents))
      return nullptr;
    files.emplace(file, HeaderType::Private);
  }

  std::vector<std::string> commonArgs;
  commonArgs.emplace_back(std::move(mainExecutable));
  commonArgs.emplace_back("-resource-dir");
  commonArgs.emplace_back(job->clangResourcePath);
  commonArgs.emplace_back("-fsyntax-only");
  commonArgs.emplace_back("-w");
  commonArgs.emplace_back(getLanguageOptions(job->language));

  if (!job->language_std.empty()) {
    std::string tmp("-std=");
    tmp += job->language_std;
    commonArgs.emplace_back(tmp);
  }

  if (!job->useRTTI)
    commonArgs.emplace_back("-fno-rtti");

  if (!job->visibility.empty()) {
    std::string tmp("-fvisibility=");
    tmp += job->visibility;
    commonArgs.emplace_back(tmp);
  }

  if (job->enableModules)
    commonArgs.emplace_back("-fmodules");

  if (!job->moduleCachePath.empty()) {
    std::string tmp("-fmodules-cache-path=");
    tmp += job->moduleCachePath;
    commonArgs.emplace_back(tmp);
  }

  if (job->validateSystemHeaders)
    commonArgs.emplace_back("-fmodules-validate-system-headers");

  if (job->useObjectiveCARC)
    commonArgs.emplace_back("-fobjc-arc");

  if (job->useObjectiveCWeakARC)
    commonArgs.emplace_back("-fobjc-weak");

  // Add a default macro for TAPI.
  commonArgs.emplace_back("-D__clang_tapi__=1");

  for (auto &macro : job->macros) {
    if (macro.second)
      commonArgs.emplace_back("-U" + macro.first);
    else
      commonArgs.emplace_back("-D" + macro.first);
  }

  if (!job->isysroot.empty())
    commonArgs.emplace_back("-isysroot" + job->isysroot);

  // Add SYSTEM framework search paths.
  for (const auto &path : job->systemFrameworkPaths)
    commonArgs.emplace_back("-iframework" + path);

  // Add SYSTEM header search paths.
  for (const auto &path : job->systemIncludePaths)
    commonArgs.emplace_back("-isystem" + path);

  // Add the framework search paths.
  for (const auto &path : job->frameworkPaths)
    commonArgs.emplace_back("-F" + path);

  // Add the header search paths.
  for (const auto &path : job->includePaths)
    commonArgs.emplace_back("-I" + path);

  // Also add the private framework path, since it is not added by default.
  if (job->isysroot.empty())
    commonArgs.emplace_back("-iframework /System/Library/PrivateFrameworks");
  else {
    SmallString<PATH_MAX> path(job->isysroot);
    sys::path::append(path, "/System/Library/PrivateFrameworks");
    std::string tmp("-iframework");
    tmp += path.str();
    commonArgs.emplace_back(tmp);
  }

  // Add deployment target.
  if (job->platform != Platform::Unknown) {
    std::string tmp;
    switch (job->platform) {
    default:
      llvm_unreachable("Unexpected platform");
    case Platform::OSX:
      tmp = "-mmacosx-version-min=";
      break;
    case Platform::iOS:
      if (job->architectures.hasX86())
        tmp = "-mios-simulator-version-min=";
      else
        tmp = "-miphoneos-version-min=";
      break;
    case Platform::watchOS:
      if (job->architectures.hasX86())
        tmp = "-mwatchos-simulator-version-min=";
      else
        tmp = "-mwatchos-version-min=";
      break;
    case Platform::tvOS:
      if (job->architectures.hasX86())
        tmp = "-mtvos-simulator-version-min=";
      else
        tmp = "-mtvos-version-min=";
      break;
    case Platform::bridgeOS:
      tmp = "-mbridgeos-version-min=";
      break;
    }
    tmp += job->osVersion;
    commonArgs.emplace_back(tmp);
  }

  // Emit diagnostics to a file for IDEs.
  if (!job->serializeDiagnosticsFile.empty()) {
    commonArgs.emplace_back("-serialize-diagnostics");
    commonArgs.emplace_back(job->serializeDiagnosticsFile);
  }

  // Add extra clang arguments.
  for (const auto &arg : job->clangExtraArgs)
    commonArgs.emplace_back(arg);

  commonArgs.emplace_back("tapi_autogen_header_includes.h");

  std::unique_ptr<XPISet> xpiSet(new XPISet);
  for (auto type : {HeaderType::Public, HeaderType::Private}) {
    if ((type == HeaderType::Public) && !job->scanPublicHeaders)
      continue;
    if ((type == HeaderType::Private) && !job->scanPrivateHeaders)
      continue;

    const char *headerContents = nullptr;
    if (type == HeaderType::Public)
      headerContents = publicHeaderContents.c_str();
    else
      headerContents = privateHeaderContents.c_str();

    for (auto arch : job->architectures) {
      std::vector<std::string> finalArgs;
      finalArgs.insert(finalArgs.end(), commonArgs.begin(), commonArgs.end());
      std::string target("--target=");
      target += makeTargetTriple(arch, job->platform);
      finalArgs.emplace_back(target);
      auto result = parseHeaders(xpiSet.get(), finalArgs, headerContents,
                                 job->fileManager, files, arch);

      // The tooling code removes our recording stat cache. We need to re-create
      // it after every invocation, so we keep recording all the files.
      job->fileManager->installStatRecorder();

      if (!result.ReturnValue)
        return nullptr;
    }
  }

  return xpiSet;
}

TAPI_NAMESPACE_INTERNAL_END
