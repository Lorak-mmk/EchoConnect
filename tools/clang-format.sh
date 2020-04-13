#!/bin/bash

# https://stackoverflow.com/a/16349776
cd "${0%/*}"

FILES=$(find ../libecho -type f \( -name '*\.cpp' -o -name '*\.cc' -o -name '*\.h' \) -not -path "../libecho/external/*" )

if hash clang-format-8 2>/dev/null; then
    CLANG_FORMAT="clang-format-8"
else
    CLANG_FORMAT="clang-format"
fi

$CLANG_FORMAT -style=file -i $FILES
