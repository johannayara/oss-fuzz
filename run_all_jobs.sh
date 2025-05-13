#!/bin/bash

# Exit immediately if a command exits with a non-zero status
# set -e

RUNS=3

export OSSFUZZ_PULL=true

# Build the Docker image for libpng
python3 infra/helper.py build_image libpng

# Build the fuzzers for libpng
python3 infra/helper.py build_fuzzers libpng

for i in $(seq 1 $RUNS); do
  OUT_DIR="build/out/write_fuzzer_run${i}"
  echo "=== Fuzz run #${i}: output → ${OUT_DIR} ==="
  mkdir -p "$OUT_DIR"
  python3 infra/helper.py run_fuzzer \
    --corpus-dir "$OUT_DIR" \
    libpng libpng_write_fuzzer
done

echo "All ${RUNS} fuzz runs complete."