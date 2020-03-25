#include "Echo.h"
#include "AudioFormatFactory.h"
#include "AudioInput.h"
#include "AudioOutput.h"
#include "BitAudioConverter.h"
#include "QTInitializer.h"

#include <QCoreApplication>
#include <QDebug>

static constexpr int windowSize = 200;
static constexpr int loFreq = 19000;
static constexpr int hiFreq = 20000;
static constexpr double magLimit = 300.0 / windowSize;

Echo::Echo() {
    auto inputFormat = AudioFormatFactory::getDefaultInputFormat();
    input = std::make_unique<AudioInput>(inputFormat);

    auto outputFormat = AudioFormatFactory::getDefaultOutputFormat();
    output = std::make_unique<AudioOutput>(outputFormat);

    converter = std::make_unique<BitAudioConverter<SampleType>>(inputFormat, outputFormat, windowSize, loFreq, hiFreq);
}

void Echo::initEcho(int a_argc, char **a_argv) {
    static QTInitializer init{a_argc, a_argv};
}

void Echo::send(const std::vector<uint8_t> &buffer) {
    auto encoded = converter->encode(buffer);
    const size_t atOnce =
        2600; /* < Temporary - this constant was created empirically. Shortly - it says how much audio data need to be
                 in the pushed to stream buffer every time so there won't be pause in transmission.  */

    output->enqueueData(
        encoded.data(),
        std::min(encoded.size(), atOnce)); /* < At the beggining we enqueue surplus audio data to play to fill out the
                                              buffer so the pause in transmission won't happen. */
    for (size_t i = atOnce; i < encoded.size(); i += atOnce) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        output->enqueueData(encoded.data() + i, std::min(atOnce, encoded.size() - i));
        auto status = output->getStreamStatus();
        qDebug() << "Status: " << status.first << " " << status.second;
        output->waitForTick();
    }

    output->waitForState(QAudio::State::IdleState);
}

static double dft(const char *buffer, int samples, int sampleSize, double ratio) {
    double re = 0;
    double im = 0;
    double angle = 0;
    const double d_angle = 2.0 * M_PI * ratio;

    for (int i = 0; i < samples; i++) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        double val = *reinterpret_cast<const int16_t *>(buffer);
        re += val * std::cos(angle);
        im += val * std::sin(angle);
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        buffer += (sampleSize / CHAR_BIT);
        angle += d_angle;
    }

    return std::sqrt(re * re + im * im) / (1 << sampleSize);
}

void Echo::getbuff(int bytes, char *buffer) {
    int inc = 0;
    while (bytes > 0) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        int nread = input->readBytes(buffer + inc, bytes - inc);
        inc += nread;
        input->waitForTick();
    }
}

void Echo::clearInput() {
    int size = input->getStreamStatus().first;
    std::vector<char> dummy(size);
    input->readBytes(dummy.data(), size);
}

std::vector<uint8_t> Echo::receive() {
    static QAudioFormat inputFormat = AudioFormatFactory::getDefaultInputFormat();

    int sampleRate = inputFormat.sampleRate();
    int sampleSize = inputFormat.sampleSize();
    int samples = windowSize / 2;  // we're dealing with half-windows
    int bytes = samples * (sampleSize / CHAR_BIT);
    std::vector<char> buffer(bytes);

    double loRatio = (double)loFreq / sampleRate;
    double hiRatio = (double)hiFreq / sampleRate;
    double loMag;
    double hiMag;

    clearInput();

    qDebug() << "Listening for" << loFreq << "/" << hiFreq << "Hz";

    // wait for the first half-window and discard it
    do {
        getbuff(bytes, buffer.data());
        loMag = dft(buffer.data(), samples, sampleSize, loRatio);
        hiMag = dft(buffer.data(), samples, sampleSize, hiRatio);
        qDebug() << std::max(loMag, hiMag);
    } while (loMag < magLimit && hiMag < magLimit);

    qDebug() << "Recording";

    std::vector<bool> res_bits;

    // then, starting from the second half-window, read every other one
    while (true) {
        getbuff(bytes, buffer.data());
        loMag = dft(buffer.data(), samples, sampleSize, loRatio);
        hiMag = dft(buffer.data(), samples, sampleSize, hiRatio);
        qDebug() << std::max(loMag, hiMag);
        if (loMag < magLimit && hiMag < magLimit) {
            break;
        }
        res_bits.push_back(loMag < hiMag);
        getbuff(bytes, buffer.data());
    }

    // pad the bit vector with zeroes
    while (res_bits.size() % CHAR_BIT != 0) {
        res_bits.push_back(false);
    }

    std::vector<uint8_t> res_bytes;
    for (int i = 0; i < res_bits.size(); i += CHAR_BIT) {
        uint8_t byte = 0;
        for (int j = 0; j < CHAR_BIT; j++) {
            byte |= (static_cast<int>(res_bits[i + j]) << j);
        }
        res_bytes.push_back(byte);
    }

    return res_bytes;
}
