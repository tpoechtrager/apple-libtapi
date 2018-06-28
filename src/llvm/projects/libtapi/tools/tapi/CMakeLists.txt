set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/tapi.exports)

add_tapi_executable(tapi
  tapi.cpp
  )

target_link_libraries(tapi
  tapiDriver
  )

if (TAPI_BUILD_LIBIOSSDK)
target_link_libraries(tapi
  iOSSDK
  )
endif()

install(TARGETS tapi
  RUNTIME DESTINATION bin
  COMPONENT tapi
  )

add_custom_target(install-tapi
  DEPENDS tapi
  COMMAND "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=tapi
          -P "${CMAKE_BINARY_DIR}/cmake_install.cmake"
  )
