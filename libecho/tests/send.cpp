#include "Echo.h"
#include "BitSender.h"
#include "BitReceiver.h"

#define N 1000
#define S 50

int main(int argc, char **argv) {
    Echo::initEcho(argc, argv);

    BitSender sender(S, 0, 0);
    BitReceiver receiver(S, 14000, 15000, 80);

    uint8_t arr[N];

    std::vector<uint8_t> vec;
    for (int i = 0; i < 40; i++)
    	vec.push_back(0);
    sender.send(vec);

    sender.setLoFreq(14000);
    sender.setHiFreq(15000);
	vec.clear();
    for (int i = 0; i < N; i++) vec.push_back(rand());
    sender.send(vec);
		
    receiver.receiveFirst(arr, N);

	int wrong = 0;
	for (int i = 0; i < N; i++) {
		if (arr[i] == vec[i]) {
			printf("%02hhx ", arr[i]);
		}
		else {
			printf("\x1b[31m%02hhx\x1b[m ", arr[i]);
			wrong++;
		}
	}
	printf("\nwrong: %d\n", wrong);
}
