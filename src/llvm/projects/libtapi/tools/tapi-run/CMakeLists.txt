add_tapi_executable(tapi-run
  tapi-run.cpp
  )

target_link_libraries(tapi-run
  tapiCore
  libtapi
  )

set_property(TARGET tapi-run APPEND_STRING
  PROPERTY
  LINK_FLAGS " -client_name ld"
  )

install(TARGETS tapi-run
  RUNTIME DESTINATION bin
  COMPONENT tapi-run
  )
add_custom_target(install-tapi-run
  DEPENDS tapi-run
  COMMAND "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=tapi-run
          -P "${CMAKE_BINARY_DIR}/cmake_install.cmake"
  )
