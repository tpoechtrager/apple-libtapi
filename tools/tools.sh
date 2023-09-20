#!/usr/bin/env bash

PLATFORM=$(uname -s)
OPERATING_SYSTEM=$(uname -o 2>/dev/null || echo "-")

if [ -z "$INSTALLPREFIX" ]; then
  INSTALLPREFIX=/usr/local
fi

if command -v gmake > /dev/null; then
  MAKE=gmake
elif command -v make > /dev/null; then
  MAKE=make
else
  echo "'make' not found!" 1>&2
fi

if [ -z "$CC" ] && [ -z "$CXX" ]; then

command -v clang &>/dev/null && {
  export CC=clang
  export CXX=clang++
}

fi

if [ -z "$JOBS" ]; then
  JOBS=$(tools/cpucount/get_cpu_count.sh)
fi
