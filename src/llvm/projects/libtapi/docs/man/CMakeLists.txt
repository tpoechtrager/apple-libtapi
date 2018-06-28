set(TAPI_MAN_FILES
  tapi.1
  tapi-archive.1
  tapi-stubify.1
  tapi-installapi.1
  tapi-reexport.1
  )

install(
  FILES ${TAPI_MAN_FILES}
  DESTINATION share/man/man1
  COMPONENT tapi-docs
  )

add_custom_target(tapi-docs)
add_custom_target(install-tapi-docs
  COMMAND "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=tapi-docs
           -P "${CMAKE_BINARY_DIR}/cmake_install.cmake"
  )
