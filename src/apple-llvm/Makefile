##
# Top-level Clang B&I Makefile
##

# LLVM/Clang default configuration.

# TODO: In CMake we should move these out of the makefile into the CMake cache
# Select default assertion mode, valid values are '0' and '1'.
Clang_Use_Assertions   := 0
# Select default optimized mode, valid values are '0' and '1'.
Clang_Use_Optimized    := 1
# Set the Clang version.
Clang_Version          := 8.0.0

# Use LTO for clang but not clang_device
ifeq ($(RC_ProjectName),clang)
Clang_Enable_LTO := 1
else
Clang_Enable_LTO := 0
endif

##
# Include build logic.
include ClangBNI.mk

# CMake build
include src/tools/clang/utils/buildit/Project.mk
include src/projects/compiler-rt/utils/buildit/Project.mk

