#!/usr/bin/env bash

set -e

TAPI_REPOSITORY=1100.0.11
TAPI_VERSION=11.0.0 # ?!

pushd "${0%/*}" &>/dev/null
source tools/tools.sh

rm -rf build
mkdir build

pushd build &>/dev/null

CMAKE_EXTRA_ARGS=""

if [ $OPERATING_SYSTEM == "Android" ]; then
  export CC="$CC -D__ANDROID_API__=26"
  export CXX="$CXX -D__ANDROID_API__=26"
fi

if [ "$TARGET" == "Darwin" ]; then
  #export MACOSX_DEPLOYMENT_TARGET=10.9
  CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=Darwin "
  export CC="$(xcrun -f clang) -stdlib=libc++"
  export CXX="$(xcrun -f clang++) -stdlib=libc++"
elif [ "$TARGET" == "iOS" ]; then
  unset MACOSX_DEPLOYMENT_TARGET
  CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=Darwin "
  export CC="arm-apple-darwin11-clang -stdlib=libc++"
  export CXX="arm-apple-darwin11-clang++ -stdlib=libc++"
elif [ "$TARGET" == "iOS-ARMV7" ]; then
  unset MACOSX_DEPLOYMENT_TARGET
  CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=Darwin "
  export CC="arm-apple-darwin11-clang -arch armv7 -stdlib=libc++"
  export CXX="arm-apple-darwin11-clang++ -arch armv7 -stdlib=libc++"
elif [ "$TARGET" == "FreeBSD" ]; then
  CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=FreeBSD "
  export CC=amd64-pc-freebsd13.0-clang
  export CXX=amd64-pc-freebsd13.0-clang++
elif [ "$TARGET" == "MINGW64" ]; then
  CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=Windows "
  export CC=x86_64-w64-mingw32-gcc
  export CXX=x86_64-w64-mingw32-g++
elif [ "$TARGET" == "MINGW32" ]; then
  CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=Windows "
  export CC=i686-w64-mingw32-gcc
  export CXX=i686-w64-mingw32-g++
fi

if [ -z "$INSTALLPREFIX" ]; then
  INSTALLPREFIX="/usr/local"
fi

# TODO: Fix this in a better way.
INCLUDE_FIX="-I $PWD/../src/llvm/projects/clang/include "
INCLUDE_FIX+="-I $PWD/projects/clang/include "

echo -n $INSTALLPREFIX > INSTALLPREFIX

cmake ../src/llvm \
 -DCMAKE_CXX_FLAGS="$INCLUDE_FIX" \
 -DLLVM_INCLUDE_TESTS=OFF \
 -DCMAKE_BUILD_TYPE=RELEASE \
 -DCMAKE_INSTALL_PREFIX=$INSTALLPREFIX \
 -DTAPI_REPOSITORY_STRING=$TAPI_REPOSITORY \
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

popd &>/dev/null
popd &>/dev/null
