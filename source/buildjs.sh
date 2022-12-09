#!/bin/bash

emscoredir=../emsdk/upstream/emscripten/cache/sysroot/include
emsdkdir=../emsdk

eig3flag=`pkg-config --cflags eigen3`

pushd $emsdkdir
source ./emsdk_env.sh
popd

emcc \
  -DCC_EMSCRIPTEN \
  -lembind \
  -I$emscoredir \
  -I$emsdkdir \
  $eig3flag \
  -I../libigl/include \
  -L../mcut/build/bin \
  -I../mcut/include \
  climcut.cpp \
  ../mcut/source/*.cpp \
  ../mcut/source/*.c \
  -o ../bin/mcut.js \
  -sEXPORTED_FUNCTIONS=_mcutop,_climcut_hello \
  -sEXPORTED_RUNTIME_METHODS=ccal,cwrap


#  -I$HOME/lib/include \
#  -I/usr/include/eigen3 \
#  -I/usr/include/eigen3 \
#  -I/usr/include/eigen3 \
#  -sFULL_ES3=1 \
#  -sWASM=1 \



