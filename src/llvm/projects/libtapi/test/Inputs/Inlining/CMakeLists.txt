add_subdirectory(Bar)
add_subdirectory(Foo)

set(SOURCES
  Inlining.m
  )

add_library(Inlining
  SHARED
  ${SOURCES}
  )
add_dependencies(Inlining InliningBar InliningFoo)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch i386 -arch x86_64 -arch x86_64h")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1 -F${CMAKE_CURRENT_BINARY_DIR}/../Public -F${CMAKE_CURRENT_BINARY_DIR}/Bar -F${CMAKE_CURRENT_BINARY_DIR}/Foo  -Xlinker -reexport_framework -Xlinker Bar -Xlinker -reexport_framework -Xlinker Foo")

set_target_properties(Inlining
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS Inlining
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
