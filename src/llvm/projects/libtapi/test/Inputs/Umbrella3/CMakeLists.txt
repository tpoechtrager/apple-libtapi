add_subdirectory(Reexports)

set(SOURCES
  Umbrella3.m
  )
set(PUBLIC_HEADERS
  Headers/Umbrella3.h
  Headers/Foo.h
  Headers/Bar.h
  Headers/Baz.h
  )

add_library(Umbrella3
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )
add_dependencies(Umbrella3 Foo Bar Baz)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(LINKER_FLAGS "-current_version 1 -compatibility_version 1 -F${CMAKE_CURRENT_BINARY_DIR}/Reexports -L${CMAKE_CURRENT_BINARY_DIR}/Reexports -Wl,-reexport_framework,Foo -Wl,-reexport_framework,Bar -Wl,-reexport-lBaz")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")

set_target_properties(Umbrella3
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS Umbrella3
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
