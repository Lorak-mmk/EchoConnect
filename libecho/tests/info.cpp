#include <math.h>

#include "Echo.h"
#include "AudioInput.h"
	
int16_t window[10000];

double mag(double re, double im) {
	return sqrt(re * re + im * im);
}

static double dft(const int16_t *buffer, int win_size, double ratio) {
    double re = 0;
    double im = 0;
    double angle = 0;
    double d_angle = 2.0 * M_PI * ratio;

    for (int i = 0; i < win_size; i++) {
        double val = buffer[i];
        re += val * cos(angle);
        im += val * sin(angle);
        angle += d_angle;
    }

    return mag(re, im) / static_cast<double>(win_size);
}

int main(int argc, char **argv) {
	Echo::initEcho(argc, argv);

	if (argc != 3) {
		printf("usage: info <win_size> <freq>\n");
		return 1;
	}

	int win_size = atoi(argv[1]);
	int freq = atoi(argv[2]);

	QAudioFormat fmt;
	fmt.setByteOrder(QAudioFormat::LittleEndian);
    fmt.setChannelCount(1);
    fmt.setCodec("audio/pcm");
    fmt.setSampleRate(44100);
    fmt.setSampleSize(16);
    fmt.setSampleType(QAudioFormat::SignedInt);
	AudioInput input(fmt);
	input.startStream();


	while (1) {
		int inc = 0;
		int bytes = win_size * 2;
		while (bytes > 0) {
			int nread = input.readBytes((char *) window + inc, bytes);
			inc += nread;
			bytes -= nread;
			input.waitForTick();
		}

		printf("%10.6lf\n", dft(window, win_size, (double) freq / 44100));
	}
}
