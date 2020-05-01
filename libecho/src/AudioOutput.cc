#include "AudioOutput.h"

constexpr int NOTIFY_INTERVAL = 32;
constexpr double VOLUME = 1.0;

AudioOutput::AudioOutput(const QAudioFormat &format) : AudioStream(format) {
    qStream->setNotifyInterval(NOTIFY_INTERVAL);
    qStream->setVolume(VOLUME);
}

void AudioOutput::enqueueData(const char *data, int length, bool waitForStart) {
    qDebug() << qUtf8Printable(type(this)) << ": queueing" << length << "bytes to play";
    std::lock_guard<std::mutex> lock(mutex);
    bool is_runnig = qStream->state() == QAudio::ActiveState;
    buffer.append(data, length);
    tryWriteData();
    if(!is_runnig && waitForStart) {
        waitForState(QAudio::ActiveState);
    }
}

void AudioOutput::tryWriteData() {
    size_t bytesToWrite = std::min(qStream->bytesFree(), buffer.size());
    if (bytesToWrite == 0) {
        return;
    }
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
