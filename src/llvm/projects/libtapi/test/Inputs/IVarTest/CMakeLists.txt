set(SOURCES
  IVarTest.m
  )
set(PUBLIC_HEADERS
  IVarTest.h
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch i386 -arch x86_64 -fobjc-arc")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(IVarTest
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

target_link_libraries(IVarTest
  objc
  "-framework Foundation"
  )

set_target_properties(IVarTest
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS IVarTest
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
