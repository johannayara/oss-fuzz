#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Create directory with provided name 
# NAME="${1:-}"
DIR="../../build/out/new_read_corpus_4hours"

echo "Created directory: $DIR"

export OSSFUZZ_PULL=true
# Build the Docker image for libpng
python3 ../../infra/helper.py build_image libpng

# Build the fuzzers for libpng
python3 ../../infra/helper.py build_fuzzers libpng

# Create the output corpus directory
#TODO : think about maybe emptying the directory if it already exists 
mkdir -p "$DIR"

# Run the fuzzer
python3 ../../infra/helper.py run_fuzzer libpng new_libpng_read_fuzzer --corpus-dir "$DIR"