#include "Echo.h"
#include "AudioFormatFactory.h"
#include "AudioInput.h"
#include "AudioOutput.h"
#include "QTInitializer.h"
#include "BitAudioConverter.h"

#include <QCoreApplication>
#include <QDebug>

const double windowLength = 0.1;
const int loFreq = 19000, hiFreq = 20000;
using SampleType = int16_t;

Echo::Echo() {
    auto inputFormat = AudioFormatFactory::getDefaultInputFormat();
    input = std::make_unique<AudioInput>(inputFormat);
    auto outputFormat = AudioFormatFactory::getDefaultOutputFormat();
    output = std::make_unique<AudioOutput>(outputFormat);
    converter = std::make_unique<BitAudioConverter<SampleType>>(inputFormat, outputFormat, windowLength, loFreq, hiFreq);
}

void Echo::initEcho(int a_argc, char** a_argv) {
    static QTInitializer init(a_argc, a_argv);
}

void Echo::send(const std::vector<uint8_t>& buffer) {
    auto encoded = converter->encode(buffer);
    const size_t atOnce = 260;  // (bitrate / notify_interval) + eps; - TODO: wyjaśnić
    output->enqueueData(encoded.data(), std::min(encoded.size(), atOnce));
    for (int i = atOnce; i < encoded.size(); i += atOnce) {
        output->enqueueData(encoded.data() + i, std::min(atOnce, encoded.size() - i)); //TODO: pozbyć się operacji na pointerach
        output->waitForTick();
    }

    output->waitForState(QAudio::State::IdleState);
}

std::vector<uint8_t> Echo::receive() {
    auto info = input->getStreamInfo();

    const size_t atOnce = info.periodSize;
    char* array = new char[atOnce];
    int idx = 0;
    while (input->getStreamStatus().second < 2000000) {
        int read = input->readBytes(array, atOnce - idx);
        idx = idx + read;
        if (idx == atOnce) {
            idx = 0;
            qDebug() << "Process data here - or better: schedule it so this loop is not blocked";
        }
        input->waitForTick();
    }
    
    return std::vector<uint8_t>();
}
