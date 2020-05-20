#include "BitReceiverv2.h"
#include "Exceptions.h"

#include <cmath>
#include "Dft.h"

static const int SAMPLE_RATE = 44100;
static const int SAMPLE_SIZE = 16;
static const int CHANNEL_COUNT = 1;
static const char *CODEC = "audio/pcm";
static const QAudioFormat::SampleType SAMPLE_TYPE = QAudioFormat::SignedInt;
static const QAudioFormat::Endian BYTEORDER = QAudioFormat::LittleEndian;

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

void BitReceiverv2::start(std::chrono::duration<double> timeout) {
    int streak = 0;
    int offset = -1;

    auto start = std::chrono::system_clock::now();

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
        if (std::chrono::system_clock::now() - start > timeout) {
            throw ConnectionBroken{};
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
            printf("should be %d\n", !res);
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
