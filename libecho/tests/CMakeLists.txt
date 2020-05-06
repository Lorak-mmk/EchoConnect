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
add_executable(sendrecv sendrecv.cpp)
add_executable(info info.cpp)
add_executable(wave wave.cpp)
add_executable(getlim getlim.cpp)
add_executable(alt alt.cpp)
add_executable(show show.cpp)
add_executable(dft dft.cpp)

target_link_libraries(${EXEC_NAME} echo gtest gtest_main)
target_link_libraries(sendrecv echo)

add_test(NAME ${EXEC_NAME} COMMAND ${EXEC_NAME})


target_link_libraries(${EXEC_NAME} echo ${GTEST_BOTH_LIBRARIES})
target_link_libraries(sendrecv echo)
target_link_libraries(info echo)
target_link_libraries(wave echo)
target_link_libraries(getlim echo)
target_link_libraries(alt echo)
target_link_libraries(show echo)
target_link_libraries(dft echo)
