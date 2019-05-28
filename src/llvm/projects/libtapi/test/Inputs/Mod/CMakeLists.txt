set(SOURCES
  Mod.m
  )
set(PUBLIC_HEADERS
  Mod.h
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64 -fmodules -fmodules-validate-system-headers")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(Mod
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

target_link_libraries(Mod
  objc
  "-framework Foundation"
  )

set_target_properties(Mod
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS Mod
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
