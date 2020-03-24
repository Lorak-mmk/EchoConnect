#!/bin/bash

# https://stackoverflow.com/a/16349776
cd "${0%/*}"

./run-clang-tidy.py \
  -p ../build \
  -checks="-*,bugprone-*,cert-*,cppcoreguidelines-*,clang-analyzer-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-modernize-use-trailing-return-type,-cppcoreguidelines-owning-memory" \
  -quiet \
