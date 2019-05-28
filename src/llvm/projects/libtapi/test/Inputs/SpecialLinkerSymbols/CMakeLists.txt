set(SOURCES
  SpecialLinkerSymbols.c
  )

set(PUBLIC_HEADERS
  SpecialLinkerSymbols.h
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(SpecialLinkerSymbols
  SHARED
  ${SOURCES}
  ${PUBLIC_HEADERS}
  )

set_target_properties(SpecialLinkerSymbols
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  INSTALL_NAME_DIR /System/Library/Frameworks
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  VERSION 1
  SOVERSION 1
  )

install(TARGETS SpecialLinkerSymbols
  FRAMEWORK DESTINATION System/Library/Frameworks
  )

