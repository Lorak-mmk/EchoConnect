set(EXEC_NAME libecho_test)
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

set(TEST_FILES
        main.cpp
        tmp_test.cpp)

add_executable(${EXEC_NAME} ${TEST_FILES})
add_test(NAME ${EXEC_NAME} COMMAND ${EXEC_NAME})

target_link_libraries(${EXEC_NAME} echo gtest)
