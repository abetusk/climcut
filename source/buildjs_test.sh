#!/bin/bash


emscoredir=$HOME/git/github/emscripten-core/emscripten/system/include
emsdkdir=$HOME/git/github/emscripten-core/emsdk

pushd $emsdkdir
source ./emsdk_env.sh
popd

emcc \
  -lembind \
  -I$emscoredir \
  -I$emsdkdir \
  exptr.cpp \
  -I$HOME/lib/include \
  -sEXPORTED_FUNCTIONS=_main,_funco,_funcu \
  -sEXPORTED_RUNTIME_METHODS=ccall,cwrap \
  -sFULL_ES3=1 \
  -sWASM=1

# -sEXPORTED_FUNCTIONS=_main,_other_function


