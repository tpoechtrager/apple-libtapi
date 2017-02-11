add_llvm_library(LLVMObjCMetadata
  ObjCBitcode.cpp
  ObjCMetadata.cpp
  ObjCMachOBinary.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/ObjCMetadata

  DEPENDS
  intrinsics_gen
  )
