//===- lib/Core/FileManager.cpp - File Manager ------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Implements the file manager.
///
//===----------------------------------------------------------------------===//

#include "tapi/Core/FileManager.h"
#include "tapi/Defines.h"
//#include "tapi/Driver/Snapshot.h"
//#include "tapi/Driver/SnapshotFileSystem.h"
#include "clang/Basic/FileSystemStatCache.h"
#include "clang/Basic/VirtualFileSystem.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/StringRef.h"

using namespace llvm;
using namespace clang;

namespace {

/// \brief A file system stat cache that records all successful stat requests in
///        the snapshot. The actual caching is deferred to the lower stat caches
///        (if they exists).
class StatRecorder final : public FileSystemStatCache {
public:
  StatRecorder() = default;

  LookupResult getStat(StringRef path, FileData &data, bool isFile,
                       std::unique_ptr<vfs::File> *file,
                       vfs::FileSystem &fs) override {
    abort();
#if 0
    auto result = statChained(path, data, isFile, file, fs);

    // Don't record non existing files and directories.
    if (result == CacheMissing)
      return result;

    if (data.IsDirectory)
      TAPI_INTERNAL::globalSnapshot->recordDirectory(path);
    else
      TAPI_INTERNAL::globalSnapshot->recordFile(path);

    return result;
#endif
  }
};

} // end anonymous namespace.

TAPI_NAMESPACE_INTERNAL_BEGIN

FileManager::FileManager(const FileSystemOptions &fileSystemOpts,
                         IntrusiveRefCntPtr<vfs::FileSystem> fs)
    : clang::FileManager(fileSystemOpts, fs) {
  // Record if initialized with VFS.
  if (fs)
    initWithVFS = true;
  // Inject our recording stat cache.
  installStatRecorder();
}

bool FileManager::exists(StringRef path) {
  clang::vfs::Status result;
  if (getNoncachedStatValue(path, result))
    return false;
  return result.exists();
}

bool FileManager::isSymlink(StringRef path) {
  if (initWithVFS)
    return false;
  return sys::fs::is_symlink_file(path);
}

void FileManager::installStatRecorder() {
  clearStatCaches();
  addStatCache(make_unique<StatRecorder>());
}

TAPI_NAMESPACE_INTERNAL_END
