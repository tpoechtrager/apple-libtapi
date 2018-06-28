#!/usr/bin/env bash

set -e

pushd "${0%/*}" &>/dev/null
source tools/tools.sh

rm -rf build
mkdir build

pushd build &>/dev/null

CMAKE_EXTRA_ARGS=""

if [ "$TARGET" == "Darwin" ]; then
  export MACOSX_DEPLOYMENT_TARGET=10.9
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
  export CC=amd64-pc-freebsd10.1-clang
  export CXX=amd64-pc-freebsd10.1-clang++
elif [ "$TARGET" == "MINGW64" ]; then
  CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=Windows "
  export CC=x86_64-w64-mingw32-gcc
  export CXX=x86_64-w64-mingw32-g++
elif [ "$TARGET" == "MINGW32" ]; then
  CMAKE_EXTRA_ARGS+="-DCMAKE_SYSTEM_NAME=Windows "
  export CC=i686-w64-mingw32-gcc
  export CXX=i686-w64-mingw32-g++
fi

if [ -n "$TARGET" -a "$PLATFORM" != "$TARGET" ]; then
  VERSION=$(cat ../VERSION.txt | grep llvm: | tr ' ' '\n' | tail -n1)
  CMAKE_EXTRA_ARGS+="-DLLVM_TABLEGEN=/opt/compiler/apple-llvm/$VERSION/bin/llvm-tblgen"
fi

cmake ../src/llvm \
 -DLLVM_INCLUDE_TESTS=OFF \
 -DCMAKE_BUILD_TYPE=RELEASE \
 -DCMAKE_INSTALL_PREFIX=$INSTALLPREFIX \
 $CMAKE_EXTRA_ARGS
$MAKE libtapi -j $JOBS

popd &>/dev/null
popd &>/dev/null
