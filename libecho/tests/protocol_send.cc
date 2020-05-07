#include "Echo.h"
#include "EchoProtocol.h"

int main(int argc, char *argv[]) {
    Echo::initEcho(argc, argv);

    EchoProtocol p(150, 16000, 14000, 1187);
    p.connect();
    p.write("abcdefghijkl", 12);
    p.write("mmmmmmmmmmmm", 12);
    p.close();
}
