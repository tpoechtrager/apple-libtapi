set(SOURCES
  ABrokenSymlink.m
  )
set(PUBLIC_HEADERS
  ABrokenSymlink.h
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(ABrokenSymlink
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

set_target_properties(ABrokenSymlink
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS ABrokenSymlink
  FRAMEWORK DESTINATION System/Library/Frameworks
  )

install(CODE "execute_process( \
              COMMAND ${CMAKE_COMMAND} -E create_symlink \
              Versions/A/ABrokenSymlink.tbd \
              ${CMAKE_INSTALL_PREFIX}/System/Library/Frameworks/ABrokenSymlink.framework/AAA.tbd \
              )"
  )

install(CODE "execute_process( \
              COMMAND ${CMAKE_COMMAND} -E create_symlink \
              Versions/A/ABrokenSymlink.tbd \
              ${CMAKE_INSTALL_PREFIX}/System/Library/Frameworks/ABrokenSymlink.framework/BBB.tbd \
              )"
  )

install(CODE "execute_process( \
              COMMAND ${CMAKE_COMMAND} -E create_symlink \
              no_such_file.h \
              ${CMAKE_INSTALL_PREFIX}/System/Library/Frameworks/ABrokenSymlink.framework/Versions/A/Headers/AAA.h \
              )"
  )
