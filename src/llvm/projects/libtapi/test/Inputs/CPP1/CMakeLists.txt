add_definitions(-std=c++11)

set(SOURCES
  CPP1.cpp
  )
set(PUBLIC_HEADERS
  CPP.h
  )

set(LIBRARY_VERSION_FLAGS "-current_version 1 -compatibility_version 1")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LIBRARY_VERSION_FLAGS}")

add_library(CPP1
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

set_target_properties(CPP1
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS CPP1
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
