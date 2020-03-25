#include "Echo.h"

#include <gtest/gtest.h>
#include <QtCore/QFile>

TEST(test_send, tmp_test) {
    std::vector<uint8_t> vbuffer;
    for (int i = 0; i < 25; i++)
        vbuffer.push_back(i);
    Echo echo;
    echo.send(vbuffer);
}

TEST(test_receive, tmp_test) {
    Echo echo;

    QThread::msleep(100);  // to prevent interference with the previous test
    std::vector<uint8_t> rec = echo.receive();
    for (uint8_t u : rec) {
        if (32 < u && u < 127)
            putchar(u);
        else
            printf("<%hhu>", u);
    }
    printf("\n");
}
