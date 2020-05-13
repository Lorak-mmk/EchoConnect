#include <unistd.h>

#include "Echo.h"
#include "EchoCalibration.h"

int main(int argc, char **argv) {
	Echo::initEcho(argc, argv);

	if (argc != 3) {
		printf("usage: self_cal <win_size> <freq>\n");
		exit(1);
	}

	auto *cal = EchoCalibration::getEchoCalibration(atoi(argv[1]), atoi(argv[2]));

	cal->startPlayback();
	printf("%lf\n", cal->getLim(100, 1000));
	cal->stopPlayback();

	return 0;
}
