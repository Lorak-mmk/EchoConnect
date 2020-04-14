#include "BitSender.h"

#include <cmath>
#include <iostream>

const QAudioFormat::Endian OUTPUT_BYTEORDER = QAudioFormat::LittleEndian;
const int OUTPUT_CHANNEL_COUNT = 1;
const char *OUTPUT_CODEC = "audio/pcm";
const int OUTPUT_SAMPLERATE = 44100;
const int OUTPUT_SAMPLESIZE = sizeof(SampleType);
const QAudioFormat::SampleType OUTPUT_SAMPLETYPE = QAudioFormat::SignedInt;
/**^ QAudioFormat settings we use while sending audio. */

static constexpr size_t SOUNDS_PER_BYTE =
    8; /**< States the number of sounds we need to play to encode a byte of data. */

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
    output->waitForState(QAudio::State::IdleState);
//     auto encoded = encode(buffer);
//     TODO: try to make atOnce smaller
//     const size_t atOnce = 2600; /* < Temporary - this constant was created empirically.
//                                      Shortly - it says how much audio data need to be pushed to stream buffer
//                                      every time so there won't be pause in transmission. */
//     output->startStream();
//     /* At the beggining we enqueue surplus audio data to play
//        to fill out the buffer so the pause in transmission won't happen. */
//     output->enqueueData(encoded.data(), std::min(encoded.size(), atOnce));
//     for (size_t i = atOnce; i < encoded.size(); i += atOnce) {
//         NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
//         output->enqueueData(encoded.data() + i, std::min(atOnce, encoded.size() - i));
//         auto status = output->getStreamStatus();
//         qDebug() << "Status: " << status.first << " " << status.second;
//         output->waitForTick();
//     }
// 
//     output->waitForState(QAudio::State::IdleState);
//     output->stopStream();
}

std::vector<char> BitSender::encode(const std::vector<uint8_t> &data) {
    std::vector<SampleType> result;
    result.reserve(data.size() * winSize * SOUNDS_PER_BYTE);

    for (auto byte : data) {
        auto encodedByte = encodeByte(byte);
        result.insert(result.end(), encodedByte.begin(), encodedByte.end());
    }

    // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    return std::vector<char>(result.data(), result.data() + result.size());
}

std::vector<SampleType> BitSender::encodeByte(uint8_t data) {
    std::vector<SampleType> result;
    result.reserve(winSize * SOUNDS_PER_BYTE);

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    for (int i = 0; i < 8; i++) {
        int bit = (data >> i) & 1;
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
