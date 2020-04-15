#ifndef ECHOCONNECT_BIT_AUDIO_CONVERTER_H
#define ECHOCONNECT_BIT_AUDIO_CONVERTER_H

#include "IAudioConverter.h"

#include <cmath>

static constexpr size_t SOUNDS_PER_BYTE =
    8; /**< States the number of sounds we need to play to encode a byte of data. */

/**
 * @brief Class which performs data to/from audio conversions by encoding each bit separately.
 *
 * We decided to convert every bit of data as one sound which lasts for windowSize / sampleRate seconds. We specify two
 * frequencies in which we encode every possible value of a bit - respectively loFreq for 0 value and hiFreq for 1
 * value. We consider this to be the simpliest possible way to encode data into a sound and because of that we decided
 * to start with that approach.
 */
template <typename T>
class BitAudioConverter : public IAudioConverter<T> {
public:
    /**
     * @brief Constructs a BitAudioConverter object.
     *
     * Following parameters are mostly used to determine how data should be encoded/decoded. @see
     * IAudioConverter::IAudioConverter.
     * @param inputFormat   Look at IAudioConverter::IAudioConverter.
     * @param outputFormat  Look at IAudioConverter::IAudioConverter.
     * @param windowSize  Look at IAudioConverter::IAudioConverter.
     * @param lo    Frequency in which we encode bits of value 0.
     * @param hi    Frequency in which we encode bits of value 1.
     */
    BitAudioConverter(const QAudioFormat& inputFormat, const QAudioFormat& outputFormat, int windowSize, int lo, int hi)
        : IAudioConverter<T>(inputFormat, outputFormat, windowSize, SOUNDS_PER_BYTE), loFreq(lo), hiFreq(hi) {}

    /**
     * @brief Temporary, unused implementation - written only to ensure that code compiles.
     *
     * TODO for Paweł: implementation and documentation
     */
    std::vector<uint8_t> decode(__attribute__((unused)) const std::vector<char> &data) override {
        return std::vector<uint8_t>();
    }

private:
    const int loFreq, hiFreq; /**< Frequencies used to encode/decode bits, respectively for 0 and 1 values. */

    /**
     * @brief Encodes byte into audio format (speakers membrane amplitude) by encoding every single bit.
     *
     * Every bit is encoded as one sound lasting for windowSize / outputFormat.sampleRate seconds in one of two
     * specified frequencies.
     * Based on <a href="https://en.wikipedia.org/wiki/Pulse-code_modulation">pcm/audio</a> encoding.
     */
    std::vector<T> encode_byte(uint8_t data) override {
        std::vector<T> result;
        result.reserve(this->encryptedByteSize);

        for (int i = 0; i < CHAR_BIT * sizeof(data); i++) {
            int bit = (data >> i) & 1;
            int freq = (bit == 1) ? hiFreq : loFreq;
            for (int t = 0; t < this->windowSize; t++) {
                result.emplace_back(audio_wave(freq, t));
            }
        }

        return result;
    }

    /**
     * @brief Temporary, unused implementation - written only to ensure that code compiles.
     *
     * TODO for Paweł: implementation and documentation
     */
    uint8_t decode_byte(__attribute__((unused)) const std::vector<char> &data) override {
        return 0;
    }

    /**
     * @brief Used for sampling value of audio wave with passed @p frequency at @p time * π / sampleRate position.
     */
    T audio_wave(int frequency, int time) {
        return (std::sin(2.0 * M_PI * time * frequency / this->outputFormat.sampleRate()) + 1.0) *
               (1 << (this->outputFormat.sampleSize() - 2));
    }
};

#endif  // ECHOCONNECT_BIT_AUDIO_CONVERTER_H
