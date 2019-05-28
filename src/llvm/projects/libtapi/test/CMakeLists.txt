# Test runner infrastructure for TAPI. This configures the TAPI test trees
# for use by Lit, and delegates to LLVM's lit test handlers.

if (CMAKE_CFG_INTDIR STREQUAL ".")
  set(LLVM_BUILD_MODE ".")
else ()
  set(LLVM_BUILD_MODE "%(build_mode)s")
endif ()

string(REPLACE ${CMAKE_CFG_INTDIR} ${LLVM_BUILD_MODE} TAPI_TOOLS_DIR ${LLVM_RUNTIME_OUTPUT_INTDIR})

configure_lit_site_cfg(
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.site.cfg.in
  ${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg
  )

configure_lit_site_cfg(
  ${CMAKE_CURRENT_SOURCE_DIR}/Unit/lit.site.cfg.in
  ${CMAKE_CURRENT_BINARY_DIR}/Unit/lit.site.cfg
  )

list(APPEND TAPI_TEST_DEPS
  tapi
  tapi-run
  clang-headers
  FileCheck
  not
  TestInputs
  yaml2obj
  tapi-frontend
  )

set(TAPI_TEST_PARAMS
  tapi_site_config=${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg
  )

add_lit_testsuite(check-tapi "Running the TAPI regression tests"
  ${CMAKE_CURRENT_BINARY_DIR}
  #LIT ${LLVM_LIT}
  PARAMS ${TAPI_TEST_PARAMS}
  DEPENDS ${TAPI_TEST_DEPS}
  ARGS ${TAPI_TEST_EXTRA_ARGS}
  )
set_target_properties(check-tapi PROPERTIES FOLDER "Tapi Tests")

include(ExternalProject)
ExternalProject_Add(TestInputs
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Inputs
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/Inputs
  INSTALL_DIR ${TAPI_BINARY_DIR}/Inputs
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${TAPI_BINARY_DIR}/Inputs
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DCMAKE_BUILD_TYPE=Release
  LOG_CONFIGURE 1
  LOG_BUILD 1
  LOG_INSTALL 1
  )
ExternalProject_Add_Step(TestInputs forceconfigure
  COMMAND ${CMAKE_COMMAND} -E echo "Force configure of TestInputs"
  DEPENDEES update
  DEPENDERS configure
  ALWAYS 1
  )
