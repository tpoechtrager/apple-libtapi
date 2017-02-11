//===- opt.cpp - The LLVM Modular Optimizer -------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Optimizations may be specified an arbitrary number of times on the command
// line, They are run in the order specified.
//
//===----------------------------------------------------------------------===//

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/APIAnalysis.h"

using namespace llvm;

// Option category for the API Analyzer
static cl::OptionCategory APIAnalyzerCategory("api-analyzer options");

static cl::list<std::string> InputFilenames(cl::Positional,
                                            cl::desc("<input bitcode file>"),
                                            cl::value_desc("filename"),
                                            cl::cat(APIAnalyzerCategory));

cl::opt<bool> ScanSelFromString("scan-strings",
                                cl::desc("Scan NSSelectorFromString calls"),
                                cl::init(false), cl::cat(APIAnalyzerCategory));
cl::opt<bool> ScanDLSym("scan-dlsym", cl::desc("Scan dlsym calls"),
                        cl::init(false), cl::cat(APIAnalyzerCategory));
cl::opt<bool> Demangle("demangle", cl::desc("Demangle symbols"),
                       cl::init(false), cl::cat(APIAnalyzerCategory));

cl::opt<bool> BitcodeOnly("bitcode-only", cl::desc("Only analyze bitcode"),
                          cl::init(false), cl::cat(APIAnalyzerCategory));

//===----------------------------------------------------------------------===//
// main for api-analyzer
//
int main(int argc, char **argv) {
  sys::PrintStackTraceOnErrorSignal();
  llvm::PrettyStackTraceProgram X(argc, argv);

  // Enable debug stream buffering.
  EnableDebugBuffering = true;

  llvm_shutdown_obj Y; // Call llvm_shutdown() on exit.

  InitializeAnalyzer();

  cl::HideUnrelatedOptions(APIAnalyzerCategory);
  cl::ParseCommandLineOptions(argc, argv, "llvm API analysis tool\n");

  // Before executing passes, print the final values of the LLVM options.
  cl::PrintOptionValues();

  // Load the input module...
  APIAnalysisResult result;

  APIAnalysisOptions options = {ScanSelFromString, ScanDLSym, Demangle,
                                BitcodeOnly};
  int exit_code = 0;

  for (auto file : InputFilenames) {
    StringRef filename(file);
    exit_code += AnalyzeFile(filename, result, options);
  }

  outs() << "--------------------------------------------------\n"
         << "Objective-C Selectors:\n"
         << "--------------------------------------------------\n";
  for (auto &s : result.messageNames)
    outs() << s.first() << "\n";

  outs() << "\n--------------------------------------------------\n"
         << "Objective-C Class Names:\n"
         << "--------------------------------------------------\n";
  for (auto &s : result.internalClassNames)
    outs() << s.first() << "\n";

  outs() << "\n--------------------------------------------------\n"
         << "Objective-C External Class Names:\n"
         << "--------------------------------------------------\n";
  for (auto &s : result.externalClassNames)
    outs() << s.first() << "\n";

  outs() << "\n--------------------------------------------------\n"
         << "External Function entries:\n"
         << "--------------------------------------------------\n";
  for (auto &d : result.externalFunctions)
    outs() << d.first() << "\n";

  outs() << "\n--------------------------------------------------\n"
         << "Internal Function entries:\n"
         << "--------------------------------------------------\n";
  for (auto &d : result.internalFunctions)
    outs() << d.first() << "\n";

  outs() << "\n--------------------------------------------------\n"
         << "External Globals:\n"
         << "--------------------------------------------------\n";
  for (auto &d : result.externalGlobals)
    outs() << d.first() << "\n";

  outs() << "\n--------------------------------------------------\n"
         << "Internal Globals:\n"
         << "--------------------------------------------------\n";
  for (auto &d : result.internalGlobals)
    outs() << d.first() << "\n";

  outs() << "\n--------------------------------------------------\n"
         << "Asm Symbols:\n"
         << "--------------------------------------------------\n";
  for (auto &d : result.asmSymbols)
    outs() << d << "\n";

  outs() << "\n--------------------------------------------------\n"
         << "Linked libraries:\n"
         << "--------------------------------------------------\n";
  for (auto &d : result.linkedLibraries)
    outs() << d.first() << "\n";

  outs() << "\n--------------------------------------------------\n"
         << "Ordered libraries:\n"
         << "--------------------------------------------------\n";
  for (auto &d : result.orderedLibraries)
    outs() << d << "\n";

  outs() << "\n--------------------------------------------------\n"
         << "Obj-C Class Metadata:\n"
         << "--------------------------------------------------\n";
  for (auto &d : result.objCClasses) {
    outs() << "*** " << d.first() << " : " << d.second.parentClass << " ***\n";
    outs() << "Defined Class Methods:\n";
    for (auto &m : d.second.internalClassMethods)
      outs() << "\t" << m << "\n";
    outs() << "Declared Class Methods:\n";
    for (auto &m : d.second.externalClassMethods)
      outs() << "\t" << m << "\n";
    outs() << "Defined Instance Methods:\n";
    for (auto &m : d.second.internalInstanceMethods)
      outs() << "\t" << m << "\n";
    outs() << "Declared Instance Methods:\n";
    for (auto &m : d.second.externalInstanceMethods)
      outs() << "\t" << m << "\n";
  }

  outs() << "\n--------------------------------------------------\n"
         << "Obj-C Protocol Metadata:\n"
         << "--------------------------------------------------\n";
  for (auto &d : result.objCProtocols) {
    outs() << "*** " << d.first() << " ***\n";
    outs() << "Defined Class Methods:\n";
    for (auto &m : d.second.internalClassMethods)
      outs() << "\t" << m << "\n";
    outs() << "Declared Class Methods:\n";
    for (auto &m : d.second.externalClassMethods)
      outs() << "\t" << m << "\n";
    outs() << "Defined Instance Methods:\n";
    for (auto &m : d.second.internalInstanceMethods)
      outs() << "\t" << m << "\n";
    outs() << "Declared Instance Methods:\n";
    for (auto &m : d.second.externalInstanceMethods)
      outs() << "\t" << m << "\n";
  }

  outs() << "\n--------------------------------------------------\n"
         << "Obj-C Category Metadata:\n"
         << "--------------------------------------------------\n";
  for (auto &d : result.objCCategories) {
    outs() << "*** " << d.first.second << " (" << d.first.first << ") ***\n";
    outs() << "Defined Class Methods:\n";
    for (auto &m : d.second.internalClassMethods)
      outs() << "\t" << m << "\n";
    outs() << "Declared Class Methods:\n";
    for (auto &m : d.second.externalClassMethods)
      outs() << "\t" << m << "\n";
    outs() << "Defined Instance Methods:\n";
    for (auto &m : d.second.internalInstanceMethods)
      outs() << "\t" << m << "\n";
    outs() << "Declared Instance Methods:\n";
    for (auto &m : d.second.externalInstanceMethods)
      outs() << "\t" << m << "\n";
  }

  outs() << "\n--------------------------------------------------\n"
         << "Obj-C Potentially Defined Selectors:\n"
         << "--------------------------------------------------\n";
  for (auto &s : result.potentiallyDefinedSelectors)
    outs() << "\t" << s.first() << "\n";

  return exit_code;
}
