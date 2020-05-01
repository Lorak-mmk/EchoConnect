#include "Echo.h"
#include "BitSender.h"

int main(int argc, char **argv) {
	Echo::initEcho(argc, argv);

	if (argc != 2) {
		printf("usage: wave <freq>\n");
		return 1;
	}

	int freq = atoi(argv[1]);

	std::vector<uint8_t> vec = {0};
	BitSender sender(1000000, freq, freq);
	sender.sendBlocking(vec);
}
