# Apple TAPI library - Version 1600.0.11.8; API: 2.3.0 #

Sources taken from: 

https://github.com/apple-oss-distributions/tapi/releases/tag/tapi-1600.0.11.8  
https://github.com/swiftlang/llvm-project/releases/tag/swift-6.0-DEVELOPMENT-SNAPSHOT-2024-07-02-a

## Dependencies: ##

CMake, Python and Clang or GCC

## Installation of library: ##

    [INSTALLPREFIX=<prefix>] ./build.sh  
    ./install.sh

## Installation of library and tapi tools: ##

    # Requires clang and lld to be installed
    [INSTALLPREFIX=<prefix>] ./build_tapi_tools.sh  
    ./install.sh

## What is TAPI? ##

TAPI is a **T**ext-based **A**pplication **P**rogramming **I**nterface. It
replaces the Mach-O Dynamic Library Stub files in Apple's SDKs to reduce the SDK
size even further.

The text-based dynamic library stub file format (.tbd) is a human readable and
editable YAML text file. The _TAPI_ projects uses the _LLVM_ YAML parser to read
those files and provides this functionality to the linker as a dynamic library.
