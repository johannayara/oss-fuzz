#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# First argument: directory name is optional 
#DIR="${1:-"build/out/new_read_corpus_4hours"}"
DIR="build/out/${1:-new_read_corpus_4hours}"

if [ -z "$DIR" ]; then
  echo "Usage: $0 [dir]"
  exit 1
fi

echo "Created directory: $DIR"

export OSSFUZZ_PULL=true
# Build the Docker image for libpng
python3 ../../infra/helper.py build_image libpng

# Build the fuzzers for libpng
python3 ../../infra/helper.py build_fuzzers libpng

# Create the output corpus directory
mkdir -p "../../${DIR}"

# Run the fuzzer
python3 ../../infra/helper.py run_fuzzer libpng new_libpng_read_fuzzer --corpus-dir "$DIR"