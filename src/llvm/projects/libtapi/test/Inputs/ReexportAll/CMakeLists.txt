add_subdirectory(SourceLib)

set(SOURCES
  Reexport.m
  )
set(PUBLIC_HEADERS
  Header1.h
  Header2.h
  Header3.h
  )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch i386 -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1 -Xarch_i386 -Wl,-reexported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/reexport.i386.symbols -Xarch_x86_64 -Wl,-reexported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/reexport.x86_64.symbols")

add_library(ReexportAll
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )
add_dependencies(ReexportAll ReexportSourceLibAll)

set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1 -Wl,-reexport_library,ReexportAll/SourceLib/libSource.1.dylib")

target_link_libraries(ReexportAll ReexportSourceLibAll)

set_target_properties(ReexportAll
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS ReexportAll
  FRAMEWORK DESTINATION Reexport/System/Library/Frameworks
  )
