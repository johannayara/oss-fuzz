#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

NAME="${1:-}"
DIR="build/out/${NAME}w_corpus"

echo "Created directory: $DIR"

# Set value of which docker command to run 
export SCRIPT_CALLER=w_corpus

# Build the Docker image for libpng
python3 infra/helper.py build_image libpng

# Build the fuzzers for libpng
python3 infra/helper.py build_fuzzers libpng

# Create the output corpus directory
#TODO : think about maybe emptying the directory if it already exists 
mkdir -p "$DIR"

# Run the fuzzer
python3 infra/helper.py run_fuzzer libpng libpng_read_fuzzer --corpus-dir "$DIR"
