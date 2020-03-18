#include <gtest/gtest.h>
#include <QtCore/QFile>
#include "echo.h"

TEST(test_send, tmp_test) {
    QFile sourceFile;
    sourceFile.setFileName("/dev/urandom");
    sourceFile.open(QIODevice::ReadOnly);
    auto urandom = sourceFile.read(100000);

    std::vector<char> vbuffer(urandom.begin(), urandom.end());
    echo::send(vbuffer);
}

TEST(test_receive, tmp_test) {}