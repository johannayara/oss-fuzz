#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Remove existing seed corpus if it exists
SEED_CORPUS="build/out/libpng/libpng_read_fuzzer_seed_corpus.zip"
if [ -f "$SEED_CORPUS" ]; then
  echo "Removing existing seed corpus: $SEED_CORPUS"
  rm "$SEED_CORPUS"
fi

# Apply the diff file
DIFF_FILE="comment_seeds.diff"
if [ -f "$DIFF_FILE" ]; then
  echo "Applying patch from $DIFF_FILE..."
  if git apply --check "$DIFF_FILE"; then
    git apply "$DIFF_FILE"
    echo "Patch applied successfully."
  else
    echo "Patch could not be applied. It may already be applied. Skipping."
  fi
else
  echo "Diff file $DIFF_FILE not found! Skipping patch."
fi


NAME="${1:-}"
DIR="build/out/${NAME}w_o_corpus"

echo "Created directory: $DIR"

# Set value of which docker command to run 
export SCRIPT_CALLER=w_o_corpus
echo "script caller : $SCRIPT_CALLER"

# Build the Docker image for libpng
python3 infra/helper.py build_image libpng

# Build the fuzzers for libpng
python3 infra/helper.py build_fuzzers libpng

# Create the output corpus directory
mkdir -p "$DIR"

# Run the fuzzer
python3 infra/helper.py run_fuzzer libpng libpng_read_fuzzer --corpus-dir "$DIR"
