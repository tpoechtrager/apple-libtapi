set(SOURCES
  Public.c
  )
set(PUBLIC_HEADERS
  Public.h
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch i386 -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(Public
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

set_target_properties(Public
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS Public
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
