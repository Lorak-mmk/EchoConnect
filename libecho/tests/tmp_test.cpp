#include "echo.h"

#include <gtest/gtest.h>
#include <QtCore/QFile>

TEST(test_send, tmp_test) {
    /*QFile sourceFile;
    sourceFile.setFileName("/dev/urandom");
    sourceFile.open(QIODevice::ReadOnly);
    auto urandom = sourceFile.read(1000);

    std::vector<char> vbuffer(urandom.begin(), urandom.end());
    echo::send(vbuffer);*/
}

TEST(test_receive, tmp_test) {
    echo::receive();
}