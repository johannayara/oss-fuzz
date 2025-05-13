#!/bin/bash -eu

cd libpng-1.6.18
./configure --disable-shared --prefix=$(pwd)/build
make
make install

# cd .. 
# make 