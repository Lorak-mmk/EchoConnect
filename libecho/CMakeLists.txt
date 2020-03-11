cmake_minimum_required(VERSION 3.1.0)

project(libecho VERSION 0.1)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Multimedia REQUIRED)

set(SRC_DIR "src" CACHE PATH "Source files path")
set(API_DIR  "include" CACHE PATH "Public API files path")

set(SRC_FILES
    ${SRC_DIR}/main.cpp
    tests/main.cpp
   )
    
set(API_FILES
    ${API_DIR}/echo.h
   )

add_library(echo SHARED "${SRC_FILES}")

include_directories(echo PRIVATE ${SRC_DIR})
include_directories(echo PRIVATE ${API_DIR})

set_target_properties(echo
                        PROPERTIES 
                            VERSION ${PROJECT_VERSION} SOVERSION 0
                            PUBLIC_HEADER "${API_FILES}" 
                     )

target_link_libraries(echo Qt5::Multimedia)

add_subdirectory(tests)
