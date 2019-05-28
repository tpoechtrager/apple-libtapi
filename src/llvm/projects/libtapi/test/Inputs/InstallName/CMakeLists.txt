set(SOURCES
  InstallName.c
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64")
set(CMAKE_SHARED_LINKER_FLAGS "-current_version 1 -compatibility_version 1")

add_library(InstallName
  SHARED
  ${SOURCES}
  )

set_target_properties(InstallName
  PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  INSTALL_NAME_DIR /System/Library/Frameworks
  VERSION 1
  SOVERSION 1
  )

install(TARGETS InstallName
  FRAMEWORK DESTINATION System/Library/Frameworks
  )

install(CODE "execute_process( \
              COMMAND install_name_tool -id /System/Library/Frameworks/InstallName.framework/Versions/A/Frameworks/libInstallName.dylib \
              ${CMAKE_INSTALL_PREFIX}/System/Library/Frameworks/InstallName.framework/Versions/A/InstallName \
        )"
  )
