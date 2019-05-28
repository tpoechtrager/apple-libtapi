set(SOURCES
  Simple.m
  )
set(PUBLIC_HEADERS
  Simple.h
  Basic.h
  External.h
  SimpleAPI.h
  )
set(PRIVATE_HEADERS
  SimplePrivate.h
  SimplePrivateSPI.h
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1.2.3.4.5 -compatibility_version 1")

add_library(Simple
  SHARED
  ${PUBLIC_HEADERS}
  ${PRIVATE_HEADERS}
  ${SOURCES}
  )

target_link_libraries(Simple
  objc
  "-framework Foundation"
  "-framework CoreData"
  )

set_target_properties(Simple
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  PRIVATE_HEADER "${PRIVATE_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1.2.3.4.5
  SOVERSION 1
  )

install(TARGETS Simple
  FRAMEWORK DESTINATION System/Library/Frameworks
  )

install(TARGETS Simple
  FRAMEWORK DESTINATION Simple/System/Library/Frameworks
  )
