#!/usr/bin/env bash

set -e

pushd "${0%/*}" &>/dev/null
source tools/tools.sh
pushd build &>/dev/null

INSTALLPREFIX=$(cat INSTALLPREFIX)

INSTALLTARGETS="install-libtapi install-tapi-headers"

[ -e "bin/tapi" ] && INSTALLTARGETS+=" install-tapi"
[ -e "bin/tapi-binary-reader" ] && INSTALLTARGETS+=" install-tapi-binary-reader"
[ -e "bin/tapi-run" ] && INSTALLTARGETS+=" install-tapi-run"
[ -e "bin/tapi-sdkdb" ] && INSTALLTARGETS+=" install-tapi-sdkdb"

if [ -f build.ninja ]; then
    command -v ninja &>/dev/null || {
        echo "Missing ninja" 1>&2
        exit 1
    }
    MAKE=ninja
fi

$MAKE $INSTALLTARGETS -j $JOBS

popd &>/dev/null
popd &>/dev/null
