set(EXEC_NAME libecho_test)
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

set(TEST_FILES
        main.cpp
        packet_tests.cpp)

add_executable(${EXEC_NAME} ${TEST_FILES})
add_executable(send send.cpp)

add_test(NAME ${EXEC_NAME} COMMAND ${EXEC_NAME})

target_link_libraries(${EXEC_NAME} echo gtest)
target_link_libraries(send echo gtest)
