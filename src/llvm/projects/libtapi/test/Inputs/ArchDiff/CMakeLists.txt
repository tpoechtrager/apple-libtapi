set(SOURCES
  arch-diff.c
  )
set(PUBLIC_HEADERS
  arch-diff.h
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64 -arch i386")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(ArchDiff
  SHARED
  ${SOURCES}
  ${PUBLIC_HEADERS}
  )

set_target_properties(ArchDiff
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS ArchDiff
  FRAMEWORK DESTINATION ArchDiff/System/Library/Frameworks
  )
