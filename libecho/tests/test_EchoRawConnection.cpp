#include "EchoRawConnection.h"

#include <gtest/gtest.h>
#include <vector>

std::vector<uint8_t> data{97, 97, 99, 100, 101, 102, 103, 104, 105};

TEST(tests_Bit, test_send) {
    auto echorc = EchoRawConnection::getBitEchoRawConnection(200, 14000, 15000, 14000, 15000, 100);
    echorc.send(data);
    sleep(2);
    echorc.send(data);
    sleep(2);
}

TEST(tests_Bit, test_receive) {}

TEST(tests_Bit, test_receiveFirst) {}
