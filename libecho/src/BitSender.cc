#include "BitSender.h"

#include <cmath>

const QAudioFormat::Endian OUTPUT_BYTEORDER = QAudioFormat::LittleEndian;
const int OUTPUT_CHANNEL_COUNT = 1;
const char *OUTPUT_CODEC = "audio/pcm";
const int OUTPUT_SAMPLERATE = 44100;
const int OUTPUT_SAMPLESIZE = sizeof(SampleType);
const QAudioFormat::SampleType OUTPUT_SAMPLETYPE = QAudioFormat::SignedInt;
/**< QAudioFormat settings we use while sending audio. */

static constexpr size_t SOUNDS_PER_BYTE =
    8; /**< States the number of sounds we need to play to encode a byte of data. */

std::vector<char> BitSender::encode(const std::vector<uint8_t> &data) {
    std::vector<SampleType> result;
    result.reserve(data.size() * windowSize * SOUNDS_PER_BYTE);

    for (auto byte : data) {
        auto encoded_byte = encode_byte(byte);
        result.insert(result.end(), encoded_byte.begin(), encoded_byte.end());
    }

    return std::vector<char>(result.data(), result.data() + result.size());
}

std::vector<SampleType> BitSender::encode_byte(uint8_t data) {
    std::vector<SampleType> result;
    result.reserve(windowSize * SOUNDS_PER_BYTE);

    for (int i = 0; i < 8; i++) {
        int bit = (data >> i) & 1;
        int freq = (bit == 1) ? hiFreq : loFreq;
        for (int t = 0; t < windowSize; t++) {
            result.emplace_back(audio_wave(freq, t));
        }
    }

    return result;
}

SampleType BitSender::audio_wave(int frequency, int time) {
    return (std::sin(2.0 * M_PI * time * frequency / format.sampleRate()) + 1.0) *
           (1 << (format.sampleSize() - 2));
}

QAudioFormat BitSender::getOutputFormat() {
    QAudioFormat result;

    result.setByteOrder(OUTPUT_BYTEORDER);
    result.setChannelCount(OUTPUT_CHANNEL_COUNT);
    result.setCodec(OUTPUT_CODEC);
    result.setSampleRate(OUTPUT_SAMPLERATE);
    result.setSampleSize(OUTPUT_SAMPLESIZE);
    result.setSampleType(OUTPUT_SAMPLETYPE);

    return result;
}
