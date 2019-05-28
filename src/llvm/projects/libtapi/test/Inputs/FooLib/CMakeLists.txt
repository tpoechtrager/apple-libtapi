set(SOURCES
  foo.c
  )
set(PUBLIC_HEADERS
  foo.h
  )
set(PRIVATE_HEADERS
  foo_private.h
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(FooLib
  SHARED
  ${PUBLIC_HEADERS}
  ${PRIVATE_HEADERS}
  ${SOURCES}
  )

set_target_properties(FooLib
  PROPERTIES
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  PRIVATE_HEADER "${PRIVATE_HEADERS}"
  INSTALL_NAME_DIR /usr/lib
  VERSION 1
  SOVERSION 1
  OUTPUT_NAME foo
  )

install(TARGETS FooLib
  LIBRARY DESTINATION dylib_dstroot/usr/lib
  PUBLIC_HEADER DESTINATION dylib_dstroot/usr/include
  PRIVATE_HEADER DESTINATION dylib_dstroot/usr/local/include
  )
