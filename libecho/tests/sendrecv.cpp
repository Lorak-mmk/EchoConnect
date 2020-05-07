#include "BitReceiverv2.h"
#include "BitSenderv2.h"
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

    BitSenderv2 sender(win_size, freq);
    BitReceiverv2 receiver(win_size, freq, lim);

    if (argc > 4)
        receiver.dbg = out;

    sender.send(out, 40);
    for (int i = 0; i < N; i++)
        out[i] = i;
    sender.start();
    sender.send(out, N);

    receiver.start(30s);
    receiver.receive(in, N);

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
