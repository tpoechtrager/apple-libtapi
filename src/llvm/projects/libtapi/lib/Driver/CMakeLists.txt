#[[
add_tapi_library(tapiDriver
  API2XPIConverter.cpp
  ArchiveDriver.cpp
  Configuration.cpp
  ConfigurationFile.cpp
  DirectoryScanner.cpp
  Driver.cpp
  DriverOptions.cpp
  DriverUtils.cpp
  HeaderGlob.cpp
  InstallAPIDriver.cpp
  Options.cpp
  ReexportDriver.cpp
  StubDriver.cpp
  Snapshot.cpp
  SnapshotFileSystem.cpp

  DEPENDS
  TapiDriverOptions

  LINK_LIBS
  tapiConfig
  tapiCore
  tapiDiagnostics
  tapiFrontend
  )
]]
