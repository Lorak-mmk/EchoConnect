set(EXEC_NAME libecho_test)
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

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
add_executable(self_cal self_cal.cpp)

add_test(NAME ${EXEC_NAME} COMMAND ${EXEC_NAME})

target_link_libraries(${EXEC_NAME} echo ${GTEST_BOTH_LIBRARIES})
target_link_libraries(sendrecv echo)
target_link_libraries(info echo)
target_link_libraries(wave echo)
target_link_libraries(getlim echo)
target_link_libraries(alt echo)
target_link_libraries(show echo)
target_link_libraries(dft echo)
target_link_libraries(self_cal echo)
