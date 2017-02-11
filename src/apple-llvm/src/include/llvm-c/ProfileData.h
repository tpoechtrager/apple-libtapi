/*===-- llvm-c/ProfileData.h - ProfileData Library C Interface ----*- C -*-===*\
|*                                                                            *|
|*                     The LLVM Compiler Infrastructure                       *|
|*                                                                            *|
|* This file is distributed under the University of Illinois Open Source      *|
|* License. See LICENSE.TXT for details.                                      *|
|*                                                                            *|
|*===----------------------------------------------------------------------===*|
|*                                                                            *|
|* This header declares the C interface to libProfileData.a, which implements *|
|* the infrastructure for working with profile data.                          *|
|*                                                                            *|
\*===----------------------------------------------------------------------===*/

#ifndef LLVM_C_PROFILEDATA_H
#define LLVM_C_PROFILEDATA_H

#include "llvm-c/Support.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup LLVMC LLVM-C: C interface to LLVM
 *
 * This module exposes parts of the LLVM library as a C API.
 *
 * @{
 */

typedef struct LLVMOpaqueCoverageMapping *LLVMCoverageMappingRef;

typedef struct LLVMOpaqueCoverageFunctionRange *LLVMCoverageFunctionRangeRef;

typedef struct LLVMOpaqueCoverageFunction *LLVMCoverageFunctionRef;

typedef struct LLVMOpaqueCoverageData *LLVMCoverageDataRef;

typedef struct LLVMOpaqueCoverageSegment *LLVMCoverageSegmentRef;

/**
 * Create a new CoverageMapping.
 *
 * Every call to this function should be paired with a call to
 * LLVMDisposeCoverageMapping() or the CoverageMapping will leak memory.
 */
LLVMCoverageMappingRef LLVMCreateCoverageMapping(const char *ObjectFilename,
                                                 const char *ProfileFilename);

/**
 * Create a new CoverageMapping targetting a particular architecture.
 *
 * Use this when the object file is a fat binary.
 *
 * Every call to this function should be paired with a call to
 * LLVMDisposeCoverageMapping() or the CoverageMapping will leak memory.
 */
LLVMCoverageMappingRef
LLVMCreateCoverageMappingForArch(const char *ObjectFilename,
                                 const char *ProfileFilename,
                                 const char *Arch);

/**
 * Destroy a CoverageMapping instance.
 */
void LLVMDisposeCoverageMapping(LLVMCoverageMappingRef CMR);

/**
 * Create a range containing all of the covered functions in the mapping.
 *
 * Every call to this function should be paired with a call to
 * LLVMDisposeCoverageFunctionRange() or the CoverageFunctionRange will leak
 * memory.
 *
 * @see llvm::profile::CoverageData::getCoveredFunctions
 */
LLVMCoverageFunctionRangeRef
LLVMCreateCoverageFunctionRange(LLVMCoverageMappingRef CMR);

/**
 * Create a range of all of the covered functions in a given file.
 *
 * Every call to this function should be paired with a call to
 * LLVMDisposeCoverageFunctionRange() or the CoverageFunctionRange will leak
 * memory.
 *
 * @see llvm::profile::CoverageData::getCoveredFunctions
 */
LLVMCoverageFunctionRangeRef
LLVMCreateCoverageFunctionRangeForFile(LLVMCoverageMappingRef CMR,
                                       const char *Filename);

/**
 * Destroy a CoverageFunctionRange
 */
void LLVMDisposeCoverageFunctionRange(LLVMCoverageFunctionRangeRef CFIR);

/**
 * Get the next CoverageFunction from the CoverageFunctionRange
 *
 * Returns NULL if there are no more CoverageFunctions.
 */
LLVMCoverageFunctionRef
LLVMGetNextCoverageFunction(LLVMCoverageMappingRef CMR,
                            LLVMCoverageFunctionRangeRef CFRR);

/**
 * Get the name of the function.
 */
const char *LLVMGetCoverageFunctionName(LLVMCoverageFunctionRef CFR);

/**
 * Get the execution count of the function.
 */
uint64_t LLVMGetCoverageFunctionExecutionCount(LLVMCoverageFunctionRef CFR);

/**
 * Get the coverage for a particular file.
 *
 * Every call to this function should be paired with a call to
 * LLVMDisposeCoverageData() or the CoverageData will leak memory.
 *
 * @see llvm::profile::CoverageData::getCoverageForFile
 */
LLVMCoverageDataRef LLVMCreateCoverageDataForFile(LLVMCoverageMappingRef CMR,
                                                  const char *Filename);

/**
 * Get the coverage for a particular function.
 *
 * Every call to this function should be paired with a call to
 * LLVMDisposeCoverageData() or the CoverageData will leak memory.
 *
 * @see llvm::profile::CoverageData::getCoverageForFunction
 */
LLVMCoverageDataRef
LLVMCreateCoverageDataForFunction(LLVMCoverageMappingRef CMR,
                                  const LLVMCoverageFunctionRef Function);

/**
 * Destroy a CoverageData instance.
 */
void LLVMDisposeCoverageData(LLVMCoverageDataRef CDR);

/**
 * Obtain an iterator to the first CoverageSegment of a CoverageData.
 *
 * Returns NULL if there are no coverage segments.
 *
 * @see llvm::profile::CoverageData::begin()
 */
LLVMCoverageSegmentRef LLVMGetFirstCoverageSegment(LLVMCoverageDataRef CDR);

/**
 * Advance a CoverageSegment iterator to the next CoverageSegment.
 *
 * Returns NULL if the iterator was already at the end and there are no more
 * CoverageSegments.
 */
LLVMCoverageSegmentRef LLVMGetNextCoverageSegment(LLVMCoverageDataRef CDR,
                                                  LLVMCoverageSegmentRef CSR);

/**
 * Get the line number where a coverage segment begins.
 */
unsigned LLVMGetCoverageSegmentLine(LLVMCoverageSegmentRef CSR);

/**
 * Get the column where a coverage segment begins.
 */
unsigned LLVMGetCoverageSegmentColumn(LLVMCoverageSegmentRef CSR);

/**
 * Get the execution count of a segment, or zero if no count was recorded.
 */
uint64_t LLVMGetCoverageSegmentCount(LLVMCoverageSegmentRef CSR);

/**
 * Get whether or not an execution count was recorded for a coverage segment.
 */
LLVMBool LLVMGetCoverageSegmentHasCount(LLVMCoverageSegmentRef CSR);

/**
 * Get whether or not a coverage segments represents a new region.
 */
LLVMBool LLVMGetCoverageSegmentIsRegionEntry(LLVMCoverageSegmentRef CSR);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* LLVM_C_PROFILEDATA_H */
