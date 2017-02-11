set(LLVM_LINK_COMPONENTS
  Analysis
  Core
  IRReader
  ObjCMetadata
  Object
  Support
  )

find_package(LibXml2)
if (NOT LIBXML2_FOUND)
  message(FATAL_ERROR "api-analyzer requires libxml")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    SYSTEM ${LIBXML2_INCLUDE_DIR})

add_llvm_library(APIAnalysis
  APIAnalysis.cpp
  ArchiveAnalyzer.cpp
  ModuleAnalyzer.cpp
  BinaryAnalyzer.cpp
  XarAnalyzer.cpp
  )

target_link_libraries(APIAnalysis INTERFACE xar)

add_dependencies(APIAnalysis intrinsics_gen)

install(DIRECTORY include/llvm
    DESTINATION include
    COMPONENT api-analyzer-headers
    FILES_MATCHING
    PATTERN "*.h"
    )
