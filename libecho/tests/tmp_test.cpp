#include <gtest/gtest.h>
#include "echo.h"

TEST(test_send, tmp_test) {
    const char *buffer = "abcdefgh\0hgfedcba";
    std::vector<char> vbuffer;
    for(int i = 0; i < 10000; i++) {
        vbuffer.insert(vbuffer.end(), buffer, buffer + 17);
    }
    echo::send(vbuffer);
}

TEST(test_receive, tmp_test) {}