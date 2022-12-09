#!/bin/bash
#
# To the extent possible under law, the person who associated CC0 with
# this source file has waived all copyright and related or neighboring rights
# to this source file.
#


emscoredir=../emsdk/upstream/emscripten/cache/sysroot/include
emsdkdir=../emsdk

eig3flag=`pkg-config --cflags eigen3`

pushd $emsdkdir
source ./emsdk_env.sh
popd

# thanks to https://qiita.com/ukyo/items/51cd6e43bd39df27ac76
# for the --post-js trick
#
emcc \
  -sASSERTIONS \
  -sALLOW_MEMORY_GROWTH \
  --post-js ./mcut_post.js \
  -sWASM=1 \
  -sSINGLE_FILE \
  -O3 \
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
  -sEXPORTED_RUNTIME_METHODS=ccall,cwrap

browserify mcut_wrapper.js -o ../bin/mcut_web.js

#  -I$HOME/lib/include \
#  -I/usr/include/eigen3 \
#  -I/usr/include/eigen3 \
#  -I/usr/include/eigen3 \
#  -sFULL_ES3=1 \
#  -sWASM=1 \


#browserify


