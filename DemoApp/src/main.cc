#include "Echo.h"

#define N 300

using namespace std::chrono_literals;

uint8_t out[N];
uint8_t in[N];

int main(int argc, char **argv) {
    Echo::initEcho(argc, argv);

    if (argc < 4) {
        printf(
                "usage: sendrecv <win_size> <freq> <lim> [dbg]\n"
                "The dbg parameter is for reporting parts of the waveform\n"
                "that were received incorrectly. If you wish to enable it,\n"
                "just put any text there.\n");
        return 1;
    }
    int win_size = atoi(argv[1]);
    int freq = atoi(argv[2]);
    int lim = atoi(argv[3]);

    auto connection = EchoRawConnection::getBitEchoRawConnectionv2(win_size, freq, freq, lim);

    connection->send(out, 40);
    for (int i = 0; i < N; i++)
        out[i] = i;
    connection->sendStart();
    connection->send(out, N);

    connection->receiveStart(30s);
    connection->receive(in, N);

    int wrong = 0;
    for (int i = 0; i < N; i++) {
        if (in[i] == out[i]) {
            printf("%02hhx ", in[i]);
        } else {
            printf("\x1b[31m%02hhx\x1b[m ", in[i]);
            wrong++;
        }
    }
    printf("\nwrong: %d\n", wrong);
}