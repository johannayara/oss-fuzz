#!/bin/bash -eu

echo "Building vulnerable libpng..."

cd libpng-1.6.18
make clean || true

# Determine if we’re building with LEAK_MEM and ASan
if [[ "${1:-}" == "leak_mem" ]]; then
    echo "-> Configuring without sanitizers"
    unset CFLAGS
    unset LDFLAGS
else
    echo "-> Configuring with AddressSanitizer"
    export CFLAGS="-fsanitize=address -g -O0"
    export LDFLAGS="-fsanitize=address"
fi

./configure --disable-shared --prefix=$(pwd)/build
make
make install
cd ..

echo "Building PoC..."

make clean

if [[ "${1:-}" == "leak_mem" ]]; then
    echo "-> Compiling PoC with -DLEAK_MEM "
    make LEAK_MEM=1
else
    echo "-> Compiling PoC normally with ASan"
    make 
fi

echo "Running PoC..."
if ! ./poc > out.txt 2>&1; then
    echo "PoC crashed (non-zero exit code), see out.txt"
fi

echo "Cleaning up..."
make clean
cd libpng-1.6.18 
make clean