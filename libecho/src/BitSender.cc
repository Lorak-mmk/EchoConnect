#include "BitSender.h"
#include "HammingCode.h"

#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const QAudioFormat::Endian OUTPUT_BYTEORDER = QAudioFormat::LittleEndian;
const int OUTPUT_CHANNEL_COUNT = 1;
const char *OUTPUT_CODEC = "audio/pcm";
const int OUTPUT_SAMPLERATE = 44100;
const int OUTPUT_SAMPLESIZE = 8;
const QAudioFormat::SampleType OUTPUT_SAMPLETYPE = QAudioFormat::UnSignedInt;
/* ^ QAudioFormat settings we use while sending audio. */

/* v States the number of sounds we need to play to encode a byte of data. */
static constexpr size_t SOUNDS_PER_BYTE = 8;

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

void BitSender::send(const std::vector<uint8_t> &buffer) {
    auto encoded = encode(buffer);
    output->enqueueData(encoded.data(), encoded.size());
}

void BitSender::sendBlocking(const std::vector<uint8_t> &buffer) {
    auto encoded = encode(buffer);
    output->enqueueData(encoded.data(), encoded.size());
    sendWait();
}

void BitSender::sendWait() {
    output->waitForState(QAudio::State::IdleState);
}

std::vector<char> BitSender::encode(const std::vector<uint8_t> &data) {
    std::vector<SampleType> result;
    result.reserve(data.size() * winSize * SOUNDS_PER_BYTE);

    for (auto byte : data) {
        auto encodedByte = encodeByte(byte);
        result.insert(result.end(), encodedByte.begin(), encodedByte.end());
    }

    return std::vector<char>(result.data(), result.data() + result.size());
}

std::vector<SampleType> BitSender::encodeByte(uint8_t data) {
    std::vector<SampleType> result;
    result.reserve(winSize * SOUNDS_PER_BYTE);

    HammingCode hamming;
    std::vector<uint8_t> data_vec = {data};  // TODO: this is really slow
    std::vector<bool> hammingEnc = hamming.encode(data_vec);

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    for (bool bit : hammingEnc) {
        int freq = (bit == 1) ? hiFreq : loFreq;
        for (int t = 0; t < winSize; t++) {
            result.emplace_back(audioWave(freq, t));
        }
    }

    return result;
}

SampleType BitSender::audioWave(int frequency, int time) {
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    return (std::sin(2.0 * M_PI * time * frequency / format.sampleRate()) + 1.0) * (1 << (format.sampleSize() - 2));
}
