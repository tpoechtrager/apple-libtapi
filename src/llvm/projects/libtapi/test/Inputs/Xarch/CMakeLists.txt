set(SOURCES
  Xarch.c
  )
set(PUBLIC_HEADERS
  Xarch.h
  )


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch i386 -arch x86_64 -Xarch_i386 -Wl,-allowable_client,Foo -Xarch_x86_64 -Wl,-reexport_framework,Foundation")
set(LINKER_FLAGS "-current_version 1 -compatibility_version 1")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")

add_library(Xarch
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

set_target_properties(Xarch
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS Xarch
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
