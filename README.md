# Apple TAPI library - Version 1300.6.5; API: 1.6.0 #

Sources taken from: 

https://opensource.apple.com/tarballs/tapi  
https://github.com/apple/llvm-project/tree/apple/stable/20210107

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
