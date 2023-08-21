set( LLVM_LINK_COMPONENTS
  Object
  BinaryFormat
  )

add_tapi_library(tapiObjCMetadata
  ObjCMachOBinary.cpp
  ObjCMetadata.cpp
  )
