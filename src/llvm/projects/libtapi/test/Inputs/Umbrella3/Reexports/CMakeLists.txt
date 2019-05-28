add_library(Foo
  SHARED
  Foo.m
  )

add_library(Bar
  SHARED
  Bar.m
  )

add_library(Baz
  SHARED
  Baz.m
  )

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(LINKER_FLAGS "-current_version 1 -compatibility_version 1")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")

set_target_properties(Foo
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

set_target_properties(Bar
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

set_target_properties(Baz
  PROPERTIES
  INSTALL_NAME_DIR /usr/lib
  VERSION 1
  SOVERSION 1
  )

install(TARGETS Foo
  FRAMEWORK DESTINATION System/Library/Frameworks
  )

install(TARGETS Bar
  FRAMEWORK DESTINATION System/Library/Frameworks
  )

install(TARGETS Baz
  LIBRARY DESTINATION usr/lib
  )
