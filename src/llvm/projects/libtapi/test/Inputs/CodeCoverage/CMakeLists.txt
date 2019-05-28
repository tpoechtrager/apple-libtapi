set(SOURCES
  CodeCoverage.m
  )
set(PUBLIC_HEADERS
  CodeCoverage.h
  )

set(LIBRARY_VERSION_FLAGS "-current_version 1 -compatibility_version 1")
set(CLANG_CODE_COVERAGE_FLAGS "-fprofile-instr-generate")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CLANG_CODE_COVERAGE_FLAGS}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LIBRARY_VERSION_FLAGS} ${CLANG_CODE_COVERAGE_FLAGS}")

add_library(CodeCoverage
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

set_target_properties(CodeCoverage
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER ${PUBLIC_HEADERS}
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
)

install(TARGETS CodeCoverage
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
