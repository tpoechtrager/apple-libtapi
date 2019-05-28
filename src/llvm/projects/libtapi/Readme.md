# TAPI

TAPI is a **T**ext-based **A**pplication **P**rogramming **I**nterface. It
replaces the Mach-O Dynamic Library Stub files in Apple's SDKs to reduce the SDK
size even further.

The text-based dynamic library stub file format (.tbd) is a human readable and
editable YAML text file. The _TAPI_ projects uses the _LLVM_ YAML parser to read
those files and provides this functionality to the linker as a dynamic library.


## Building TAPI

*   Create a source directory <src_dir>.
*   Clone the **LLVM** source code for the `swift-4.2-branch` from
    [GitHub Apple/swift-llvm](https://github.com/apple/swift-llvm) into the
    <src_dir>/llvm directory.

    `git clone git@github.com:apple/swift-llvm.git llvm -b swift-4.2-branch`

*   Clone the **CLANG** source code for the `swift-4.2-branch` from
    [GitHub Apple/swift-clang](https://github.com/apple/swift-clang) into the
    <src_dir>/clang directory.

    `git clone git@github.com:apple/swift-clang.git clang -b swift-4.2-branch`

*   Place the content of this tar archive into the <src_dir>/tapi directory.

*   Create a separate build directory and configure the project with CMake:
    ```
    cmake -G Ninja -C <src_dir>/tapi/cmake/caches/apple-tapi.cmake
          -DCMAKE_INSTALL_PREFIX=<install_dir> <src_dir>/llvm
    ```

*   The CMake cache file defines most of the settings for you, such as enabling
    LTO. It also specifies the distribution components to include all the
    targets needed for TAPI.

*   To build and install the **TAPI** project invoke:
    ```
    ninja install-distribution
    ```
    in the build directory.
