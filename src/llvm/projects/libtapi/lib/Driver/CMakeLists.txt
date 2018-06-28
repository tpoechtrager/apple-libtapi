add_tapi_library(tapiDriver
  ArchiveDriver.cpp
  Diagnostics.cpp
  DirectoryScanner.cpp
  Driver.cpp
  DriverOptions.cpp
  DriverUtils.cpp
  GenerateAPITestsDriver.cpp
  InstallAPIDriver.cpp
  Options.cpp
  ReexportDriver.cpp
  ScanDriver.cpp
  SDKDBDriver.cpp
  SDKDBVerifier.cpp
  StubDriver.cpp
  Snapshot.cpp
  SnapshotFileSystem.cpp

  DEPENDS
  TapiDriverOptions

  LINK_LIBS
  tapiConfig
  tapiCore
  tapiScanner
  tapiSDKDB
  )
