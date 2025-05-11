#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# First argument: directory name is required 
NAME="${1:-}"
if [ -z "$NAME" ]; then
  echo "Usage: $0 <name> [port]"
  exit 1
fi

# Second argument: port (optional, default to 8081)
PORT="${2:-8081}"

DIR="build/out/${NAME}"

if [ -f "$DIR" ]; then
  echo "Found directory: $DIR"
fi

# Build the fuzzer for libpng coverage
python3 infra/helper.py build_fuzzers --sanitizer coverage libpng

# Build the fuzzers for libpng
python3 infra/helper.py coverage libpng --corpus-dir "$DIR" --fuzz-target libpng_write_fuzzer --port "$PORT"




