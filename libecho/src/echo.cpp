#include "echo.h"
#include "AudioFormatFactory.h"
#include "AudioInput.h"
#include "AudioOutput.h"
#include "QTInitializer.h"

void echo::send(const std::vector<uint8_t> &buffer) {
    static AudioOutput audio(AudioFormatFactory::getDefaultOutputFormat());

    /* Example transmission */
    const size_t atOnce = 260;  // (bitrate / notify_interval) + eps;
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    audio.enqueueData(reinterpret_cast<const char *>(buffer.data()), std::min(buffer.size(), atOnce));

    for (int i = atOnce; i < buffer.size(); i += atOnce) {
        // Push some bytes to sound output
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast, cppcoreguidelines-pro-bounds-pointer-arithmetic)
        audio.enqueueData(reinterpret_cast<const char *>(buffer.data() + i), std::min(atOnce, buffer.size() - i));
        auto status = audio.getStreamStatus();
        qDebug() << "Status: " << status.first << " " << status.second;
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
    const int time = 2000000;
    while (audio.getStreamStatus().second < time) {
        int read = audio.readBytes(array, atOnce - idx);
        idx = idx + read;
        if (idx == atOnce) {
            idx = 0;
            qDebug() << "Process data here - or better: schedule it so this loop is not blocked";
        }
        audio.waitForTick();
    }

    delete[] array;

    return std::vector<uint8_t>();
}

void echo::initEcho(int a_argc, char **a_argv) {
    static QTInitializer init{a_argc, a_argv};
}
