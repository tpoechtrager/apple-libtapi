set(SOURCES
  Foo.m
  )
set(PUBLIC_HEADERS
  Foo.h
  )

add_library(InliningFoo
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch i386 -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

set_target_properties(InliningFoo
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks/Inlining.framework/Versions/A/Frameworks
  VERSION 1
  SOVERSION 1
  OUTPUT_NAME Foo
  )

install(TARGETS InliningFoo
  FRAMEWORK DESTINATION System/Library/Frameworks/Inlining.framework/Versions/A/Frameworks
  )
