project(libecho VERSION 0.1)


find_package(Qt5 COMPONENTS Multimedia REQUIRED)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif ()

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DQT_NO_DEBUG_OUTPUT -DQT_NO_INFO_OUTPUT -g")


set(SRC_DIR "src" CACHE PATH "Source files path")
set(API_DIR "include" CACHE PATH "Public API files path")
set(SRC_FILES
        ${SRC_DIR}/Echo.cc
        ${SRC_DIR}/EchoRawConnection.cc
        ${SRC_DIR}/EchoRawConnectionImpl.h
        ${SRC_DIR}/EchoRawConnectionImpl.cc
	${SRC_DIR}/EchoProtocol.h
	${SRC_DIR}/EchoProtocol.cc
        ${SRC_DIR}/AudioOutput.cc
        ${SRC_DIR}/AudioOutput.h
        ${SRC_DIR}/AudioInput.cc
        ${SRC_DIR}/AudioInput.h
        ${SRC_DIR}/QTInitializer.cc
        ${SRC_DIR}/AudioStream.h
        ${SRC_DIR}/ISender.h
        ${SRC_DIR}/BitSender.h
        ${SRC_DIR}/BitSender.cc
        ${SRC_DIR}/BitSenderv2.h
        ${SRC_DIR}/BitSenderv2.cc
        ${SRC_DIR}/IReceiver.h
        ${SRC_DIR}/BitReceiver.cc
        ${SRC_DIR}/BitReceiver.h
        ${SRC_DIR}/BitReceiverv2.cc
        ${SRC_DIR}/BitReceiverv2.h
        ${SRC_DIR}/Packet.h
        ${SRC_DIR}/Packet.cc
        ${SRC_DIR}/HammingCode.h
        ${SRC_DIR}/HammingCode.cc
        ${SRC_DIR}/Typename.h)

set(API_FILES
        ${API_DIR}/Echo.h
        ${API_DIR}/EchoRawConnection.h)

include_directories(${API_DIR})

add_library(echo SHARED "${SRC_FILES}")
set_target_properties(echo
        PROPERTIES
        VERSION ${PROJECT_VERSION} SOVERSION 0
        PUBLIC_HEADER "${API_FILES}")

target_include_directories(echo PRIVATE ${SRC_DIR})
target_include_directories(echo PRIVATE external/CRCpp/inc)

target_link_libraries(echo Qt5::Multimedia)


add_subdirectory(tests)
add_subdirectory(docs)
add_subdirectory(examples)
