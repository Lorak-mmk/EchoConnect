cmake_minimum_required(VERSION 3.11.0)

SET(CMAKE_CXX_STANDARD 17)
SET(BUILD_SHARED_LIBS ON)

project(EchoConnect VERSION 1)

if (MSVC)
    add_compile_options(/W3)
else()
    add_compile_options(-Wall -Wextra -fno-strict-aliasing)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_subdirectory(libecho)
