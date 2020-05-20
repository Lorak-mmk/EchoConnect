#!/bin/bash

# https://stackoverflow.com/a/16349776
cd "${0%/*}" || exit

CHECK_DIRS=("../libecho/src" "../libecho/include" "../DemoApp/src")
CHECKS_ENABLED="bugprone-*,cppcoreguidelines-*,clang-analyzer-*,misc-*,modernize-*,performance-*,portability-*,readability-*"
CHECKS_DISABLED="-modernize-use-trailing-return-type,-cppcoreguidelines-owning-memory,-cppcoreguidelines-non-private-member-variables-in-classes,-misc-non-private-member-variables-in-classes,-cppcoreguidelines-pro-bounds-pointer-arithmetic,-cppcoreguidelines-pro-type-union-access,-cppcoreguidelines-pro-type-vararg,-cppcoreguidelines-pro-bounds-array-to-pointer-decay,-modernize-avoid-c-arrays,-cppcoreguidelines-avoid-c-arrays,-cppcoreguidelines-pro-type-reinterpret-cast,-readability-isolate-declaration,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers,-readability-implicit-bool-conversion"
CHECKS="${CHECKS_ENABLED},${CHECKS_DISABLED}"
HEADERS="libecho\/src|libecho\/include"

if hash clang-tidy-9 2>/dev/null; then
    CLANG_TIDY="clang-tidy-9"
else
    CLANG_TIDY="clang-tidy"
fi



find "${CHECK_DIRS[@]}" -type f \( -name '*\.cpp' -o -name '*\.cc' \) -print0 |  \
xargs --null $CLANG_TIDY -header-filter="$HEADERS" -checks="$CHECKS" -p=../build -quiet --warnings-as-errors="$CHECKS"
