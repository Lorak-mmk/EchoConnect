#include <gtest/gtest.h>
#include <QtCore/QFile>

#include <cassert>

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

/*TEST(test_receive, tmp_test) {
    int pid = fork();
    assert(pid != -1);
    if (pid > 0) {
        Echo echo;

        std::vector<uint8_t> rec = echo.receive();
        //assert(rec.size() < 258 && rec.size() > 254);
        for (int i = 0; i != 0x100; ++i) printf("%x ", rec[i]);
        puts("");
    }
    else {
        sleep(1);
        execlp("/usr/bin/aplay", "aplay", "sample.wav", NULL);
    }
}
TEST(test_send, tmp_test) {
    std::vector<uint8_t> vbuffer;
    for (int i = 0; i < 256; i++) {
        vbuffer.push_back(i);
    }
    Echo echo;
    echo.send(vbuffer);
}*/

// TEST(test_send_receive, tmp_test) {
//     srand(time(nullptr));
//     std::vector<uint8_t> vbuff;
//     for (uint8_t i = 0; i != 100; ++i) {
//         vbuff.push_back(rand() & 0xff);
//     }
//     int pid = fork();
//     assert(pid != -1);
//     if (pid == 0) {  // child process
//         Echo echo;
//         std::vector<uint8_t> rec = echo.receive();
//         int ok = 0;
//         for (size_t i = 0; i != std::min(vbuff.size(), rec.size()); ++i) {
//             if (rec[i] == vbuff[i])
//                 ok++;
//         }
//         assert((int)rec.size() - (int)vbuff.size() <= 5);
//         printf("vbuff: ");
//         for (size_t i = vbuff.size(); i-- > 0;)
//             printf("%x ", vbuff[i]);
//         puts("");
//         printf("rec:   ");
//         for (size_t i = rec.size(); i-- > 0;)
//             printf("%x ", rec[i]);
//         puts("");
//         qDebug() << ok << "/ 100 bytes received correctly";
//         exit(ok);
//     } else {  // parent process
//         Echo echo;
//         QThread::msleep(4000);
//         echo.send(vbuff);
//
//         int wstatus;
//         pid_t p = wait(&wstatus);
//         assert(p == pid);
//         if (WEXITSTATUS(wstatus) > 90)
//             exit(0);
//         else
//             exit(1);
//     }
// }

