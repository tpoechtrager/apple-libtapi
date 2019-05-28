set(SOURCES
  system.c
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(SymlinkTest
  SHARED
  ${SOURCES}
  )

set_target_properties(SymlinkTest
  PROPERTIES
  INSTALL_NAME_DIR /usr/lib
  VERSION 1
  SOVERSION 1
  OUTPUT_NAME System
  )

install(TARGETS SymlinkTest
  LIBRARY DESTINATION symlinktest/usr/lib
  )
install(CODE "execute_process( \
      COMMAND ${CMAKE_COMMAND} -E create_symlink \
      /usr/lib/libSystem.dylib \
      ${CMAKE_INSTALL_PREFIX}/symlinktest/usr/lib/libfoo.dylib \
    )"
  )
