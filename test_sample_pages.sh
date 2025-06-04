#!/bin/bash
set -e

# Compile the validator

g++ -std=c++20 -o validator main.cpp HTMLValidator.cpp

# Run the validator against each sample page
fail=0
for file in sample-pages/*.html; do
    base=$(basename "$file")
    if [[ $base == valid-* ]]; then
        expected="valid"
    else
        expected="invalid"
    fi
    output=$(./validator "$file")
    if [[ "$output" != "$expected" ]]; then
        echo "Mismatch for $file: expected $expected but got $output"
        fail=1
    else
        echo "$file: $output"
    fi
done

exit $fail
