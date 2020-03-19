#ifndef ECHOCONNECT_AUDIOOUTPUT_H
#define ECHOCONNECT_AUDIOOUTPUT_H

#include <QtCore/QThread>
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioOutput>
#include "AudioStream.h"


class AudioOutput : public AudioStream<QAudioOutput> {
    Q_OBJECT
public:
    explicit AudioOutput(const QAudioFormat& format);

    void enqueueData(const char* data, size_t length);

    StatusType getStreamStatus() override;

    void handleNotify() override;

private:
    void tryWriteData();

    QByteArray buffer{};
};

#endif  // ECHOCONNECT_AUDIOOUTPUT_H
