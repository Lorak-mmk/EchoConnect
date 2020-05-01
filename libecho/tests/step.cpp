#include <math.h>

#include "Echo.h"
#include "AudioInput.h"
	
int16_t window[30000];
double mags[20000];

double mag(double re, double im) {
	return sqrt(re * re + im * im);
}

static void dft_slide(const int16_t *buffer, int win_size, double ratio, double *out, int len) {
    double re = 0;
    double im = 0;
    double angle0 = 0;
    double angle1 = 0;
    double d_angle = 2.0 * M_PI * ratio;

    for (int i = 0; i < win_size; i++) {
        re += buffer[i] * std::cos(angle1);
        im += buffer[i] * std::sin(angle1);
        angle1 += d_angle;
    }
    out[0] = mag(re, im) / static_cast<double>(win_size);

    for (int i = 0; i < len - 1; i++) {
        re -= buffer[i] * std::cos(angle0);
        im -= buffer[i] * std::sin(angle0);
        re += buffer[i + win_size] * std::cos(angle1);
        im += buffer[i + win_size] * std::sin(angle1);
        out[i + 1] = mag(re, im) / static_cast<double>(win_size);
        angle0 += d_angle;
        angle1 += d_angle;
    }
}

void step_shift(const double *buffer, int size, int lim) {
    double sum = 0;
    double delta;
    double diff;
    // TODO: this cutoff point is loosely dependent on the window size and the
    // volume and noisiness of the microphone. I don't think there's a reasonable
    // way to compute this, but who knows. For now it's just hardcoded.
    double diff_min = INFINITY;

    for (int i = 0; i < size; i++) {
        sum += buffer[i];
    }

    for (int i = 0; i < size; i++) {
        sum += buffer[i + size];
        delta = buffer[i + size] - buffer[i];
        //printf("%lf\n", delta);
        if (delta < lim) {
            continue;
        }

        diff = sum - 0.5 * ((size + 1) * (buffer[i + size] + buffer[i]));
        diff = std::abs(diff);
        if (diff_min > diff) {
            diff_min = diff;
        }
        sum -= buffer[i];
    }
	if (diff_min != INFINITY)
		printf("%10.6lf\n", diff_min);
}

int main(int argc, char **argv) {
	Echo::initEcho(argc, argv);

	if (argc != 4) {
		printf("usage: step <win_size> <freq> <lim>\n");
		return 1;
	}
	int win_size = atoi(argv[1]);
	int freq = atoi(argv[2]);
	int lim = atoi(argv[3]);

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
			int nread = input.readBytes((char *) (window + (2 * win_size)) + inc, bytes);
			inc += nread;
			bytes -= nread;
			input.waitForTick();
		}

		dft_slide(window, win_size, (double) freq / 44100, mags, 2 * win_size);
		for (int i = 0; i < 2 * win_size; i++)
			window[i] = window[i + win_size];
		step_shift(mags, win_size, lim);
	}
}
