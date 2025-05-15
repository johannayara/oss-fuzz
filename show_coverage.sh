#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# First argument: directory name is required 
DIR="${1:-"build/out/merged_new_read_fuzzer"}"
if [ -z "$DIR" ]; then
  echo "Usage: $0 <dir>"
  exit 1
fi

DIR="build/out/${NAME}"

if [ -f "$DIR" ]; then
  echo "Found directory: $DIR"
fi

# Build the fuzzer for libpng coverage
python3 infra/helper.py build_fuzzers --sanitizer coverage libpng

# Build the fuzzers for libpng
python3 infra/helper.py coverage libpng --corpus-dir "$DIR" --fuzz-target new_libpng_read_fuzzer
