set(EXEC_NAME RawDemo)

set(SRC_FILES main.cc)

add_executable(${EXEC_NAME} ${SRC_FILES})

target_link_libraries(${EXEC_NAME} PRIVATE echo)
