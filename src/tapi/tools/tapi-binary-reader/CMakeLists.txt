set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/tapi-binary-reader.exports)

set(LLVM_LINK_COMPONENTS
  Support
  )

add_tapi_executable(tapi-binary-reader
  tapi-binary-reader.cpp
  )

target_link_libraries(tapi-binary-reader
  PRIVATE
  tapiCore
  tapiObjCMetadata
  )

install(TARGETS tapi-binary-reader
  RUNTIME DESTINATION bin
  COMPONENT tapi-binary-reader
  )
add_llvm_install_targets(install-tapi-binary-reader
  DEPENDS tapi-binary-reader
  COMPONENT tapi-binary-reader
  )
