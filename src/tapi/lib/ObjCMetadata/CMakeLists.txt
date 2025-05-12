add_llvm_component_library(LLVMObjCMetadata
  ObjCMetadata.cpp
  ObjCMachOBinary.cpp

  ADDITIONAL_HEADER_DIRS
  ${LLVM_MAIN_INCLUDE_DIR}/llvm/ObjCMetadata

  DEPENDS
  intrinsics_gen

  LINK_COMPONENTS
  Analysis
  Core
  Object
  Support
  )
