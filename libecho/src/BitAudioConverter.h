#ifndef ECHOCONNECT_BIT_AUDIO_CONVERTER_H
#define ECHOCONNECT_BIT_AUDIO_CONVERTER_H

#include "IAudioConverter.h"

#include <cmath>

namespace {
const size_t SOUNDS_PER_BYTE = 8;
}

template <typename T>
class BitAudioConverter : public IAudioConverter<T> {
public:
    BitAudioConverter(QAudioFormat inputFormat, QAudioFormat outputFormat, double windowSize, int lo, int hi)
        : IAudioConverter<T>(inputFormat, outputFormat, windowSize, SOUNDS_PER_BYTE), loFreq(lo), hiFreq(hi) {}

    std::vector<uint8_t> decode(std::vector<char> data) override {
        return std::vector<uint8_t>();
    }

private:
    const int loFreq, hiFreq;

    std::vector<T> encode_byte(uint8_t data) override {
        std::vector<T> result;
        result.reserve(this->encryptedByteSize);

        for (int i = 0; i < 8; i++) {
            int bit = (data >> i) & 1;
            int freq = (bit == 1) ? hiFreq : loFreq;
            for (int t = 0; t < this->windowSize; t++) {
                result.emplace_back(audio_wave(freq, t));
            }
        }

        return result;
    }

    uint8_t decode_byte(std::vector<char>) override {
        return 0;
    }

    T audio_wave(int frequency, int time) {
        return (std::sin(2.0 * M_PI * time * frequency / this->outputFormat.sampleRate()) + 1.0) * 
                (1 << (this->outputFormat.sampleSize() - 2));
    }
};

#endif  // ECHOCONNECT_BIT_AUDIO_CONVERTER_H
