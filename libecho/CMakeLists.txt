project(libecho VERSION 0.1)


find_package(Qt5 COMPONENTS Multimedia REQUIRED)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif ()


set(SRC_DIR "src" CACHE PATH "Source files path")
set(API_DIR "include" CACHE PATH "Public API files path")
set(SRC_FILES
        ${SRC_DIR}/echo.cpp
        ${SRC_DIR}/AudioOutput.cc
        ${SRC_DIR}/AudioOutput.h
        ${SRC_DIR}/AudioInput.cc
        ${SRC_DIR}/AudioInput.h
        ${SRC_DIR}/AudioFormatFactory.cc
        ${SRC_DIR}/AudioFormatFactory.h
        ${SRC_DIR}/QTInitializer.cc)

set(API_FILES
        ${API_DIR}/echo.h)


include_directories(echo PRIVATE ${API_DIR})


add_library(echo SHARED "${SRC_FILES}")
set_target_properties(echo
        PROPERTIES
        VERSION ${PROJECT_VERSION} SOVERSION 0
        PUBLIC_HEADER "${API_FILES}")

target_link_libraries(echo Qt5::Multimedia)


add_subdirectory(tests)
add_subdirectory(docs)
