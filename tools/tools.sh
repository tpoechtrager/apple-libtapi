#!/usr/bin/env bash

PLATFORM=$(uname -s)

if [ -z "$INSTALLPREFIX" ]; then
  INSTALLPREFIX=/usr/local
fi

if [[ $PLATFORM == *BSD ]] || [ $PLATFORM == "DragonFly" ]; then
  MAKE=gmake
else
  MAKE=make
fi

if [ -z "$CC" ] && [ -z "$CXX" ]; then

which clang &>/dev/null && {
  export CC=clang
  export CXX=clang++
}

fi

if [ -z "$JOBS" ]; then
  JOBS=$(tools/cpucount/get_cpu_count.sh)
fi
