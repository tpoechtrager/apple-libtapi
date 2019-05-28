set(SOURCES
  SubFramework.c
  )
set(PUBLIC_HEADERS
  SubFramework.h
  )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(LINKER_FLAGS "-current_version 1 -compatibility_version 1 -umbrella test")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")

add_library(SubFramework
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

set_target_properties(SubFramework
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER ${PUBLIC_HEADERS}
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS SubFramework
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
