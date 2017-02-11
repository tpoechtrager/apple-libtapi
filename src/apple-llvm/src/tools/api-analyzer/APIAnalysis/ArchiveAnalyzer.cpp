
#include "BinaryAnalyzer.h"

#include "llvm/Object/Archive.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Errc.h"
#include "llvm/Support/MemoryBuffer.h"

using namespace llvm;
using namespace object;

int AnalyzeArchive(StringRef filePath, APIAnalysisIntermediateResult &result,
                   const APIAnalysisOptions &options) {
  // Create or open the archive object.
  ErrorOr<std::unique_ptr<MemoryBuffer>> Buf =
      MemoryBuffer::getFile(filePath, -1, false);
  std::error_code EC = Buf.getError();
  if (EC && EC != errc::no_such_file_or_directory) {
    errs() << "error opening '" << filePath << "': " << EC.message() << "!\n";
    return 1;
  }

  if (!EC) {
    Archive Ar(Buf.get()->getMemBufferRef(), EC);

    if (EC) {
      errs() << "error loading '" << filePath << "': " << EC.message() << "!\n";
      return 1;
    }
    for (auto &c : Ar.children()) {
      ErrorOr<MemoryBufferRef> MBOrErr = c->getMemoryBufferRef();
      if (std::error_code EC = MBOrErr.getError()) {
        errs() << "unable to read input: '" << EC.message() << "'";
        return 1;
      }
      MemoryBufferRef MB = MBOrErr.get();
      ErrorOr<std::unique_ptr<Binary>> BinaryOrErr = createBinary(MB);
      if (std::error_code EC = BinaryOrErr.getError()) {
        errs() << "unable to read input: '" << EC.message() << "'";
        return 1;
      }
      Binary &binary = *BinaryOrErr.get();
      return AnalyzeBinaryImpl(binary, result, options);
    }
    return 0;
  }
  return 0;
}
