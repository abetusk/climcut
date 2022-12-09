#!/bin/bash

emscoredir=$HOME/git/github/emscripten-core/emscripten/system/include
emsdkdir=$HOME/git/github/emscripten-core/emsdk

pushd $emsdkdir
source ./emsdk_env.sh
popd

emcc \
  -I/usr/include/eigen3 \
  -L../bin \
  -I$HOME/lib/include \
  -I/usr/include/eigen3 \
  -I../../include \
  climcut.cpp \
  ../../source/*.cpp \
  ../../source/*.c \
  -o climcut.js \
  -I/usr/include/eigen3 \
  -sEXPORTED_FUNCTIONS=_mcutop \
  -sEXPORTED_RUNTIME_METHODS=ccal,cwrap



