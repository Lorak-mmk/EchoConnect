cmake_minimum_required(VERSION 3.1.0)

project(libecho)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Multimedia REQUIRED)

set (SRC_DIR "src" CACHE PATH "Source files path")
set (H_DIR  "include" CACHE PATH "Public API files path")

set (C_FILES 
    ${SRC_DIR}/main.cpp
    )
    
set (H_FILES
    ${H_DIR}/echo.h
    )

add_library(echo SHARED "${C_FILES}")

include_directories (${H_DIR})

set_target_properties (echo
                        PROPERTIES 
                            VERSION 0.1 SOVERSION 0
                            PUBLIC_HEADER "${H_FILES}" 
                      )

target_link_libraries(echo Qt5::Multimedia)
