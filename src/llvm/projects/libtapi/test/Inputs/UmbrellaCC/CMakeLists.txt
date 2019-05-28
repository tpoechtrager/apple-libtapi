add_subdirectory(Reexports)

set(SOURCES
  UmbrellaCC.m
  )
set(PUBLIC_HEADERS
  UmbrellaCC.h
  )

set(LIBRARY_VERSION_FLAGS "-current_version 1 -compatibility_version 1")
set(CLANG_CODE_COVERAGE_FLAGS "-fprofile-instr-generate")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CLANG_CODE_COVERAGE_FLAGS}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LIBRARY_VERSION_FLAGS} ${CLANG_CODE_COVERAGE_FLAGS} -F${CMAKE_CURRENT_BINARY_DIR}/Reexports -Wl,-reexport_framework,FooCC")

add_library(UmbrellaCC
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )
add_dependencies(UmbrellaCC FooCC)

set_target_properties(UmbrellaCC
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER ${PUBLIC_HEADERS}
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
)

install(TARGETS UmbrellaCC
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
