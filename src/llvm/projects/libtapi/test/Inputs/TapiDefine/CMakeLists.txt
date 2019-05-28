set(SOURCES
  TapiDefine.c
  )
set(PUBLIC_HEADERS
  TapiDefine.h
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(LIBRARY_VERSION_FLAGS "-current_version 1 -compatibility_version 1")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LIBRARY_VERSION_FLAGS}")

add_library(TapiDefine
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

set_target_properties(TapiDefine
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER ${PUBLIC_HEADERS}
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS TapiDefine
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
