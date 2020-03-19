#include "echo.h"
#include "AudioOutput.h"

#include <QCoreApplication>
#include <QDebug>


void echo::send(const std::vector<char>& buffer) {
    static AudioOutput audio;
    audio.startAudio();

    const size_t atOnce = 260; // (bitrate / notify_interval) + eps;
    audio.enqueueData(buffer.data(), std::min(buffer.size(), atOnce));

    for(int i = atOnce; i <  buffer.size(); i += atOnce) {
        audio.enqueueData(buffer.data() + i, std::min(atOnce, buffer.size() - i));
        audio.waitForTick();
    }

    audio.stopAudio();
}

std::vector<uint8_t> echo::receive() {
    std::vector<uint8_t> result;
    const size_t LENGTH = 40;
    for (size_t i = 0; i < LENGTH; i++) {
        result.push_back(static_cast<uint8_t>('0' + i));
    }
    return result;
}
