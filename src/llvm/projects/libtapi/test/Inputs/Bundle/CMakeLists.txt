set(SOURCES
  bundle.m
  )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -arch x86_64 -arch i386")

add_library(Bundle
  MODULE
  ${SOURCES}
  )

target_link_libraries(Bundle
  objc
  "-framework Foundation"
  "-framework CoreData"
  )

set_target_properties(Bundle
  PROPERTIES
  PREFIX ""
  SUFFIX ""
  )

install(TARGETS Bundle
  LIBRARY DESTINATION Bundle/System/Library/Bundles
  )
