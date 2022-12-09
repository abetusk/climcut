#!/bin/bash
#
# To the extent possible under law, the person who associated CC0 with
# this source file has waived all copyright and related or neighboring rights
# to this source file.
#


MCUTLIBDIR="../mcut/build/bin"
MCUTINCDIR="../mcut/include"

eig3flag=`pkg-config --cflags eigen3`


g++ \
  -O3 \
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

