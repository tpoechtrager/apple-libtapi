set(SOURCES
  test.c
  )
set(PUBLIC_HEADERS
  test.h
  test_exclude.h
  )
set(PRIVATE_HEADERS
  test_private.h
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(Test
  SHARED
  ${PUBLIC_HEADERS}
  ${PRIVATE_HEADERS}
  ${SOURCES}
  )

set_target_properties(Test
  PROPERTIES
  INSTALL_NAME_DIR /usr/lib
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  PRIVATE_HEADER "${PRIVATE_HEADERS}"
  VERSION 1
  SOVERSION 1
  )

install(TARGETS Test
  LIBRARY DESTINATION UsrLib/usr/lib
  PUBLIC_HEADER DESTINATION UsrLib/usr/include
  PRIVATE_HEADER DESTINATION UsrLib/usr/local/include
  )
