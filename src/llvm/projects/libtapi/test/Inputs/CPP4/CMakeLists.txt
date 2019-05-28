add_definitions(-std=c++11)

set(SOURCES
  CPP4.cpp
  )
set(PUBLIC_HEADERS
  Templates.h
  )

set(LIBRARY_VERSION_FLAGS "-current_version 1 -compatibility_version 1")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LIBRARY_VERSION_FLAGS}")

add_library(CPP4
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

set_target_properties(CPP4
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS CPP4
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
