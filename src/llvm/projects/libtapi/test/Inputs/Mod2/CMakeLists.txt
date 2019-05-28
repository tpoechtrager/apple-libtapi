set(SOURCES
  Mod2.m
  )
set(PUBLIC_HEADERS
  Mod2.h
  A.h
  B.h
  C.h
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(Mod2
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

set_target_properties(Mod2
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS Mod2
  FRAMEWORK DESTINATION System/Library/Frameworks
  )

install(
  FILES module.modulemap
  DESTINATION System/Library/Frameworks/Mod2.framework/Versions/A/Modules/
  )

install(CODE "execute_process( \
              COMMAND ${CMAKE_COMMAND} -E create_symlink \
              Versions/A/Modules \
              ${CMAKE_INSTALL_PREFIX}/System/Library/Frameworks/Mod2.framework/Modules \
              )"
  )
