#include "AudioInput.h"

constexpr int NOTIFY_INTERVAL = 1;
constexpr double VOLUME = 1.0;

AudioInput::AudioInput(const QAudioFormat &format) : AudioStream(format) {
    qStream->setNotifyInterval(NOTIFY_INTERVAL);
    qStream->setVolume(VOLUME);
}

qint64 AudioInput::readBytes(char *data, qint64 maxSize) {
    return qDevice->read(data, maxSize);
}

void AudioInput::handleNotify() {}

AudioInput::StatusType AudioInput::getStreamStatus() {
    return std::make_pair(qStream->bytesReady(), qStream->processedUSecs());
}
