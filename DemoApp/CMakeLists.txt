project(DemoApp VERSION 0.1)

set(EXEC_NAME DemoApp)

include(FetchContent)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(SRC_DIR "src" CACHE PATH "Source files path")
set(SRC_FILES
        ${SRC_DIR}/Argument.h
        ${SRC_DIR}/AAction.cc
        ${SRC_DIR}/AAction.h
        ${SRC_DIR}/AView.h
        ${SRC_DIR}/AView.cc
        ${SRC_DIR}/Menu.h
        ${SRC_DIR}/Menu.cc
        ${SRC_DIR}/Utils.h
        ${SRC_DIR}/Utils.cc
        ${SRC_DIR}/Help.h
        ${SRC_DIR}/Help.cc
        ${SRC_DIR}/SendText.h
        ${SRC_DIR}/SendText.cc
        ${SRC_DIR}/Config.cc
        ${SRC_DIR}/SendFile.cc
        ${SRC_DIR}/SendFile.h
        ${SRC_DIR}/Console.h
        ${SRC_DIR}/Console.cc
        ${SRC_DIR}/main.cc)

add_executable(${EXEC_NAME} "${SRC_FILES}")
set_target_properties(${EXEC_NAME}
        PROPERTIES
        VERSION ${PROJECT_VERSION})

#target_precompile_headers(${EXEC_NAME} PRIVATE ${SRC_DIR}/Config.h)

target_include_directories(${EXEC_NAME} PRIVATE ${SRC_DIR})
target_include_directories(${EXEC_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libecho/include)

target_link_libraries(${EXEC_NAME} PRIVATE echo)
target_link_libraries(${EXEC_NAME} PRIVATE nlohmann_json::nlohmann_json)
