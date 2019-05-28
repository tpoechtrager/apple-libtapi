add_library(FooCC
  SHARED
  Foo.m
  )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-instr-generate")
set(LINKER_FLAGS "-current_version 1 -compatibility_version 1")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")

set_target_properties(FooCC
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS FooCC
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
