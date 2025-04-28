#!/bin/sh
cd /src/aflplusplus
apt-get install -y ssh gdb joe
git pull
git checkout dev
unset CC
unset CXX
unset CFLAGS
unset CXXFLAGS
make clean install
