#!/usr/bin/env bash

set -e

pushd "${0%/*}" &>/dev/null
source tools/tools.sh

rm -rf build
mkdir build

pushd build &>/dev/null

cmake ../src/apple-llvm/src \
 -DLLVM_INCLUDE_TESTS=OFF \
 -DCMAKE_BUILD_TYPE=RELEASE \
 -DCMAKE_INSTALL_PREFIX=$INSTALLPREFIX
$MAKE libtapi -j $JOBS

popd &>/dev/null
popd &>/dev/null
