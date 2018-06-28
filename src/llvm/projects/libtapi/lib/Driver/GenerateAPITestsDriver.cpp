//===- GenerateAPITestsDriver.cpp - TAPI API Test Generator -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the API test generator.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/Architecture.h"
#include "tapi/Core/Framework.h"
#include "tapi/Core/HeaderFile.h"
#include "tapi/Core/XPI.h"
#include "tapi/Core/YAMLReaderWriter.h"
#include "tapi/Defines.h"
#include "tapi/Driver/Diagnostics.h"
#include "tapi/Driver/DirectoryScanner.h"
#include "tapi/Driver/Driver.h"
#include "tapi/Driver/Options.h"
#include "tapi/Scanner/Scanner.h"
#include "clang/Driver/DriverDiagnostic.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/Regex.h"
#include "llvm/Support/YAMLTraits.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

using namespace llvm;
using namespace clang;

namespace {
struct Issue {
  std::string radar;
  std::vector<std::string> symbolNames;
};
} // end anonymous namespace.

LLVM_YAML_IS_SEQUENCE_VECTOR(Issue)

namespace llvm {
namespace yaml {

template <> struct MappingTraits<Issue> {
  static void mapping(IO &io, Issue &issue) {
    io.mapRequired("radar", issue.radar);
    io.mapRequired("symbols", issue.symbolNames);
  }
};

} // end namespace yaml.
} // end namespace llvm.

static std::vector<Issue> issues;

TAPI_NAMESPACE_INTERNAL_BEGIN

static void emitTestForGlobalSymbol(raw_ostream &os, const XPI *symbol,
                                    Architecture arch, int n = 0) {
  auto availability = symbol->getAvailabilityInfo(arch);
  if (!availability.hasValue())
    return;

  if (availability->_unavailable)
    return;

  auto name = symbol->getName().drop_front(1);
  auto varName = ("addressOf" + name).str();
  os.indent(n) << "const void *" << varName << " = &" << name << ";\n";
  os.indent(n + 0) << "if (" << varName << " == 0)\n";
  os.indent(n + 2) << "check(osVersion, @\"" << availability->_introduced
                   << "\", \"" << name << "\");\n";
}

static void emitTestForObjCClass(raw_ostream &os, const XPI *symbol,
                                 Architecture arch, int n = 0) {
  auto availability = symbol->getAvailabilityInfo(arch);
  if (!availability.hasValue())
    return;

  if (availability->_unavailable)
    return;

  os.indent(n + 0) << "if (![" << symbol->getName() << " class])\n";
  os.indent(n + 2) << "check(osVersion, @\"" << availability->_introduced
                   << "\", \"" << symbol->getName() << "\");\n";
}

static void emitTestForSymbol(raw_ostream &os, const XPI *symbol,
                              Architecture arch, int n = 0) {
  auto it = find_if(issues, [symbol](const Issue &issue) {
    auto it =
        find_if(issue.symbolNames, [symbol](const std::string &symbolName) {
          return symbolName == symbol->getName();
        });
    return it != issue.symbolNames.end();
  });
  if (it != issues.end())
    os.indent(n) << "/* " << it->radar << "\n";

  switch (symbol->getKind()) {
  default:
    llvm_unreachable("unexpected XPI kind");
  case XPIKind::GlobalSymbol:
    emitTestForGlobalSymbol(os, symbol, arch, n);
    break;
  case XPIKind::ObjectiveCClass:
    emitTestForObjCClass(os, symbol, arch, n);
    break;
  case XPIKind::ObjectiveCClassEHType:
    // emitTestForObjCClass(os, symbol, arch, n);
    break;
  case XPIKind::ObjectiveCInstanceVariable:
    // skip for now.
    break;
  }

  if (it != issues.end())
    os.indent(n) << "*/\n";

  os << "\n\n";
}

static bool writeAPITest(const Framework &framework, Architecture arch,
                         std::string umbrellaFramework = std::string()) {
  auto frameworkName = sys::path::stem(framework.getName());
  if (umbrellaFramework.empty())
    umbrellaFramework = frameworkName;
  for (auto &sub : framework._subFrameworks) {
    if (!writeAPITest(sub, arch, umbrellaFramework))
      return false;
  }

  if (framework._headerFiles.empty())
    return true;

  if (!framework._headerSymbols)
    return true;

  std::error_code ec;
  auto fileName = (getArchName(arch) + "/run_" + getArchName(arch) + "_" +
                   frameworkName + ".m")
                      .str();
  raw_fd_ostream os(fileName, ec, sys::fs::OpenFlags::F_None);
  if (ec) {
    errs() << "error: " << ec.message() << ": " << fileName << "\n";
    return false;
  }

  os << "@import Foundation;\n";
  os << "@import " << umbrellaFramework << ";\n\n";

  if (frameworkName == "AVFoundation")
    os << "#import <AVFoundation/AVAssetDownloadStorageManager.h>\n\n";
  else if (frameworkName == "AudioToolbox")
    os << "#import <AudioToolbox/AudioCodec.h>\n\n";
  else if (frameworkName == "CoreTelephony") {
    os << "#import <CoreTelephony/CTCall.h>\n";
    os << "#import <CoreTelephony/CTTelephonyNetworkInfo.h>\n";
    os << "#import <CoreTelephony/CTSubscriber.h>\n";
    os << "#import <CoreTelephony/CTCallCenter.h>\n";
    os << "#import <CoreTelephony/CTCarrier.h>\n";
    os << "#import <CoreTelephony/CTCellularData.h>\n";
    os << "#import <CoreTelephony/CTSubscriberInfo.h>\n\n";
  } else if (frameworkName == "CoreVideo")
    os << "#import <CoreVideo/CVHostTime.h>\n\n";
  else if (frameworkName == "PhotosUI")
    os << "#import <PhotosUI/PHEditingExtensionContext.h>\n\n";
  else if (frameworkName == "SystemConfiguration")
    os << "#import <SystemConfiguration/CaptiveNetwork.h>\n\n";
  else if (frameworkName == "WebKit") {
    os << "#import <WebKit/WKHTTPCookieStore.h>\n";
    os << "#import <WebKit/WKSnapshotConfiguration.h>\n\n";
  }

  for (const auto &sub : framework._subFrameworks) {
    if (sub._headerFiles.empty())
      continue;

    if (!sub._headerSymbols)
      continue;

    auto frameworkName = sys::path::stem(sub.getName());
    os.indent(0) << "extern void run" + frameworkName + "();\n";
  }

  os.indent(0) << "extern NSString *osVersion;\n";
  os.indent(0) << "extern void check(NSString *osVersion, NSString "
                  "*introduced, const char *name);\n\n";
  os.indent(0) << "void run" + frameworkName + "() {\n";

  for (const auto &sub : framework._subFrameworks) {
    if (sub._headerFiles.empty())
      continue;

    if (!sub._headerSymbols)
      continue;

    auto frameworkName = sys::path::stem(sub.getName());
    os.indent(2) << "run" + frameworkName + "();\n";
  }

  for (const auto *symbol : framework._headerSymbols->exports())
    emitTestForSymbol(os, symbol, arch, 2);

  os.indent(0) << "}\n";

  os.close();
  if (ec) {
    errs() << "error: " << ec.message() << ": " << fileName << "\n";
    return false;
  }

  return true;
}

static bool writeAPITest(const std::vector<Framework> &frameworks,
                         Architecture arch) {
  if (auto ec = sys::fs::create_directory(getArchName(arch))) {
    errs() << "error: " << ec.message() << ": " << getArchName(arch);
    return false;
  }

  for (const auto &framework : frameworks)
    if (!writeAPITest(framework, arch))
      return false;

  std::error_code ec;
  auto fileName =
      (getArchName(arch) + "/main_" + getArchName(arch) + ".m").str();
  raw_fd_ostream os(fileName, ec, sys::fs::OpenFlags::F_None);
  if (ec) {
    errs() << "error: " << ec.message() << ": " << fileName;
    return false;
  }
  os.indent(0) << "@import UIKit;\n\n";
  os.indent(0) << "NSString *osVersion;\n";
  os.indent(0) << "BOOL hasError = false;\n\n";
  os.indent(0) << "void check(NSString *osVersion, NSString *introduced, const "
                  "char *name) {\n";
  os.indent(2) << "if ([osVersion compare:introduced options:NSNumericSearch] "
                  "!= NSOrderedAscending) {\n";
  os.indent(4) << "NSLog(@\"%s is not available\", name);\n";
  os.indent(4) << "hasError = true;\n";
  os.indent(2) << "}\n";
  os.indent(0) << "}\n\n";

  for (const auto &framework : frameworks) {
    if (framework._headerFiles.empty())
      continue;

    if (!framework._headerSymbols)
      continue;

    auto frameworkName = sys::path::stem(framework.getName());
    os.indent(0) << "extern void run" + frameworkName + "();\n";
  }

  os.indent(0) << "\n";
  os.indent(0) << "int main(int argc, char *argv[]) {\n";
  os.indent(2) << "osVersion = [[UIDevice currentDevice] systemVersion];\n\n";

  for (const auto &framework : frameworks) {
    if (framework._headerFiles.empty())
      continue;

    if (!framework._headerSymbols)
      continue;

    auto frameworkName = sys::path::stem(framework.getName());
    os.indent(2) << "run" + frameworkName + "();\n";
  }

  os.indent(2) << "\n";
  os.indent(2) << "return hasError ? 1 : 0;\n";
  os.indent(0) << "}\n";

  os.close();
  if (ec) {
    errs() << "error: " << ec.message() << ": " << fileName;
    return false;
  }

  return true;
}

static bool parseFramework(Framework &framework, Options &opts,
                           DiagnosticsEngine &diag) {
  for (auto &sub : framework._subFrameworks)
    if (!parseFramework(sub, opts, diag))
      return false;

  if (framework._headerFiles.empty())
    return true;

  outs() << "Parsing " << framework.getName() << "\n";
  // Setup the header scanning job.
  auto &fm = opts.getFileManager();
  auto job = make_unique<ParsingJob>();
  job->fileManager = &fm;
  job->architectures = opts.linkerOptions.architectures;
  job->platform = opts.frontendOptions.platform;
  job->osVersion = opts.frontendOptions.osVersion;
  job->language = opts.frontendOptions.language;
  job->language_std = opts.frontendOptions.language_std;
  job->isysroot = opts.frontendOptions.isysroot;
  job->macros = opts.frontendOptions.macros;
  job->frameworkPaths = opts.frontendOptions.frameworkPaths;
  job->includePaths = opts.frontendOptions.includePaths;
  job->clangExtraArgs = opts.frontendOptions.clangExtraArgs;
  job->scanPublicHeaders = true;
  job->scanPrivateHeaders = false;
  job->clangResourcePath = opts.frontendOptions.clangResourcePath;

  // Create a sorted list of framework headers.
  std::vector<const FileEntry *> publicHeaderFiles;
  for (const auto &header : framework._headerFiles) {
    if (header.second == HeaderType::Public) {
      auto *file = fm.getFile(header.first);
      if (!file) {
        diag.report(diag::err_no_such_public_header_file) << header.first;
        return false;
      }
      publicHeaderFiles.emplace_back(file);
    }
  }

  sort(publicHeaderFiles, [](const FileEntry *lhs, const FileEntry *rhs) {
    return StringRef(lhs->getName()) < StringRef(rhs->getName());
  });

  // Check if the framework has an umbrella header and move that to the
  // beginning.
  auto matchFirstAndMoveToTop = [](std::vector<const FileEntry *> &array,
                                   Regex &regex) -> bool {
    auto it = find_if(array, [&regex](const FileEntry *file) {
      return regex.match(file->getName());
    });

    if (it == array.end())
      return false;

    std::rotate(array.begin(), it, std::next(it));
    return true;
  };
  auto frameworkName = sys::path::stem(framework.getName());
  auto umbrellaName = Regex::escape(frameworkName) + "\\.h";
  Regex umbrellaRegex(umbrellaName);
  matchFirstAndMoveToTop(publicHeaderFiles, umbrellaRegex);

  job->publicHeaderFiles = std::move(publicHeaderFiles);

  // Add the current framework directory as a system framework directory. This
  // will prevent it from being droped from the top of the list if there is
  // a matching system framework include path.
  job->frameworkPaths.insert(job->frameworkPaths.begin(),
                             sys::path::parent_path(framework.getPath()));

  framework._headerSymbols = Scanner::run(std::move(job));

  return true;
}

/// \brief Scan the directory for public headers and generate API tests.
bool Driver::GenerateAPITests::run(DiagnosticsEngine &diag, Options &opts) {
  diag.setErrorLimit(opts.diagnosticsOptions.errorLimit);

  // Handle architectures.
  if (opts.linkerOptions.architectures.empty()) {
    diag.report(diag::err_no_architecture);
    return false;
  }

  // Handle platform.
  if (opts.frontendOptions.platform == Platform::Unknown) {
    diag.report(diag::err_no_deployment_target);
    return false;
  }

  if (opts.driverOptions.inputs.empty()) {
    diag.report(clang::diag::err_drv_no_input_files);
    return false;
  }

  if (opts.driverOptions.inputs.size() != 1) {
    diag.report(diag::err_expected_one_input_file);
    return false;
  }

  if (!opts.tapiOptions.configurationFile.empty()) {
    const auto *file =
        opts.getFileManager().getFile(opts.tapiOptions.configurationFile);
    if (!file) {
      diag.report(clang::diag::err_drv_no_such_file)
          << opts.tapiOptions.configurationFile;
      return false;
    }

    auto bufferOrErr = opts.getFileManager().getBufferForFile(file);
    if (auto ec = bufferOrErr.getError()) {
      diag.report(diag::err_cannot_read_file)
          << file->getName() << ec.message();
      return false;
    }
    yaml::Input yin(bufferOrErr.get()->getBuffer());

    yin >> issues;

    if (auto ec = yin.error()) {
      diag.report(diag::err) << file->getName() << ec.message();
      return false;
    }
  }

  //
  // Scan through the directories and create a list of all found frameworks.
  //
  DirectoryScanner scanner(opts.getFileManager(), diag);
  std::vector<Framework> frameworks;
  auto &path = opts.driverOptions.inputs.back();
  if (!opts.getFileManager().isDirectory(path)) {
    diag.report(diag::err_no_directory) << path;
    return false;
  }

  if (!scanner.scanDirectory(path, frameworks))
    return false;

  if (frameworks.empty()) {
    diag.report(diag::err_no_framework);
    return false;
  }

  sort(frameworks, [](const Framework &lhs, const Framework &rhs) {
    return lhs.getName() < rhs.getName();
  });

  auto last = std::unique(frameworks.begin(), frameworks.end(),
                          [](const Framework &lhs, const Framework &rhs) {
                            return lhs._baseDirectory == rhs._baseDirectory;
                          });
  frameworks.erase(last, frameworks.end());

  for (auto &framework : frameworks)
    parseFramework(framework, opts, diag);

  for (auto arch : opts.linkerOptions.architectures)
    if (!writeAPITest(frameworks, arch))
      return false;

  return true;
}

TAPI_NAMESPACE_INTERNAL_END
