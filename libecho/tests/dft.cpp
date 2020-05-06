#include <math.h>

#include "AudioInput.h"
#include "Echo.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

    if (argc != 3) {
        printf("usage: step <win_size> <freq>\n");
        return 1;
    }
    int win_size = atoi(argv[1]);
    int freq = atoi(argv[2]);

    int16_t *dwindow = new int16_t[2 * win_size];
    double *mags = new double[win_size];

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
            int nread = input.readBytes((char *)(dwindow + win_size) + inc, bytes);
            inc += nread;
            bytes -= nread;
        }

        dft_slide(dwindow, win_size, (double)freq / 44100, mags, win_size);
        for (int i = 0; i < win_size; i++) {
            for (int j = 0; j < mags[i]; j++)
                putchar('#');
            putchar('\n');
        }

        for (int i = 0; i < win_size; i++)
            dwindow[i] = dwindow[i + win_size];
    }
}
