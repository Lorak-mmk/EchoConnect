set(EXEC_NAME libecho_test)


set(TEST_FILES
        main.cpp
        tmp_test.cpp
   )

find_package(echo REQUIRED)

add_executable(${EXEC_NAME} ${TEST_FILES})
add_test(NAME ${EXEC_NAME} COMMAND ${EXEC_NAME})

target_link_libraries(${BINARY} PUBLIC echo)
