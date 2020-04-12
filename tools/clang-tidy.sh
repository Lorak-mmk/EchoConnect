#!/bin/bash

# https://stackoverflow.com/a/16349776
cd "${0%/*}" || exit

CHECK_DIRS=("../libecho/src" "../libecho/include")
CHECKS="-*,bugprone-*,cert-*,cppcoreguidelines-*,clang-analyzer-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-modernize-use-trailing-return-type,-cppcoreguidelines-owning-memory,-cppcoreguidelines-non-private-member-variables-in-classes,-misc-non-private-member-variables-in-classes"

find "${CHECK_DIRS[@]}" -type f \( -name '*\.cpp' -o -name '*\.cc' \) -print0 |  \
xargs --null clang-tidy -header-filter='.*' -checks="$CHECKS" -p=../build -quiet --warnings-as-errors="$CHECKS"
