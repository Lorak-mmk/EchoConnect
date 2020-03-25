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
static constexpr int magLimit = 500000 / windowSize;

using SampleType = int8_t;

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
    const size_t atOnce = 260;  // (bitrate / notify_interval) + eps; - TODO: wyjaśnić

    output->enqueueData(encoded.data(), std::min(encoded.size(), atOnce));
    for (int i = atOnce; i < encoded.size(); i += atOnce) {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        output->enqueueData(encoded.data() + i, std::min(atOnce, encoded.size() - i));
        auto status = output->getStreamStatus();
        qDebug() << "Status: " << status.first << " " << status.second;
        // TODO: i changed something and this line now freezes the playback after a short time
        // output->waitForTick();
    }

    output->waitForState(QAudio::State::IdleState);
}

static double dft(const char *buffer, int samples, int sampleSize, double ratio) {
    double re = 0;
    double im = 0;
    double angle = 0;
    const double d_angle = 2.0 * M_PI * ratio;
    const int mask = (1 << sampleSize) - 1;

    for (int i = 0; i < samples; i++) {
        double val = *((int *) buffer) & mask;
        re += val * cos(angle);
        im += val * sin(angle);
        buffer += (sampleSize / 8);
        angle += d_angle;
    }

    return sqrt(re * re + im * im);
}

// TODO: make it not use 100%cpu
void getbuff(AudioInput &audio, int bytes, char *buffer) {
    while (bytes > 0) {
        int nread = audio.readBytes(buffer, bytes);
        buffer += nread;
        bytes -= nread;
    }
}

std::vector<uint8_t> Echo::receive() {
    static QAudioFormat inputFormat = AudioFormatFactory::getDefaultInputFormat();
    static AudioInput audio(inputFormat);

    int sampleRate = inputFormat.sampleRate();
    int sampleSize = inputFormat.sampleSize();
    int samples = windowSize / 2; // we're dealing with half-windows
    int bytes = samples * (sampleSize / 8);
    char *buffer = new char[bytes];

    double loRatio = (double)loFreq / sampleRate;
    double hiRatio = (double)hiFreq / sampleRate;
    double loMag;
    double hiMag;

    qDebug() << "Listening for" << loFreq << "/" << hiFreq << "Hz";

    // wait for the first half-window and discard it
    do {
        getbuff(audio, bytes, buffer);
        loMag = dft(buffer, samples, sampleSize, loRatio);
        hiMag = dft(buffer, samples, sampleSize, hiRatio);
    } while (loMag < magLimit && hiMag < magLimit);

    qDebug() << "Recording";

    std::vector<bool> res_bits;

    // then, starting from the second half-window, read every other one
    while (true) {
        getbuff(audio, bytes, buffer);
        loMag = dft(buffer, samples, sampleSize, loRatio);
        hiMag = dft(buffer, samples, sampleSize, hiRatio);
        if (loMag < magLimit && hiMag < magLimit) break;
        res_bits.push_back(loMag < hiMag);
        getbuff(audio, bytes, buffer);
    }

    delete[] buffer;

    // pad the bit vector with zeroes
    while (res_bits.size() % 8 != 0)
        res_bits.push_back(false);

    std::vector<uint8_t> res_bytes;
    for (int i = 0; i < res_bits.size(); i += 8) {
        uint8_t byte = 0;
        for (int j = 0; j < 8; j++) {
            byte |= (static_cast<int>(res_bits[i + j]) << j);
        }
        res_bytes.push_back(byte);
    }

    return res_bytes;
}
