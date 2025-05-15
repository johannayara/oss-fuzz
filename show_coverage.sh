#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# First argument: directory name is optional 
DIR="${1:-"build/out/merged_w_corpus"}"

if [ -f "$DIR" ]; then
  echo "Found directory: $DIR"
fi

# Build the fuzzer for libpng coverage
python3 infra/helper.py build_fuzzers --sanitizer coverage libpng

# Build the fuzzers for libpng
python3 infra/helper.py coverage libpng --corpus-dir "$DIR" --fuzz-target libpng_read_fuzzer




