set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/tapi-sdkdb.exports)

set(LLVM_LINK_COMPONENTS
  Support
  )

add_tapi_executable(tapi-sdkdb
  tapi-sdkdb.cpp
  )

target_link_libraries(tapi-sdkdb
  PRIVATE
  tapiCore
  tapiSDKDB
  tapiDriver
  )

install(TARGETS tapi-sdkdb
  RUNTIME DESTINATION bin
  COMPONENT tapi-sdkdb
  )
add_llvm_install_targets(install-tapi-sdkdb
  DEPENDS tapi-sdkdb
  COMPONENT tapi-sdkdb
  )
