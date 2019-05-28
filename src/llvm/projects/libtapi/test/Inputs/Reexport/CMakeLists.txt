add_subdirectory(SourceLib)

set(SOURCES
  Reexport.m
  )
set(PUBLIC_HEADERS
  Header1.h
  Header2.h
  Header3.h
  )

add_library(Reexport
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )
add_dependencies(Reexport ReexportSourceLib)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch i386 -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1 -Xarch_i386 -Wl,-reexported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/reexport.i386.symbols -Xarch_x86_64 -Wl,-reexported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/reexport.x86_64.symbols")

target_link_libraries(Reexport ReexportSourceLib)

set_target_properties(Reexport
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS Reexport
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
