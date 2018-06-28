add_tapi_library(tapiScanner
  APIScanner.cpp
  Scanner.cpp
  
  LINK_LIBS
  clangBasic
  clangTooling
  )
