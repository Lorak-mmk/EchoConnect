set(EXEC_NAME libecho_test)
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_FILES
        main.cpp
        test_Packet.cpp
        test_EchoRawConnection.cpp)

add_executable(${EXEC_NAME} ${TEST_FILES})
add_test(NAME ${EXEC_NAME} COMMAND ${EXEC_NAME})

target_link_libraries(${EXEC_NAME} echo ${GTEST_BOTH_LIBRARIES})
