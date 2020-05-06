#include "BitSenderv2.h"
#include <cmath>
#include <iostream>

#define RISE 0.1
#define FALL 0.1

static const QAudioFormat::Endian OUTPUT_BYTEORDER = QAudioFormat::LittleEndian;
static const int OUTPUT_CHANNEL_COUNT = 1;
static const char *OUTPUT_CODEC = "audio/pcm";
static const int SAMPLE_RATE = 44100;
static const int SAMPLE_SIZE = 16;
static const QAudioFormat::SampleType SAMPLE_TYPE = QAudioFormat::SignedInt;

QAudioFormat BitSenderv2::getOutputFormat() {
    QAudioFormat result;

    result.setByteOrder(OUTPUT_BYTEORDER);
    result.setChannelCount(OUTPUT_CHANNEL_COUNT);
    result.setCodec(OUTPUT_CODEC);
    result.setSampleRate(SAMPLE_RATE);
    result.setSampleSize(SAMPLE_SIZE);
    result.setSampleType(SAMPLE_TYPE);

    return result;
}

void BitSenderv2::start() {
    int16_t *out = new int16_t[win_size * 3];
    write_bit(out, 1);
    write_bit(out + win_size, 0);
    write_bit(out + win_size + win_size, 0);
    output->enqueueData((char *)out, win_size * 6);
    delete[] out;
}

void BitSenderv2::send(uint8_t *buffer, int size) {
    int16_t *out = new int16_t[size * win_size * 8];
    int16_t *out_end = out;
    uint8_t byte;

    for (int i = 0; i < size; i++) {
        byte = buffer[i];
        for (int j = 0; j < 8; j++) {
            write_bit(out_end, byte & 1);
            out_end += win_size;
            byte >>= 1;
        }
    }

    output->enqueueData((char *)out, size * win_size * 16);
    delete[] out;
}

void BitSenderv2::wait() {
    output->waitForState(QAudio::State::IdleState);
}

void BitSenderv2::write_bit(int16_t *out, uint8_t bit) {
    for (int i = 0; i < win_size; i++) {
        out[i] = sin(2.0 * M_PI * counter * freq / SAMPLE_RATE) * 32766 * amp;
        counter++;

        if (bit) {
            amp += RISE;
            if (amp >= 1.0)
                amp = 1.0;
        } else {
            amp -= FALL;
            if (amp <= 0.0)
                amp = 0.0;
        }
    }
}
