
#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

mkdir -p build/out/w_new_corpus
if [ -z "$(ls -A build/out/w_new_corpus)" ]; then
    cp -a build/out/libpng/test_corpus/. build/out/w_new_corpus
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


# Build the Docker image for libpng
python3 infra/helper.py build_image libpng

# Build the fuzzers for libpng
python3 infra/helper.py build_fuzzers libpng

# Run the fuzzer
python3 infra/helper.py run_fuzzer libpng libpng_read_fuzzer --corpus-dir build/out/w_new_corpus
