add_subdirectory(APIAnalysis)

set(LLVM_LINK_COMPONENTS
  ${LLVM_TARGETS_TO_BUILD}
  DebugInfoDWARF
  MC
  MCDisassembler
  Object
  Support
  )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/APIAnalysis/include)

add_llvm_tool(api-analyzer
  api-analyzer.cpp
  )

target_link_libraries(api-analyzer APIAnalysis)