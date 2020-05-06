#include <math.h>
#include "AudioInput.h"
#include "Echo.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define WINDOWS 1000

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

int main(int argc, char **argv) {
    Echo::initEcho(argc, argv);

    if (argc < 3) {
        printf(
            "usage: getlim <win_size> <freq> [skip]\n"
            "The skip parameter tells how many windows to skip.\n"
            "Eg. if your mic picks up the key you press to launch\n"
            "this program and messes up the waveform, a value of\n"
            "1000 should be fine. It is 0 by default\n");
        return 1;
    }
    int win_size = atoi(argv[1]);
    int freq = atoi(argv[2]);
    int skip = (argc == 3 ? 0 : atoi(argv[3]));

    QAudioFormat fmt;
    fmt.setByteOrder(QAudioFormat::LittleEndian);
    fmt.setChannelCount(1);
    fmt.setCodec("audio/pcm");
    fmt.setSampleRate(44100);
    fmt.setSampleSize(16);
    fmt.setSampleType(QAudioFormat::SignedInt);
    AudioInput input(fmt);
    input.startStream();

    int16_t *samples = new int16_t[win_size * (WINDOWS + 1)];
    double *mags = new double[win_size * WINDOWS];

    for (int i = 0; i < skip; i++) {
        int inc = 0;
        int bytes = win_size * 2;
        while (bytes > 0) {
            int nread = input.readBytes((char *)samples + inc, bytes);
            inc += nread;
            bytes -= nread;
            input.waitForTick();
        }
    }

    int inc = 0;
    int bytes = win_size * (WINDOWS + 1) * 2;
    while (bytes > 0) {
        int nread = input.readBytes((char *)samples + inc, bytes);
        inc += nread;
        bytes -= nread;
        input.waitForTick();
    }

    dft_slide(samples, win_size, freq / 44100.0, mags, win_size * WINDOWS);

    for (int i = 0; i < win_size * WINDOWS; i++) {
        for (int j = 0; j < mags[i]; j++)
            putchar('#');
        putchar('\n');
    }

    std::sort(mags, mags + (win_size * WINDOWS));
    printf(">>>>>>> the best lim seems to be %lf <<<<<<<\n", mags[win_size * WINDOWS / 2]);

    delete[] samples;
    delete[] mags;

    return 0;
}
