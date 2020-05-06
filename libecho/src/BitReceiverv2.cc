#include "BitReceiverv2.h"
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
static const int SAMPLE_RATE = 44100;
static const int SAMPLE_SIZE = 16;
static const int CHANNEL_COUNT = 1;
static const char *CODEC = "audio/pcm";
static const QAudioFormat::SampleType SAMPLE_TYPE = QAudioFormat::SignedInt;
static const QAudioFormat::Endian BYTEORDER = QAudioFormat::LittleEndian;

static double mag(double re, double im) {
    return sqrt(re * re + im * im);
}

/*
static double dft(const int16_t *buffer, int win_size, double ratio) {
    double re = 0;
    double im = 0;
    double angle = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    double d_angle = 2.0 * M_PI * ratio;

    for (int i = 0; i < win_size; i++) {
        double val = buffer[i];
        re += val * std::cos(angle);
        im += val * std::sin(angle);
        angle += d_angle;
    }

    return mag(re, im) / static_cast<double>(win_size);
}
*/

static void dft_slide(const int16_t *buffer, int win_size, double ratio, double *out, int len) {
    double re = 0;
    double im = 0;
    double angle0 = 0;
    double angle1 = 0;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
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

QAudioFormat BitReceiverv2::getInputFormat() {
    QAudioFormat format;
    format.setByteOrder(BYTEORDER);
    format.setChannelCount(CHANNEL_COUNT);
    format.setCodec(CODEC);
    format.setSampleRate(SAMPLE_RATE);
    format.setSampleSize(SAMPLE_SIZE);
    format.setSampleType(SAMPLE_TYPE);
    return format;
}

BitReceiverv2::BitReceiverv2(int win_size, int freq, double lim) : IReceiver(getInputFormat(), win_size) {
    dwindow = new int16_t[2 * win_size];
    mags = new double[win_size];
    freq_ratio = (double)freq / SAMPLE_RATE;
    this->lim = lim;

    for (int i = 0; i < win_size; i++) {
        dwindow[i] = 0;
        dwindow[i + win_size] = 0;
        mags[i] = 0;
    }
}

BitReceiverv2::~BitReceiverv2() {
    delete[] dwindow;
    delete[] mags;
}

void BitReceiverv2::fetch(int len) {
    int inc = 2 * (2 * win_size - len);
    int bytes = len * 2;

    for (int i = 0; i < 2 * win_size - len; i++) {
        dwindow[i] = dwindow[i + len];
    }

    while (bytes > 0) {
        int nread = input->readBytes(reinterpret_cast<char *>(dwindow) + inc, bytes);
        inc += nread;
        bytes -= nread;
        input->waitForTick();
    }

    dft_slide(dwindow, win_size, freq_ratio, mags, win_size);

    /*
    for (int i = win_size - len; i < win_size; i++) {
            for (int j = 0; j < mags[i]; j++)
                    putchar('#');
            putchar('\n');
    }
    */
}

void BitReceiverv2::start() {
    int streak = 0;
    int offset = -1;

    while (offset < 0) {
        fetch(win_size);
        for (int i = 0; i < win_size; i++) {
            if (mags[i] > lim) {
                streak++;
            } else {
                if (win_size / 2 < streak && streak < win_size * 2) {
                    offset = i + win_size + (win_size - streak) / 2;
                }
                streak = 0;
            }
        }
    }

    while (offset > win_size) {
        fetch(win_size);
        offset -= win_size;
    }
    fetch(offset);
}

uint8_t BitReceiverv2::read_bit() {
    static int seq = 0;  // for dbg
    /*
    for (int i = 0; i < 300; i++)
            putchar('-');
    putchar('\n');
    */

    /*
    int a = 10;
    int above = 0;

    fetch(win_size);
    for (int i = a; i < win_size - a; i++) {
            if (mags[i] > lim)
                    above++;
    }
    return above > (win_size - 2 * a) / 2;
    */

    fetch(win_size);
    uint8_t res = (mags[win_size / 2] > lim);

    if (dbg) {
        if (res != ((*dbg >> seq) & 1)) {
            printf("should be %hhu\n", !res);
            for (int i = 0; i < win_size; i++) {
                for (int j = 0; j < mags[i]; j++) {
                    putchar(j == (int)lim ? '|' : '#');
                }
                putchar('\n');
            }
            putchar('\n');
        }
        seq++;
        if (seq == 8) {
            seq = 0;
            dbg++;
        }
    }

    return res;
}

int BitReceiverv2::receive(uint8_t *buffer, int size) {
    for (int i = 0; i < size; i++) {
        *buffer = 0;
        for (int j = 0; j < 8; j++) {
            *buffer |= (read_bit() << j);
        }
        buffer++;
    }

    return size;
}
