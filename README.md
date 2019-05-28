# Apple TAPI library - Version 2.0.0; API: 1.4.0 # 

Sources taken from: 

https://opensource.apple.com/tarballs/tapi  
https://github.com/apple/swift/tree/swift-4.2-branch  
https://github.com/apple/swift-clang/tree/swift-4.2-branch  

## Dependencies: ##

CMake, Python and GCC or Clang

## Installation: ##

    [INSTALLPREFIX=<prefix>] ./build.sh  
    ./install.sh

## What is TAPI? ##

TAPI is a **T**ext-based **A**pplication **P**rogramming **I**nterface. It
replaces the Mach-O Dynamic Library Stub files in Apple's SDKs to reduce the SDK
size even further.

The text-based dynamic library stub file format (.tbd) is a human readable and
editable YAML text file. The _TAPI_ projects uses the _LLVM_ YAML parser to read
those files and provides this functionality to the linker as a dynamic library.

## Travis CI ##

[![Build Status](https://travis-ci.org/tpoechtrager/apple-libtapi.svg?branch=2.0.0)](https://travis-ci.org/tpoechtrager/apple-libtapi)
