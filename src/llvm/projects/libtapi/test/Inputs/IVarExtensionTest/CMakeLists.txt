set(SOURCES
  IVarExtensionTest.m
  )
set(PUBLIC_HEADERS
  IVarExtensionTest.h
  )
set(PRIVATE_HEADERS
  IVarExtensionTest_Private.h
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64 -fobjc-arc")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(IVarExtensionTest
  SHARED
  ${PUBLIC_HEADERS}
  ${PRIVATE_HEADERS}
  ${SOURCES}
  )

target_link_libraries(IVarExtensionTest
  objc
  "-framework Foundation"
  )

set_target_properties(IVarExtensionTest
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  PUBLIC_HEADER "${PUBLIC_HEADERS}"
  PRIVATE_HEADER "${PRIVATE_HEADERS}"
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS IVarExtensionTest
  FRAMEWORK DESTINATION System/Library/Frameworks
  )
