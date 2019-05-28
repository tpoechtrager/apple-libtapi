set(SOURCES
  Framework.c
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch i386 -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(SubFrameworks
  SHARED
  ${SOURCES}
  )

set_target_properties(SubFrameworks
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS SubFrameworks
  FRAMEWORK DESTINATION SubFrameworks/System/Library/Frameworks
  )

# Add a standard subframework
add_library(SubFrameworksStandard
  SHARED
  SubFramework1.c
  )

set_target_properties(SubFrameworksStandard
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS SubFrameworksStandard
  FRAMEWORK DESTINATION SubFrameworks/System/Library/Frameworks/SubFrameworks.framework/Frameworks
  )

# Add a non-standard subframework
add_library(SubFrameworksNonStandard
  SHARED
  SubFramework2.c
  )

set_target_properties(SubFrameworksNonStandard
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS SubFrameworksNonStandard
  FRAMEWORK DESTINATION SubFrameworks/System/Library/Frameworks/SubFrameworks.framework/NonStandard
  )
