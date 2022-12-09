#!/bin/bash
#
# LICENSE: cc0

git submodule update --init --recursive
mkdir -p mcut/build
cp patch/include/mcut/internal/utils.h mcut/include/mcut/internal/utils.h
cp patch/source/kernel.cpp mcut/source/kernel.cpp

pushd mcut/build
cmake ..
make
popd

pushd source
./build.sh
./buildjs.sh
popd
