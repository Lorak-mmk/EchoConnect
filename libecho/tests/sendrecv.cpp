#include "BitReceiver.h"
#include "BitSender.h"
#include "Echo.h"

#define N 300
#define S 50

#define LO 16000
#define HI 18000

int main(int argc, char **argv) {
    Echo::initEcho(argc, argv);

    BitSender sender(S, 0, 0);
    BitReceiver receiver(S, LO, HI, 10, 80);

    uint8_t arr[N];

    std::vector<uint8_t> vec;
    for (int i = 0; i < 40; i++)
        vec.push_back(0);
    sender.send(vec);

    sender.setLoFreq(LO);
    sender.setHiFreq(HI);
    vec.clear();
    for (int i = 0; i < N; i++)
        vec.push_back(rand());
    sender.send(vec);

    receiver.receiveFirst(arr, N);

    int wrong = 0;
    for (int i = 0; i < N; i++) {
        if (arr[i] == vec[i]) {
            printf("%02hhx ", arr[i]);
        } else {
            printf("\x1b[31m%02hhx\x1b[m ", arr[i]);
            wrong++;
        }
    }
    printf("\nwrong: %d\n", wrong);
}
