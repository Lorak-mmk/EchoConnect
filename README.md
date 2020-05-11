![Linux](https://github.com/Lorak-mmk/EchoConnect/workflows/Linux/badge.svg?branch=master)
![Windows](https://github.com/Lorak-mmk/EchoConnect/workflows/Windows/badge.svg?branch=master)
![MacOS](https://github.com/Lorak-mmk/EchoConnect/workflows/MacOS/badge.svg?branch=master)
![Linting](https://github.com/Lorak-mmk/EchoConnect/workflows/Linting/badge.svg?branch=master)


# EchoConnect

The project will ultimately enable easy communication between computers that do not require any additional services (Bluetooth / WiFi), using high-frequency sounds. We will create a documented transmission protocol that supports error correction and transmission validation. On its basis, we are going to build a mechanism that allows you to transfer files or text between computers. The whole will be wrapped in API made available in the form of shared libraries (.so / .dll). In addition, we've created sample applications (CLI / QT) that use this API to present its capabilities.

### Use cases:
-   People with defective/poor computer set
-   Hobbyists and curious people

### Functionalities:
-   Data transmission by sound
-   Error correction, verification of correctness
-   API
-   CLI + GUI app using the API

### Technologies used:
-   Language: C++
-   Audio support, GUI: QT
-   Tests: GoogleTest
-   Documentation: Doxygen
-   Linter: clang-tidy
-   Formatter: clang-format

### Tools:
-   Repository: Github
-   Issue tracker: Github Issues
-   Project management: Github Projets
-   Continuous Integration: Github Actions

## Documentation:
To build on your own (instructions below) or for the newest commit on master at:
https://codedocs.xyz/Lorak-mmk/EchoConnect/index.html

## Build:
```shell-script
git clone --recurse-submodules https://github.com/Lorak-mmk/EchoConnect.git
cd EchoConnect
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make

# Tests
GTEST_COLOR=1 make test CTEST_OUTPUT_ON_FAILURE=TRUE
```

Output files:

Library: `build/libecho/*.so*`

Tests: `build/libecho/tests/libecho_tests`


**Automatic documentation build requires Release build mode, i.e. cmake -DCMAKE_BUILD_TYPE=Release**


Manual documentation build (in build/):
```shell-script
make doc_doxygen
```

Documentation files: `build/libecho/docs/doc_doxygen`

### Required libraries:
- QT
