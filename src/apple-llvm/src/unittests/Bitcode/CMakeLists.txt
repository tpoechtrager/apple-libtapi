set(LLVM_LINK_COMPONENTS
  AsmParser
  BitReader
  BitWriter
  Core
  Support
  )

add_llvm_unittest(BitcodeTests
  AppleInternalBitcodeVersioning.cpp
  BitReaderTest.cpp
  BitstreamReaderTest.cpp
  BitstreamWriterTest.cpp
  )
