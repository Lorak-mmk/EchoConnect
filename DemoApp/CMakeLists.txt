project(DemoApp VERSION 0.1)

set(EXEC_NAME DemoApp)

find_package(Qt5 COMPONENTS Multimedia REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif ()


if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif ()

set(SRC_DIR "src" CACHE PATH "Source files path")
set(SRC_FILES
        ${SRC_DIR}/Argument.h
        ${SRC_DIR}/IAction.cc
        ${SRC_DIR}/IAction.h
        ${SRC_DIR}/IView.h
        ${SRC_DIR}/IView.cc
        ${SRC_DIR}/Menu.h
        ${SRC_DIR}/Menu.cc
        ${SRC_DIR}/Utils.h
        ${SRC_DIR}/Utils.cc
        ${SRC_DIR}/main.cc)

add_executable(${EXEC_NAME} "${SRC_FILES}")
set_target_properties(echo
        PROPERTIES
        VERSION ${PROJECT_VERSION})

target_include_directories(${EXEC_NAME} PRIVATE ${SRC_DIR})
target_include_directories(${EXEC_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libecho/include)

target_link_libraries(${EXEC_NAME} PRIVATE echo)
