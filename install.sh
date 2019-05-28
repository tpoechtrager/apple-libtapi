#!/usr/bin/env bash

set -e

pushd "${0%/*}" &>/dev/null
source tools/tools.sh
pushd build &>/dev/null

INSTALLPREFIX=\
\ $(cat CMakeCache.txt | grep CMAKE_INSTALL_PREFIX:PATH |\
  head -n1 | tr '=' '\n' | tail -n1)

mkdir -p $INSTALLPREFIX
mkdir -p $INSTALLPREFIX/include
cp -rva ../src/llvm/projects/libtapi/include/tapi $INSTALLPREFIX/include
cp -va projects/libtapi/include/tapi/Version.inc $INSTALLPREFIX/include/tapi

$MAKE install-libtapi -j $JOBS

popd &>/dev/null
popd &>/dev/null
