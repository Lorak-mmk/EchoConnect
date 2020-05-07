#include "Echo.h"
#include "EchoProtocol.h"

char buf[31];
size_t pos = 0;

int main(int argc, char *argv[]) {
    Echo::initEcho(argc, argv);

    EchoProtocol p(150, 14000, 16000, 1187);
    p.listen();
    try {
        pos += p.read(buf, 30, 10);
        pos += p.read(buf + pos, 30 - pos, 10);
        pos += p.read(buf + pos, 30 - pos, 10);
        pos += p.read(buf + pos, 30 - pos, 10);
        p.close();
    } catch (std::runtime_error &e) {
        puts(e.what());
    }
    puts(buf);
}

