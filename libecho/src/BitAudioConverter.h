#ifndef ECHOCONNECT_BIT_AUDIO_CONVERTER_H
#define ECHOCONNECT_BIT_AUDIO_CONVERTER_H

#include "IAudioConverter.h"

#include <cmath>

template <typename T>
class BitAudioConverter : public IAudioConverter<T> {
public:
    BitAudioConverter(QAudioFormat inputFormat, QAudioFormat outputFormat, double windowLength, int lo, int hi)
        : IAudioConverter<T>(inputFormat, outputFormat, windowLength), loFreq(lo), hiFreq(hi) {}

private:
    const int loFreq, hiFreq;

    std::vector<T> encode_byte(uint8_t data) override {
        std::vector<T> result;
        for (int i = 0; i < 8; i++) {
            int bit = (data >> i) & 1;
            int freq = (bit == 1) ? hiFreq : loFreq;
            for (int t = 0; t < this->outputFormat.sampleRate() * this->windowLength; t++) {
                result.emplace_back(audio_wave(freq, t));
            }
        }
        return result;
    }

    uint8_t decode_byte(std::vector<char>) override {
        return 0;
    }

    T audio_wave(int frequency, int time) {
        return sin((double)frequency * time / this->outputFormat.sampleRate()) *
               (1 << (this->outputFormat.sampleSize() - 1));
    }
};

#endif  // ECHOCONNECT_BIT_AUDIO_CONVERTER_H
