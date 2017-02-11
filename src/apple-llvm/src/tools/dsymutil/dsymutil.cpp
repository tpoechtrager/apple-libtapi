//===-- dsymutil.cpp - Debug info dumping utility for llvm ----------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This program is a utility that aims to be a dropin replacement for
// Darwin's dsymutil.
//
//===----------------------------------------------------------------------===//

#include "CFBundle.h"
#include "DebugMap.h"
#include "MachOUtils.h"
#include "dsymutil.h"
#include "llvm/Object/MachO.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/FileUtilities.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/Options.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetSelect.h"
#include <cstdint>
#include <string>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <uuid/uuid.h>
#endif

using namespace llvm::dsymutil;

namespace {
using namespace llvm::cl;

OptionCategory DsymCategory("Specific Options");
static opt<bool> Help("h", desc("Alias for -help"), Hidden);
static opt<bool> Version("v", desc("Alias for -version"), Hidden);

static list<std::string> InputFiles(Positional, OneOrMore,
                                    desc("<input files>"), cat(DsymCategory));

static opt<std::string>
    OutputFileOpt("o",
                  desc("Specify the output file. default: <input file>.dwarf"),
                  value_desc("filename"), cat(DsymCategory));
static alias OutputFileOptA("out", desc("Alias for -o"),
                            aliasopt(OutputFileOpt));

static opt<std::string> OsoPrependPath(
    "oso-prepend-path",
    desc("Specify a directory to prepend to the paths of object files."),
    value_desc("path"), cat(DsymCategory));

static opt<bool> DumpStab(
    "symtab",
    desc("Dumps the symbol table found in executable or object file(s) and\n"
         "exits."),
    init(false), cat(DsymCategory));
static alias DumpStabA("s", desc("Alias for --symtab"), aliasopt(DumpStab));

static opt<bool> FlatOut("flat",
                         desc("Produce a flat dSYM file (not a bundle)."),
                         init(false), cat(DsymCategory));
static alias FlatOutA("f", desc("Alias for --flat"), aliasopt(FlatOut));

static opt<bool> Minimize(
    "minimize",
    desc("When used when creating a dSYM file, this option will suppress\n"
         "the emission of the .debug_inlines, .debug_pubnames, and\n"
         ".debug_pubtypes sections since dsymutil currently has better\n"
         "equivalents: .apple_names and .apple_types. When used in\n"
         "conjunction with --update option, this option will cause redundant\n"
         "accelerator tables to be removed."),
    init(false), cat(DsymCategory));
static alias MinimizeA("z", desc("Alias for --minimize"), aliasopt(Minimize));

static opt<bool> Update(
    "update",
    desc("Updates existing dSYM files to contain the latest accelerator\n"
         "tables and other DWARF optimizations. This option will currently\n"
         "add the new .apple_names and .apple_types hashed accelerator\n"
         "tables."),
    init(false), cat(DsymCategory));
static alias UpdateA("u", desc("Alias for --update"), aliasopt(Update));

static opt<std::string> SymbolMap(
    "symbol-map",
    desc("Updates the existing dsyms inplace using symbol map specified."),
    value_desc("bcsymbolmap"), cat(DsymCategory));

static opt<unsigned> Threads(
    "threads",
    desc("Specifies the maximum number (n) of simultaneous threads to use\n"
         "when linking multiple architectures."),
    init(0), cat(DsymCategory));
static alias ThreadsA("t", desc("Alias for --threads"), aliasopt(Threads));

static opt<bool> Verbose("verbose", desc("Verbosity level"), init(false),
                         cat(DsymCategory));

static opt<bool>
    NoOutput("no-output",
             desc("Do the link in memory, but do not emit the result file."),
             init(false), cat(DsymCategory));

static list<std::string> ArchFlags(
    "arch",
    desc("Link DWARF debug information only for specified CPU architecture\n"
         "types. This option can be specified multiple times, once for each\n"
         "desired architecture.  All cpu architectures will be linked by\n"
         "default."),
    ZeroOrMore, cat(DsymCategory));

static opt<bool>
    NoODR("no-odr",
          desc("Do not use ODR (One Definition Rule) for type uniquing."),
          init(false), cat(DsymCategory));

static opt<bool> DumpDebugMap(
    "dump-debug-map",
    desc("Parse and dump the debug map to standard output. Not DWARF link "
         "will take place."),
    init(false), cat(DsymCategory));

static opt<bool> InputIsYAMLDebugMap(
    "y", desc("Treat the input file is a YAML debug map rather than a binary."),
    init(false), cat(DsymCategory));
}

static bool createPlistFile(llvm::StringRef Bin, llvm::StringRef BundleRoot) {
  if (NoOutput)
    return true;

  // Create plist file to write to.
  llvm::SmallString<128> InfoPlist(BundleRoot);
  llvm::sys::path::append(InfoPlist, "Contents/Info.plist");
  std::error_code EC;
  llvm::raw_fd_ostream PL(InfoPlist, EC, llvm::sys::fs::F_Text);
  if (EC) {
    llvm::errs() << "error: cannot create plist file " << InfoPlist << ": "
                 << EC.message() << '\n';
    return false;
  }

  std::string bundleVersionStr, bundleShortVersionStr, bundleIDStr;
  bool OmitShortVersion = false;
  getBundleInfo(Bin, bundleVersionStr, bundleShortVersionStr, bundleIDStr,
                OmitShortVersion);

  if (bundleIDStr.empty()) {
    llvm::StringRef BundleID = *llvm::sys::path::rbegin(BundleRoot);
    if (llvm::sys::path::extension(BundleRoot) == ".dSYM")
      bundleIDStr = llvm::sys::path::stem(BundleID);
    else
      bundleIDStr = BundleID;
  }

  // Print out information to the plist file.
  PL << "<?xml version=\"1.0\" encoding=\"UTF-8\"\?>\n"
     << "<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" "
     << "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
     << "<plist version=\"1.0\">\n"
     << "\t<dict>\n"
     << "\t\t<key>CFBundleDevelopmentRegion</key>\n"
     << "\t\t<string>English</string>\n"
     << "\t\t<key>CFBundleIdentifier</key>\n"
     << "\t\t<string>com.apple.xcode.dsym." << bundleIDStr << "</string>\n"
     << "\t\t<key>CFBundleInfoDictionaryVersion</key>\n"
     << "\t\t<string>6.0</string>\n"
     << "\t\t<key>CFBundlePackageType</key>\n"
     << "\t\t<string>dSYM</string>\n"
     << "\t\t<key>CFBundleSignature</key>\n"
     << "\t\t<string>\?\?\?\?</string>\n";

  if (!OmitShortVersion)
    PL << "\t\t<key>CFBundleShortVersionString</key>\n"
       << "\t\t<string>" << bundleShortVersionStr << "</string>\n";

  PL << "\t\t<key>CFBundleVersion</key>\n"
     << "\t\t<string>" << bundleVersionStr << "</string>\n"
     << "\t</dict>\n"
     << "</plist>\n";

  PL.close();
  return true;
}

static bool createBundleDir(llvm::StringRef BundleBase) {
  if (NoOutput)
    return true;

  llvm::SmallString<128> Bundle(BundleBase);
  llvm::sys::path::append(Bundle, "Contents", "Resources", "DWARF");
  if (std::error_code EC = create_directories(Bundle.str(), true,
                                              llvm::sys::fs::perms::all_all)) {
    llvm::errs() << "error: cannot create directory " << Bundle << ": "
                 << EC.message() << "\n";
    return false;
  }
  return true;
}

static std::error_code getUniqueFile(const llvm::Twine &Model, int &ResultFD,
                                     llvm::SmallVectorImpl<char> &ResultPath) {
  // If in NoOutput mode, use the createUniqueFile variant that
  // doesn't open the file but still generates a somewhat unique
  // name. In the real usage scenario, we'll want to ensure that the
  // file is trully unique, and creating it is the only way to achieve
  // that.
  if (NoOutput)
    return llvm::sys::fs::createUniqueFile(Model, ResultPath);
  return llvm::sys::fs::createUniqueFile(Model, ResultFD, ResultPath);
}

static std::string getOutputFileName(llvm::StringRef InputFile,
                                     bool TempFile = false) {
  if (OutputFileOpt == "-")
    return OutputFileOpt;

  if (TempFile) {
    llvm::SmallString<128> TmpFile;
    llvm::sys::path::system_temp_directory(true, TmpFile);
    llvm::StringRef Basename =
        OutputFileOpt.empty() ? InputFile : llvm::StringRef(OutputFileOpt);
    llvm::sys::path::append(TmpFile, llvm::sys::path::filename(Basename));

    int FD;
    llvm::SmallString<128> UniqueFile;
    if (auto EC = getUniqueFile(TmpFile + ".tmp%%%%%.dwarf", FD, UniqueFile)) {
      llvm::errs() << "error: failed to create temporary outfile '"
                   << TmpFile << "': " << EC.message() << '\n';
      return "";
    }
    llvm::sys::RemoveFileOnSignal(UniqueFile);
    if (!NoOutput) {
      // Close the file immediately. We know it is unique. It will be
      // reopened and written to later.
      llvm::raw_fd_ostream CloseImmediately(FD, true /* shouldClose */, true);
    }
    return UniqueFile.str();
  }

  // When updating, do in place replacement.
  if (OutputFileOpt.empty() && (Update || !SymbolMap.empty()))
    return InputFile;

  if (FlatOut) {
    // If a flat dSYM has been requested, things are pretty simple.
    if (OutputFileOpt.empty()) {
      if (InputFile == "-")
        return "a.out.dwarf";
      return (InputFile + ".dwarf").str();
    }

    return OutputFileOpt;
  }

  // We need to create/update a dSYM bundle.
  // A bundle hierarchy looks like this:
  //   <bundle name>.dSYM/
  //       Contents/
  //          Info.plist
  //          Resources/
  //             DWARF/
  //                <DWARF file(s)>
  std::string DwarfFile =
      InputFile == "-" ? llvm::StringRef("a.out") : InputFile;
  llvm::SmallString<128> BundleDir(OutputFileOpt);
  if (BundleDir.empty())
    BundleDir = DwarfFile + ".dSYM";
  if (!createBundleDir(BundleDir) || !createPlistFile(DwarfFile, BundleDir))
    return "";

  llvm::sys::path::append(BundleDir, "Contents", "Resources", "DWARF",
                          llvm::sys::path::filename(DwarfFile));
  return BundleDir.str();
}

static void loadSymbolMap(std::string SymbolMapFilename, LinkOptions &Options,
                          llvm::StringRef InputFile, const DebugMap &Map) {
  static std::vector<std::string> UnobfuscatedStrings;
  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> ErrOrMemBuffer(nullptr);
#if __APPLE__
  // Look through the UUID Map
  if (llvm::sys::fs::is_directory(SymbolMapFilename) &&
      !Map.getUUID().empty()) {
    uuid_string_t UUIDString;
    uuid_unparse_upper((uint8_t *)Map.getUUID().data(), UUIDString);
    std::string plistFilename = (llvm::sys::path::parent_path(InputFile) +
                                 llvm::Twine("/../") + UUIDString + ".plist")
                                    .str();

    CFStringRef plistFile = CFStringCreateWithCString(
        kCFAllocatorDefault, plistFilename.c_str(), kCFStringEncodingUTF8);
    CFURLRef fileURL = CFURLCreateWithFileSystemPath(
        kCFAllocatorDefault, plistFile, kCFURLPOSIXPathStyle, false);
    CFReadStreamRef resourceData =
        CFReadStreamCreateWithFile(kCFAllocatorDefault, fileURL);
    if (resourceData) {
      CFReadStreamOpen(resourceData);
      CFDictionaryRef plist = (CFDictionaryRef)CFPropertyListCreateWithStream(
          kCFAllocatorDefault, resourceData, 0, kCFPropertyListImmutable, NULL,
          NULL);

      if (plist) {
        if (CFDictionaryContainsKey(plist, CFSTR("DBGOriginalUUID"))) {
          CFStringRef OldUUID = (CFStringRef)CFDictionaryGetValue(
              plist, CFSTR("DBGOriginalUUID"));
          SymbolMapFilename +=
              (llvm::Twine("/") + llvm::StringRef(CFStringGetCStringPtr(
                                      OldUUID, kCFStringEncodingUTF8)) +
               ".bcsymbolmap")
                  .str();
        }
        CFRelease(plist);
      }
      CFReadStreamClose(resourceData);
      CFRelease(resourceData);
    }
    CFRelease(fileURL);
    CFRelease(plistFile);
  }
#endif

  // If the SymbolMap is not a directory, use the file as symbolmap.
  if (!llvm::sys::fs::is_directory(SymbolMapFilename)) {
    ErrOrMemBuffer = llvm::MemoryBuffer::getFile(SymbolMapFilename);
  } else {
    SymbolMapFilename +=
        (llvm::Twine("/") + llvm::sys::path::filename(InputFile) + "-" +
         MachOUtils::getArchName(Map.getTriple().getArchName()) +
         ".bcsymbolmap")
            .str();
    ErrOrMemBuffer = llvm::MemoryBuffer::getFile(SymbolMapFilename);
  }

  UnobfuscatedStrings.clear();
  if (auto EC = ErrOrMemBuffer.getError()) {
    llvm::errs() << "warning: " << SymbolMapFilename << ": " << EC.message()
                 << ". Not unobfuscating.\n";
    return;
  }

  auto &MemBuf = **ErrOrMemBuffer;
  llvm::StringRef Data(MemBuf.getBufferStart(),
                       MemBuf.getBufferEnd() - MemBuf.getBufferStart());
  llvm::StringRef LHS;

  // Check version string first.
  std::tie(LHS, Data) = Data.split('\n');
  bool MangleNames = false;
  if (!LHS.startswith("BCSymbolMap Version:")) {
    // Version string not present, warns but try to parse it.
    llvm::errs() << "warning: " << SymbolMapFilename
                 << " is missing version string. Assuming 1.0.\n";
    UnobfuscatedStrings.emplace_back(LHS);
  } else if (LHS.equals("BCSymbolMap Version: 1.0")) {
    MangleNames = true;
  } else if (LHS.equals("BCSymbolMap Version: 2.0")) {
    MangleNames = false;
  } else {
    llvm::StringRef VersionNum;
    std::tie(LHS, VersionNum) = LHS.split(':');
    llvm::errs() << "warning: " << SymbolMapFilename
                 << " has unsupported symbol map version" << VersionNum
                 << ". Not unobfuscating.\n";
    return;
  }
  while (!Data.empty()) {
    std::tie(LHS, Data) = Data.split('\n');
    UnobfuscatedStrings.emplace_back(LHS);
  }

  if (Options.Translator)
    return;

  Options.Translator = [MangleNames](llvm::StringRef Input) -> llvm::StringRef {
    if (!Input.startswith("__hidden#") && !Input.startswith("___hidden#"))
      return Input;
    unsigned LineNumber = UINT_MAX;
    bool MightNeedUnderscore = false;
    llvm::StringRef Line = Input.drop_front(sizeof("__hidden#") - 1);
    if (Line[0] == '#') {
      Line = Line.drop_front();
      MightNeedUnderscore = true;
    }
    Line.split('_').first.getAsInteger(10, LineNumber);
    if (LineNumber >= UnobfuscatedStrings.size()) {
      llvm::errs() << "warning: reference to a unexisting unobfuscated string "
                   << Input.str() << ". "
                   << "Symbol map mismatch?\n" << Line << '\n';

      return Input;
    }

    const std::string &Translation = UnobfuscatedStrings[LineNumber];
    if (!MightNeedUnderscore || !MangleNames)
      return Translation;
    // Objective C symbols for the Macho symbol table start with that
    // weird \1 character. Do not preprend an underscore to these and
    // drop that initial \1.
    if (Translation[0] == 1)
      return llvm::StringRef(Translation).drop_front();

    // We need permanent storage for the string we are about to
    // create. Just append it to the vector containing
    // translations. This should only happen during MachO symbol table
    // translation, thus there should be no risk on exponential growth.
    UnobfuscatedStrings.emplace_back("_" + Translation);
    return UnobfuscatedStrings.back();
  };
}

void llvm::dsymutil::exitDsymutil(int ExitStatus) {
  // Cleanup temporary files.
  llvm::sys::RunInterruptHandlers();
  exit(ExitStatus);
}

int main(int argc, char **argv) {
  llvm::sys::PrintStackTraceOnErrorSignal();
  llvm::PrettyStackTraceProgram StackPrinter(argc, argv);
  llvm::llvm_shutdown_obj Shutdown;
  LinkOptions Options;
  void *MainAddr = (void *)(intptr_t)&exitDsymutil;
  std::string SDKPath = llvm::sys::fs::getMainExecutable(argv[0], MainAddr);
  SDKPath = llvm::sys::path::parent_path(SDKPath);

  HideUnrelatedOptions(DsymCategory);
  llvm::cl::ParseCommandLineOptions(
      argc, argv,
      "manipulate archived DWARF debug symbol files.\n\n"
      "dsymutil links the DWARF debug information found in the object files\n"
      "for the executable <input file> by using debug symbols information\n"
      "contained in its symbol table.\n");

  if (Help)
    PrintHelpMessage();

  if (Version) {
    llvm::cl::PrintVersionMessage();
    return 0;
  }

  Options.Verbose = Verbose;
  Options.NoOutput = NoOutput;
  Options.NoODR = NoODR;
  Options.Minimize = Minimize;
  Options.Update = Update;
  Options.PrependPath = OsoPrependPath;

  if (!SymbolMap.empty())
    Options.Update = true;

  if (Options.Update &&
      std::find(InputFiles.begin(), InputFiles.end(), "-") !=
          InputFiles.end()) {
    // FIXME: We cannot use stdin for an update because stdin will be
    // consumed by the BinaryHolder during the debugmap parsing, and
    // then we will want to consume it again in DwarfLinker. If we
    // used a unique BinaryHolder object that could cache multiple
    // binaries this restriction would go away.
    llvm::errs() << "error: standard input cannot be used as input for a "
                 << "dSYM update.\n";
    exitDsymutil(1);
  }

  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllTargets();
  llvm::InitializeAllAsmPrinters();

  std::vector<std::string> Inputs;

  if (Options.Update) {
    // If we are updating, we might get dSYM bundles as input.
    for (const auto &Input : InputFiles) {
      if (!llvm::sys::fs::is_directory(Input)) {
        Inputs.push_back(Input);
        continue;
      }
      std::string dSYMDir = Input + "/Contents/Resources/DWARF";
      if (!llvm::sys::fs::is_directory(dSYMDir)) {
        llvm::errs() << "error: " << Input << " is a directory, but doesn't "
                     << "look like a dSYM bundle.\n";
        exitDsymutil(1);
      }
      std::error_code EC;
      llvm::sys::fs::directory_iterator DirIt(dSYMDir, EC);
      llvm::sys::fs::directory_iterator DirEnd;
      if (EC) {
        llvm::errs() << "error: " << dSYMDir << ": " << EC.message() << '\n';
        exitDsymutil(1);
      }

      while (DirIt != DirEnd) {
        Inputs.push_back(DirIt->path());
        DirIt.increment(EC);
        if (EC) {
          llvm::errs() << "error: " << dSYMDir << ": " << EC.message() << '\n';
          exitDsymutil(1);
        }
      }
    }
  } else {
    Inputs = InputFiles;
  }

  if (!FlatOut && OutputFileOpt == "-") {
    llvm::errs() << "error: cannot emit to standard output without --flat\n";
    return 1;
  }

  if (InputFiles.size() > 1 && !SymbolMap.empty() &&
      !llvm::sys::fs::is_directory(SymbolMap)) {
    llvm::errs() << "error: when unobfuscating multiple files, --symbol-map "
                 << "needs to point to a directory.\n";
    exitDsymutil(1);
  }

  if (Inputs.size() > 1 && FlatOut && !OutputFileOpt.empty()) {
    llvm::errs() << "error: cannot use -o with multiple inputs in flat mode\n";
    return 1;
  }

  for (const auto &Arch : ArchFlags)
    if (Arch != "*" && Arch != "all" &&
        !llvm::object::MachOObjectFile::isValidArch(Arch)) {
      llvm::errs() << "error: Unsupported cpu architecture: '" << Arch << "'\n";
      exitDsymutil(1);
    }

  for (auto &InputFile : Inputs) {
    // Dump the symbol table for each input file and requested arch
    if (DumpStab) {
      if (!dumpStab(InputFile, ArchFlags, OsoPrependPath))
        exitDsymutil(1);
      continue;
    }

    auto DebugMapPtrsOrErr = parseDebugMap(InputFile, ArchFlags, OsoPrependPath,
                                           Verbose, InputIsYAMLDebugMap);

    if (auto EC = DebugMapPtrsOrErr.getError()) {
      llvm::errs() << "error: cannot parse the debug map for \"" << InputFile
                   << "\": " << EC.message() << '\n';
      exitDsymutil(1);
    }

    if (Options.Update) {
      // The debug map should be empty. Add one object file
      // corresponding to the input file.
      for (auto &Map : *DebugMapPtrsOrErr)
        Map->addDebugMapObject(InputFile,
                               llvm::sys::TimeValue::PosixZeroTime());
      if (InputFiles.size() > 1 && !SymbolMap.empty() &&
          !llvm::sys::fs::is_directory(SymbolMap)) {
        llvm::errs() << "error: when unobfuscating fat binaries, --symbol-map "
                     << "needs to point to a directory.\n";
        exitDsymutil(1);
      }
    }

    if (DebugMapPtrsOrErr->empty()) {
      llvm::errs() << "error: no architecture to link\n";
      exitDsymutil(1);
    }

    // If there is more than one link to execute, we need to generate
    // temporary files.
    bool NeedsTempFiles =
        !DumpDebugMap && ((*DebugMapPtrsOrErr).size() != 1 || Options.Update);
    llvm::SmallVector<MachOUtils::ArchAndFilename, 4> TempFiles;
    for (auto &Map : *DebugMapPtrsOrErr) {
      if (Verbose || DumpDebugMap)
        Map->print(llvm::outs());

      if (DumpDebugMap)
        continue;

      if (!SymbolMap.empty())
        loadSymbolMap(SymbolMap, Options, InputFile, *Map);

      if (Map->begin() == Map->end())
        llvm::errs() << "warning: no debug symbols in executable (-arch "
                     << MachOUtils::getArchName(Map->getTriple().getArchName())
                     << ")\n";

      std::string OutputFile = getOutputFileName(InputFile, NeedsTempFiles);
      if (OutputFile.empty() || !linkDwarf(OutputFile, *Map, Options))
        exitDsymutil(1);

      if (NeedsTempFiles)
        TempFiles.emplace_back(Map->getTriple().getArchName().str(),
                               OutputFile);
    }

    if (NeedsTempFiles &&
        !MachOUtils::generateUniversalBinary(
            TempFiles, getOutputFileName(InputFile), Options, SDKPath))
      exitDsymutil(1);
  }

  exitDsymutil(0);
}
