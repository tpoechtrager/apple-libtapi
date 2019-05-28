set(SOURCES
  Umbrella1.m
  )
set(PUBLIC_HEADERS
  Headers/AAA.h
  Headers/Umbrella1.h
  Headers/Umbrella1_Private.h
  )
set(PRIVATE_HEADERS
  PrivateHeaders/AAAA.h
  PrivateHeaders/Umbrella1Private.h
  )

add_library(Umbrella1
  SHARED
  ${PUBLIC_HEADERS}
  ${PRIVATE_HEADERS}
  ${SOURCES}
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

set_target_properties(Umbrella1
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  PRIVATE_HEADER "${PRIVATE_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS Umbrella1
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
