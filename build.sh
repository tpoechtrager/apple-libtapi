#!/usr/bin/env bash

set -e

TAPI_VERSION=1300.6.5

pushd "${0%/*}" &>/dev/null
source tools/tools.sh

if [[ "$(basename "$0")" == *tapi_tools* ]]; then
  BUILD_TAPI_TOOLS=1
  [[ "$CC" != *clang* ]] && export CC="clang"
  [[ "$CXX" != *clang++* ]] && export CXX="clang++"
  command -v lld &>/dev/null || command -v ld.lld &>/dev/null || command -v ld64.lld &>/dev/null || {
    echo "Missing lld" 1>&2
    exit 1
  }
  CMAKE_EXE_LINKER_FLAGS+=" -fuse-ld=lld"
  CMAKE_SHARED_LINKER_FLAGS+=" -fuse-ld=lld"
fi

rm -rf build
mkdir build

pushd build &>/dev/null

CMAKE_EXTRA_ARGS=""

if [ "$OPERATING_SYSTEM" = "Android" ]; then
  export CC="$CC -D__ANDROID_API__=26"
  export CXX="$CXX -D__ANDROID_API__=26"
fi

case "$TARGET" in
  Darwin)
    #export MACOSX_DEPLOYMENT_TARGET=10.9
    CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=Darwin "
    CC="$(xcrun -f clang) -stdlib=libc++"
    CXX="$(xcrun -f clang++) -stdlib=libc++"
    export CC CXX
  ;;
  iOS)
    unset MACOSX_DEPLOYMENT_TARGET
    CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=Darwin "
    export CC="arm-apple-darwin11-clang -stdlib=libc++"
    export CXX="arm-apple-darwin11-clang++ -stdlib=libc++"
  ;;
  iOS-ARMV7)
    unset MACOSX_DEPLOYMENT_TARGET
    CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=Darwin "
    export CC="arm-apple-darwin11-clang -arch armv7 -stdlib=libc++"
    export CXX="arm-apple-darwin11-clang++ -arch armv7 -stdlib=libc++"
  ;;
  FreeBSD)
    CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=FreeBSD "
    export CC="amd64-pc-freebsd13.0-clang"
    export CXX="amd64-pc-freebsd13.0-clang++"
  ;;
  MINGW64)
    CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=Windows "
    export CC="x86_64-w64-mingw32-gcc"
    export CXX="x86_64-w64-mingw32-g++"
  ;;
  MINGW32)
    CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=Windows "
    export CC="i686-w64-mingw32-gcc"
    export CXX="i686-w64-mingw32-g++"
  ;;
esac

if [ -z "$INSTALLPREFIX" ]; then
  INSTALLPREFIX="/usr/local"
fi

# TODO: Fix this in a better way.
INCLUDE_FIX="-I $PWD/../src/llvm/projects/clang/include "
INCLUDE_FIX+="-I $PWD/projects/clang/include "

printf '%s' "$INSTALLPREFIX" > INSTALLPREFIX

cmake ../src/llvm \
 -DCMAKE_CXX_FLAGS="$INCLUDE_FIX" \
 -DCMAKE_SHARED_LINKER_FLAGS="$CMAKE_SHARED_LINKER_FLAGS" \
 -DCMAKE_EXE_LINKER_FLAGS="$CMAKE_EXE_LINKER_FLAGS" \
 -DLLVM_INCLUDE_TESTS=OFF \
 -DCMAKE_BUILD_TYPE=RELEASE \
 -DLLVM_ENABLE_PROJECTS="tapi;clang" \
 -DCMAKE_INSTALL_PREFIX="$INSTALLPREFIX" \
 -DTAPI_REPOSITORY_STRING=$TAPI_VERSION \
 -DTAPI_FULL_VERSION=$TAPI_VERSION \
 $CMAKE_EXTRA_ARGS

echo ""
echo "## Building clangBasic ##"
echo ""

$MAKE clangBasic -j $JOBS

echo ""
echo "## Building libtapi ##"
echo ""

$MAKE libtapi -j $JOBS

if [ -n "$BUILD_TAPI_TOOLS" ]; then
  echo ""
  echo "## Building tapi tools ##"
  echo ""

  $MAKE tapi tapi-binary-reader tapi-run tapi-sdkdb -j $JOBS
fi

popd &>/dev/null
popd &>/dev/null
