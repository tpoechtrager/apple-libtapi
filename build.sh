#!/usr/bin/env bash

pushd "${0%/*}" &>/dev/null

if [ -z "$CC" ] && [ -z "$CXX" ]; then

which clang &>/dev/null && {
  export CC=clang
  export CXX=clang++
}

fi

if [ -z "$JOBS" ]; then
  JOBS=$(tools/cpucount/get_cpu_count.sh)
fi

rm -rf build
mkdir build

pushd build &>/dev/null

cmake ../src/apple-llvm/src \
 -DLLVM_INCLUDE_TESTS=OFF \
 -DCMAKE_BUILD_TYPE=RELEASE
make libtapi -j $JOBS

popd &>/dev/null
popd &>/dev/null
