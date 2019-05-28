set(SOURCES
  Bar.m
  )
set(PUBLIC_HEADERS
  Bar.h
  )

add_library(InliningBar
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )
add_dependencies(InliningBar Public)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch i386 -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1 -F${CMAKE_CURRENT_BINARY_DIR}/../../Public -Xarch_x86_64 -Wl,-reexport_framework,Public")

set_target_properties(InliningBar
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks/Inlining.framework/Versions/A/Frameworks
  VERSION 1
  SOVERSION 1
  OUTPUT_NAME Bar
  )

install(TARGETS InliningBar
  FRAMEWORK DESTINATION System/Library/Frameworks/Inlining.framework/Versions/A/Frameworks
  )
