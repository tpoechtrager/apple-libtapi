set( LLVM_LINK_COMPONENTS
  Object
  BinaryFormat
  )

add_tapi_library(tapiCore
  API.cpp
  APIJSONSerializer.cpp
  APIPrinter.cpp
  APIVisitor.cpp
  AvailabilityInfo.cpp
  FakeSymbols.cpp
  ClangDiagnostics.cpp
  FileListReader.cpp
  FileManager.cpp
  FileSystem.cpp
  Framework.cpp
  HeaderFile.cpp
  InterfaceFile.cpp
  InterfaceFileManager.cpp
  MachODylibReader.cpp
  MachOReader.cpp
  PackedVersion.cpp
  Path.cpp
  ReexportFileWriter.cpp
  Registry.cpp
  TapiError.cpp
  TextStub.cpp
  TextStubv4.cpp
  TextStubCommon.cpp
  Utils.cpp
  XPI.cpp
  XPISet.cpp
  YAMLReaderWriter.cpp

  LINK_LIBS
  clangBasic
  )
