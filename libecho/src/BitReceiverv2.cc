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

    int mid_sum = 0;
    int mid_cnt = 0;

    fetch(win_size);
    for (int i = 0; i < win_size; i++) {
        if (mags[i] > lim) {
            mid_sum += i;
            mid_cnt++;
        }
    }
    high_mag = mags[win_size / 2];

    fetch(win_size);
    for (int i = 0; i < win_size; i++) {
        if (mags[i] < lim) {
            mid_sum += i;
            mid_cnt++;
        }
    }
    low_mag = mags[win_size / 2];

    fetch(win_size);
    for (int i = 0; i < win_size; i++) {
        if (mags[i] > lim) {
            mid_sum += i;
            mid_cnt++;
        }
    }
    fetch(win_size);

    offset = mid_cnt > 0 ? (win_size / 2) - (mid_sum / mid_cnt) : 0;

    if (offset > 0) {
        fetch(win_size);
        fetch(offset);
    } else {
        fetch(win_size + offset);
    }
}

uint8_t BitReceiverv2::read_bit() {
    //static int seq = 0;  // for dbg
    double val;
    uint8_t res;

    fetch(win_size);
    val = mags[win_size / 2];
    res = val > lim;

    if (res != prev_bit) {
        if (res) {
            high_mag = (high_mag + val) / 2;
        }
        else {
            low_mag = (low_mag + val) / 2;
        }
        lim = (6 * lim + (high_mag + low_mag) / 2) / 7;
    }
    prev_bit = res;

	/*
    if (dbg) {
        if (res != ((*dbg >> seq) & 1))
            fprintf(stderr, "should be %d (lim = %lf)\n", !res, lim);

        for (int i = 0; i < win_size; i++) {
            for (int j = 0; j < mags[i]; j++) {
                if (j == (int)lim)
                    fprintf(stderr, "|");
                else
                    fprintf(stderr, res == ((*dbg >> seq) & 1) ? "#" : "*");
            }
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "\n");

        seq++;
        if (seq == 8) {
            seq = 0;
            dbg++;
        }
    }
    */

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
