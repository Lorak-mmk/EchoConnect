#include "Echo.h"

#include <gtest/gtest.h>
#include <QtCore/QFile>

#include <cassert>

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

TEST(test_send_receive, tmp_test) {
    srand(time(nullptr));
    std::vector<uint8_t> vbuff;
    for (uint8_t i = 0; i != 100; ++i) {
        vbuff.push_back(rand() & 0xff);
    }
    int pid = fork();
    ASSERT_NE(pid, -1);
    if (pid == 0) { // child process
        Echo echo;
        std::vector<uint8_t> rec = echo.receive();
        int ok = 0;
        for (size_t i = 0; i != std::min(vbuff.size(), rec.size()); ++i) {
            if (rec[i] == vbuff[i]) ok++;
        }
        EXPECT_LE((int)rec.size() - (int)vbuff.size(), 5);
        printf("vbuff: ");
        for (size_t i=vbuff.size(); i-->0; ) printf("%x ", vbuff[i]);
        puts("");
        printf("rec:   ");
        for (size_t i=rec.size(); i-->0; ) printf("%x ", rec[i]);
        puts("");
        qDebug() << ok << "/ 100 bytes received correctly";
        exit(ok);
    }
    else { // parent process
        Echo echo;
        QThread::msleep(4000);
        echo.send(vbuff);
        
        int wstatus;
        pid_t p = wait(&wstatus);
        ASSERT_EQ(p, pid);
        EXPECT_GT(WEXITSTATUS(wstatus), 90);
    }
}
