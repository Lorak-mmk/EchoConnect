project(libecho VERSION 0.1)


find_package(Qt5 COMPONENTS Multimedia REQUIRED)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
if (CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif ()


set(SRC_DIR "src" CACHE PATH "Source files path")
set(API_DIR "include" CACHE PATH "Public API files path")
set(SRC_FILES
        ${SRC_DIR}/EchoRawConnection.cc
        ${SRC_DIR}/EchoRawConnectionImpl.h
        ${SRC_DIR}/EchoRawConnection.cc
        ${SRC_DIR}/AudioOutput.cc
        ${SRC_DIR}/AudioOutput.h
        ${SRC_DIR}/AudioInput.cc
        ${SRC_DIR}/AudioInput.h
        ${SRC_DIR}/QTInitializer.cc
        ${SRC_DIR}/AudioStream.h
        ${SRC_DIR}/ISender.h
        ${SRC_DIR}/BitSender.h
        ${SRC_DIR}/BitSender.cc
        ${SRC_DIR}/IReceiver.h
        ${SRC_DIR}/BitReceiver.cc
        ${SRC_DIR}/BitReceiver.h
        ${SRC_DIR}/Packet.h
        ${SRC_DIR}/Packet.cc)

set(API_FILES
        ${API_DIR}/Echo.h
        ${API_DIR}/EchoRawConnection.h)

include_directories(echo PRIVATE ${API_DIR})
include_directories(echo PRIVATE ${SRC_DIR})
include_directories(echo PRIVATE external/CRCpp/inc)

add_library(echo SHARED "${SRC_FILES}")
set_target_properties(echo
        PROPERTIES
        VERSION ${PROJECT_VERSION} SOVERSION 0
        PUBLIC_HEADER "${API_FILES}")

target_link_libraries(echo Qt5::Multimedia)


add_subdirectory(tests)
add_subdirectory(docs)
