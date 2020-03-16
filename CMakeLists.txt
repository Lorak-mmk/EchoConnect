cmake_minimum_required(VERSION 3.1.0)

project(EchoConnect VERSION 0)

SET(CC clang)
SET(CXX clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_subdirectory(libecho)
