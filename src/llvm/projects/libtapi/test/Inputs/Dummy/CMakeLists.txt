set(SOURCES
  Dummy.m
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1.2.3 -compatibility_version 1")

add_library(Dummy
  SHARED
  ${SOURCES}
  )

set_target_properties(Dummy
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1.2.3
  SOVERSION 1
  OUTPUT_NAME Dummy.extension
  )

install(TARGETS Dummy
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
