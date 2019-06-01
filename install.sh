#!/usr/bin/env bash

set -e

pushd "${0%/*}" &>/dev/null
source tools/tools.sh
pushd build &>/dev/null

INSTALLPREFIX=$(cat INSTALLPREFIX)

$MAKE install-libtapi install-tapi-headers -j $JOBS

popd &>/dev/null
popd &>/dev/null
