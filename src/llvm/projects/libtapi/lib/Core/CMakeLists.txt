set( LLVM_LINK_COMPONENTS  
  Object
#  ObjCMetadata
  BinaryFormat
  )

add_tapi_library(tapiCore
  Architecture.cpp
  ArchitectureSet.cpp
  ArchitectureSupport.cpp
  AvailabilityInfo.cpp
  Configuration.cpp
  ConfigurationFile.cpp
  FakeSymbols.cpp
  FileManager.cpp
  FileSystem.cpp
  Framework.cpp
  ExtendedInterfaceFile.cpp
  InterfaceFile.cpp
  InterfaceFileBase.cpp
  InterfaceFileManager.cpp
  JSONFile.cpp
  MachODylibReader.cpp
  Path.cpp
  ReexportFileWriter.cpp
  Registry.cpp
  Symbol.cpp
  TextAPI_v1.cpp
  TextStub_v1.cpp
  TextStub_v2.cpp
  TextStub_v3.cpp
  Utils.cpp
  XPI.cpp
  XPISet.cpp
  YAMLReaderWriter.cpp
  
  LINK_LIBS
  clangBasic
  )
