set(EXEC_NAME libecho_test)
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

set(GTEST_VERSION 1.10.0 CACHE STRING "Google test version")
################################
# GTest
################################
find_package(GTest)
if(GTest_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})
else()
    set(gtest_force_shared_crt 1)
    include(FetchContent)
    FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-${GTEST_VERSION})

    FetchContent_GetProperties(googletest)
    if(NOT googletest_POPULATED)
        FetchContent_Populate(googletest)
        add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
    endif()

    FetchContent_MakeAvailable(googletest)
endif()

set(TEST_FILES
        main.cc
        packet_tests.cc
        ecc_tests.cc)

include_directories("../src")
include_directories("../external/CRCpp/inc")

add_executable(${EXEC_NAME} ${TEST_FILES})
add_test(NAME ${EXEC_NAME} COMMAND ${EXEC_NAME})

target_link_libraries(${EXEC_NAME} PRIVATE echo gtest gtest_main)
