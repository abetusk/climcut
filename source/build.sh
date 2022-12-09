#!/bin/bash

MCUTLIBDIR="../mcut/build/bin"
MCUTINCDIR="../mcut/include"

g++ \
  -I/usr/include/eigen3 \
  -L$MCUTLIBDIR \
  -I$HOME/lib/include \
  -I/usr/include/eigen3 \
  -I$MCUTINCDIR \
  climcut.cpp \
  -o ../bin/climcut \
  -lmcut \
  -I/usr/include/eigen3

#  -L../bin \
#  -I../../include \

