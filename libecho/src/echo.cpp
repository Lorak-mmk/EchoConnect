#include "echo.h"
#include "AudioOutput.h"
#include "AudioFormatFactory.h"
#include "AudioInput.h"

#include <QCoreApplication>
#include <QDebug>


void echo::send(const std::vector<char>& buffer) {
    static AudioOutput audio(AudioFormatFactory::getDefaultOutputFormat());

    /* Example transmission */
    const size_t atOnce = 260; // (bitrate / notify_interval) + eps;
    audio.enqueueData(buffer.data(), std::min(buffer.size(), atOnce));

    for(int i = atOnce; i <  buffer.size(); i += atOnce) {
        // Push some bytes to sound output
        audio.enqueueData(buffer.data() + i, std::min(atOnce, buffer.size() - i));
        audio.waitForTick();
    }
    /* End example */

    audio.waitForState(QAudio::State::IdleState);
}

std::vector<uint8_t> echo::receive() {
    static AudioInput audio(AudioFormatFactory::getDefaultInputFormat());
    auto info = audio.getStreamInfo();

    const size_t atOnce = info.periodSize;
    char *array = new char[atOnce];
    int idx = 0;
    while(audio.getStreamStatus().second < 2000000) {
        int read = audio.readBytes(array, atOnce - idx);
        idx = idx + read;
        if(idx == atOnce) {
            idx = 0;
            qDebug() << "Process data here - or better: schedule it so this loop is not blocked";
        }
        audio.waitForTick();
    }

    return std::vector<uint8_t>();
}
