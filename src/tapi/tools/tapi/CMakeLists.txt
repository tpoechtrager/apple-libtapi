install(
  DIRECTORY ${LLVM_LIBRARY_OUTPUT_INTDIR}/clang/${CLANG_VERSION}/include
  DESTINATION lib${LLVM_LIBDIR_SUFFIX}/tapi/${TAPI_VERSION}/
  COMPONENT tapi-clang-resource-headers
  )

add_custom_target(tapi-clang-resource-headers ALL DEPENDS clang-resource-headers)
add_llvm_install_targets(install-tapi-clang-resource-headers
                         DEPENDS tapi-clang-resource-headers
                         COMPONENT tapi-clang-resource-headers)

set(LLVM_EXPORTED_SYMBOL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/tapi.exports)

add_tapi_executable(tapi
  tapi.cpp
  )

target_link_libraries(tapi
  PRIVATE
    tapiDriver
    tapiObjCMetadata
  )

add_dependencies(tapi tapi-clang-resource-headers)

install(TARGETS tapi
  RUNTIME DESTINATION bin
  COMPONENT tapi
  )

add_llvm_install_targets(install-tapi
                         DEPENDS tapi install-tapi-clang-resource-headers
                         COMPONENT tapi)
