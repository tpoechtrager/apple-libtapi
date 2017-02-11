##===- lib/LTO/Makefile ------------------------------------*- Makefile -*-===##
#
#                     The LLVM Compiler Infrastructure
#
# This file is distributed under the University of Illinois Open Source
# License. See LICENSE.TXT for details.
#
##===----------------------------------------------------------------------===##

LEVEL = ../..
LIBRARYNAME = LLVMLTO
BUILD_ARCHIVE := 1

include $(LEVEL)/Makefile.common

ifdef LLVM_VERSION_INFO
CompileCommonOpts += -DLLVM_VERSION_INFO='"$(LLVM_VERSION_INFO)"'
endif
ifdef CLANG_VENDOR
CompileCommonOpts += -DCLANG_VENDOR='"$(CLANG_VENDOR)"'
endif
