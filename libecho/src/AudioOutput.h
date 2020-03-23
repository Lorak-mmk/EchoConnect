#ifndef ECHOCONNECT_AUDIOOUTPUT_H
#define ECHOCONNECT_AUDIOOUTPUT_H

#include "AudioStream.h"

#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioOutput>

class AudioOutput : public AudioStream<QAudioOutput> {
public:
    explicit AudioOutput(const QAudioFormat &format);

    void enqueueData(const char *data, int length);

    StatusType getStreamStatus() override;

    void handleNotify() override;

private:
    void tryWriteData();

    QByteArray buffer{};
};

#endif  // ECHOCONNECT_AUDIOOUTPUT_H
