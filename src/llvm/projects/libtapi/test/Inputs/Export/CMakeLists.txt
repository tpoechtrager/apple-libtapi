set(SOURCES
  Export.m
  NonExport.m
  )
set(PUBLIC_HEADERS
  Export.h
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(Export
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

set_target_properties(Export
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

target_link_libraries(Export
  objc
  "-framework Foundation"
  )

install(TARGETS Export
  FRAMEWORK DESTINATION Export/System/Library/Frameworks
  )
