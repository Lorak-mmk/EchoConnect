#include "AudioInput.h"

#include <QtCore/QEventLoop>


AudioInput::AudioInput(const QAudioFormat &format) : AudioStream(format) {}

uint64_t AudioInput::readBytes(char *data, qint64 maxSize) {
    return qDevice->read(data, maxSize);
}

void AudioInput::handleNotify() {}

AudioInput::StatusType AudioInput::getStreamStatus() {
    return std::make_pair(qStream->bytesReady(), qStream->processedUSecs());
}
