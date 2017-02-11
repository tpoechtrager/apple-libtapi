#include "XarAnalyzer.h"

extern "C" {
#include <xar/xar.h>
}

#include <unistd.h>

#include "llvm/APIAnalysis.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/Mutex.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/FileSystem.h"

using namespace llvm;
using namespace object;

int AnalyzeXar(StringRef filePath, APIAnalysisIntermediateResult &result,
               const APIAnalysisOptions &options) {
  xar_t x;

  SmallVector<std::pair<char*, size_t>, 8> fileBuffers;
  
  static ManagedStatic<sys::Mutex> XarLock;
  XarLock->lock();

  bool isDylib = false;
  bool installNameNext = false;
  bool mainFuncNext = false;
  std::string installName;
  std::string mainFunction;

  x = xar_open(filePath.data(), READ);
  if (x == NULL) {
    errs() << "Error opening xarchive: " << filePath << "\n";
    return 1;
  }

  // Analyze all the bitcode/xar/object files.
  // Process the XAR header.
  xar_iter_t it = xar_iter_new();
  if (!it) {
    errs() << "Could not construct xar iter\n";
    xar_close(x);
    return 1;
  }
  for (xar_file_t f = xar_file_first(x, it); f; f = xar_file_next(it)) {
    const char* filetype = nullptr;
    if (xar_prop_get(f, "file-type", &filetype) != 0) {
      errs() << "Could not decode xar file type\n";
      xar_close(x);
      return 1;
    }
    if (StringRef(filetype) != "Bitcode" &&
        StringRef(filetype) != "Object" &&
        StringRef(filetype) != "LTO" &&
        StringRef(filetype) != "Bundle")
      continue;
    // extract and analyze file.
    char* buffer = nullptr;
    size_t size = 0;
    if (xar_extract_tobuffersz(x, f, &buffer, &size) != 0) {
      errs() << "Could not extract file\n";
      xar_close(x);
      return 1;
    }
    fileBuffers.push_back({buffer, size});
  }
  xar_iter_free(it);
  
  // process the library list.
  // because all the library must be in order, iterating and deleting to
  // read toc entries.
  for (xar_subdoc_t sub = xar_subdoc_first(x); sub; sub = xar_subdoc_next(sub)) {
    const char* docName = xar_subdoc_name(sub);
    if (StringRef(docName) != "Ld")
      continue;

    while(1) {
      xar_iter_t p = xar_iter_new();
      const char* key = xar_prop_first((xar_file_t)sub, p);
      while (key && StringRef(key) != "dylibs/lib" &&
             StringRef(key) != "dylibs/weak" &&
             StringRef(key) != "link-options/option") {
        key = xar_prop_next(p);
      }
      if (!key) {
        xar_iter_free(p);
        break;
      }
      const char* value = nullptr;
      if (xar_prop_get((xar_file_t)sub, key, &value) != 0) {
        errs() << "Could not read subdoc\n";
        xar_close(x);
        return 1;
      }
      if (StringRef(key) == "link-options/option") {
        if (StringRef(value) == "-dylib")
          isDylib = true;
        else if (StringRef(value) == "-install_name")
          installNameNext = true;
        else if (StringRef(value) == "-e")
          mainFuncNext = true;
        else if (installNameNext) {
          installName = value;
          installNameNext = false;
        } else if (mainFuncNext) {
          mainFunction = value;
          mainFuncNext = false;
        }
      } else if (result.linkedLibraries.find(value) ==
                 result.linkedLibraries.end()) {
        result.orderedLibraries.push_back(value);
        result.linkedLibraries.insert(value);
      }
      xar_prop_unset((xar_file_t)sub, key);
      xar_iter_free(p);
    }
  }

  xar_close(x);
  XarLock->unlock();

  // dylib has its install name as the first entry in the library list
  if (isDylib && !installName.empty() &&
      result.linkedLibraries.find(installName) ==
          result.linkedLibraries.end()) {
    result.orderedLibraries.insert(result.orderedLibraries.begin(),
                                   installName);
    result.linkedLibraries.insert(installName);
    result.installName = installName;
  }

  // main function is an undefined variable
  if (!mainFunction.empty() &&
      result.functionNames.find(mainFunction) == result.functionNames.end())
    result.functionNames.insert({mainFunction, false});

  // Analyze file content.
  int success = 0;
  for (auto &buf : fileBuffers) {
    StringRef fileContent(buf.first, buf.second);
    MemoryBufferRef memoryBuf(fileContent, "xar file");
    success += AnalyzeFileImpl(memoryBuf, result, options);
    free(buf.first);
  }

  return success;
}

int AnalyzeXar(llvm::MemoryBufferRef fileData,
               APIAnalysisIntermediateResult &result,
               const APIAnalysisOptions &options) {
  int TmpArchiveFD = -1;
  SmallString<128> TmpArchive;

  std::error_code EC = llvm::sys::fs::createTemporaryFile(
      "bitcode.tmp", "xar", TmpArchiveFD, TmpArchive);
  if (EC) {
    errs() << "unable to create temporary file: '" << EC.message() << "'";
    return 1;
  }

  {
    raw_fd_ostream Out(TmpArchiveFD, /*shouldClose=*/true);
    Out << fileData.getBuffer();
  }

  StringRef xarPath = TmpArchive.str();
  auto success = AnalyzeXar(xarPath, result, options);
  sys::fs::remove(xarPath);
  return success;
}
