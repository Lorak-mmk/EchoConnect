#include <gtest/gtest.h>

#include "Echo.h"
#include "BitSender.h"
#include "BitReceiver.h"

#define N 100
#define S 50

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    Echo::initEcho(argc, argv);

    BitSender sender(S, 0, 0);
    BitReceiver receiver(S, 14000, 15000, 15);

    uint8_t arr[N];

    std::vector<uint8_t> vec;
    for (int i = 0; i < 40; i++)
    	vec.push_back(0);
    sender.send(vec);

    sender.setLoFreq(14000);
    sender.setHiFreq(15000);
	vec.clear();
    for (int i = 0; i < N; i++) vec.push_back(i);
    //for (int i = 0; i < 10; i++) vec.push_back(0xaa);
    sender.send(vec);
		
    receiver.receiveFirst(arr, N);

	int wrong = 0;
	for (int i = 0; i < N; i++) {
		printf("%02hhx ", arr[i]);
		if (arr[i] != vec[i])
			wrong++;
	}
	printf("\nwrong: %d\n", wrong);
    //return RUN_ALL_TESTS();
}
