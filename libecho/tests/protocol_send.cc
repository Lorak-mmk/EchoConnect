#include "Echo.h"
#include "EchoProtocol.h"

int main(int argc, char *argv[]) {
    Echo::initEcho(argc, argv);

    EchoProtocol p(150, 16000, 14000, 1187);
    p.connect();
    p.write("dupadupadupa", 12);
    p.write("kocham anime", 12);
    p.close();
}
