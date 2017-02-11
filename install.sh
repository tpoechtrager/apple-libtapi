#/usr/bin/env bash

pushd "${0%/*}" &>/dev/null

if [ ! -d build ]; then
  ./build.sh
fi

if [ -z "$INSTALLPREFIX" ]; then
  INSTALLPREFIX=/usr/local
fi

set -x

mkdir -p $INSTALLPREFIX/lib
mkdir -p $INSTALLPREFIX/include

cp build/lib/libtapi.so $INSTALLPREFIX/lib
cp -r src/apple-llvm/src/projects/libtapi/include/tapi $INSTALLPREFIX/include
cp build/projects/libtapi/include/tapi/Version.inc $INSTALLPREFIX/include/tapi

popd &>/dev/null
