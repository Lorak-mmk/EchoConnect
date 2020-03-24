#include "AudioOutput.h"

AudioOutput::AudioOutput(const QAudioFormat &format) : AudioStream(format) {}

void AudioOutput::enqueueData(const char *data, int length) {
    qDebug() << "Queueing" << length << "bytes to play";
    std::lock_guard<std::mutex> lock(mutex);
    buffer.append(data, length);
    tryWriteData();
}

void AudioOutput::tryWriteData() {
    size_t bytesToWrite = std::min(qStream->bytesFree(), buffer.size());
    if (bytesToWrite == 0) {
        return;
    }
    qDebug() << "Pushing" << bytesToWrite << "bytes to output audio buffer";
    qDevice->write(buffer, bytesToWrite);
    buffer.remove(0, bytesToWrite);
}

void AudioOutput::handleNotify() {
    std::lock_guard<std::mutex> lock(mutex);
    tryWriteData();
}

AudioOutput::StatusType AudioOutput::getStreamStatus() {
    return std::make_pair(qStream->bytesFree(), qStream->processedUSecs());
}
