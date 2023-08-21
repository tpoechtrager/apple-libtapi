//===- tapi/Driver/StatRecorder.h - Stat Recorder ---------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief Defines a stat recorder.
///
//===----------------------------------------------------------------------===//
#ifndef TAPI_DRIVER_STAT_RECORDER_H
#define TAPI_DRIVER_STAT_RECORDER_H

#include "tapi/Defines.h"
#include "tapi/Driver/Snapshot.h"
#include "tapi/Driver/SnapshotFileSystem.h"
#include "clang/Basic/FileSystemStatCache.h"

TAPI_NAMESPACE_INTERNAL_BEGIN

/// \brief A file system stat cache that records all successful stat requests in
///        the snapshot. The actual caching is deferred to the lower stat caches
///        (if they exists).
class StatRecorder final : public clang::FileSystemStatCache {
public:
  StatRecorder() = default;

  std::error_code getStat(StringRef path, llvm::vfs::Status &status,
                          bool isFile, std::unique_ptr<llvm::vfs::File> *file,
                          llvm::vfs::FileSystem &fs) override {
    auto err = get(path, status, isFile, file, nullptr, fs);
    if (err)
      return err;

    if (status.isDirectory())
      TAPI_INTERNAL::globalSnapshot->recordDirectory(path);
    else
      TAPI_INTERNAL::globalSnapshot->recordFile(path);

    return std::error_code();
  }
};

TAPI_NAMESPACE_INTERNAL_END

#endif // TAPI_DRIVER_STAT_RECORDER_H
