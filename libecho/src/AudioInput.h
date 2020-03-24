#ifndef ECHOCONNECT_AUDIOINPUT_H
#define ECHOCONNECT_AUDIOINPUT_H

#include "AudioStream.h"

#include <QtMultimedia/QAudioInput>

class AudioInput : public AudioStream<QAudioInput> {
public:
    explicit AudioInput(const QAudioFormat &format);

    StatusType getStreamStatus() override;

    void handleNotify() override;

    qint64 readBytes(char *data, qint64 maxSize);
};

#endif  // ECHOCONNECT_AUDIOINPUT_H
