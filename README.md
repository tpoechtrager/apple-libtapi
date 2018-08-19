# Apple TAPI library, Version 1.30.0 # 

Sources taken from: 

https://opensource.apple.com/tarballs/clang  
https://opensource.apple.com/tarballs/tapi 

## Dependencies: ##

CMake, Python and GCC 4.8+ or Clang

## Installation: ##

    [INSTALLPREFIX=<prefix>] ./build.sh  
    ./install.sh

## What is TAPI? ##

[TAPI](https://github.com/ributzka/tapi) is a text file format for describing the contents of a shared library file on Apple's MacOS computer operating system. For example, the zlib data compression library has a Mach-O binary shared-library file called libz.dylib, and a corresponding TAPI textfile called libz.tbd. The TBD contains information such as location of the dylib file (/usr/lib/libz.1.dylib), library version, list of architectures supported (i386, x86_64), and a list of symbols within the library ( _adler32, _deflate, _compress, ..).

## Travis CI ##

[![Build Status](https://travis-ci.org/tpoechtrager/apple-libtapi.svg?branch=1.30.0)](https://travis-ci.org/tpoechtrager/apple-libtapi)

