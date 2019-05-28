set(SOURCES
  Source.m
  )

add_library(ReexportSourceLibAll
  SHARED
  ${PUBLIC_HEADERS}
  ${SOURCES}
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch i386 -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

target_link_libraries(ReexportSourceLibAll
  objc
  )

set_target_properties(ReexportSourceLibAll
  PROPERTIES
  INSTALL_NAME_DIR /System/Library/Frameworks/ReexportAll.framework/Versions/A/Frameworks
  VERSION 1
  SOVERSION 1
  OUTPUT_NAME Source
  )

install(TARGETS ReexportSourceLibAll
  LIBRARY DESTINATION Reexport/System/Library/Frameworks/ReexportAll.framework/Versions/A/Frameworks
  )
