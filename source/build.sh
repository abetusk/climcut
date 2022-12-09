#!/bin/bash

MCUTLIBDIR="../mcut/build/bin"
MCUTINCDIR="../mcut/include"

eig3flag=`pkg-config --cflags eigen3`


g++ \
  -L$MCUTLIBDIR \
  -I$MCUTINCDIR \
  $eig3flag \
  -I../libigl/include \
  climcut.cpp \
  -o ../bin/climcut \
  -lmcut

#  -L../bin \
#  -I../../include \
#  -I/usr/include/eigen3 \
#  -I$HOME/lib/include \
#  -I/usr/include/eigen3
#  -I/usr/include/eigen3 \

