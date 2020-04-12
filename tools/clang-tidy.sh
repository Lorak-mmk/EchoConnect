#!/bin/bash

# https://stackoverflow.com/a/16349776
cd "${0%/*}" || exit

CHECK_DIRS=("../libecho/src" "../libecho/include")
CHECKS="-*,bugprone-*,cert-*,cppcoreguidelines-*,clang-analyzer-*,misc-*,modernize-*,performance-*,portability-*,readability-*,-modernize-use-trailing-return-type,-cppcoreguidelines-owning-memory,-cppcoreguidelines-non-private-member-variables-in-classes,-misc-non-private-member-variables-in-classes"

if hash clang-tidy-9 2>/dev/null; then
    CLANG_TIDY="clang-tidy-9"
else
    CLANG_TIDY="clang-tidy"
fi



find "${CHECK_DIRS[@]}" -type f \( -name '*\.cpp' -o -name '*\.cc' \) -print0 |  \
xargs --null $CLANG_TIDY -header-filter='.*' -checks="$CHECKS" -p=../build -quiet --warnings-as-errors="$CHECKS"
