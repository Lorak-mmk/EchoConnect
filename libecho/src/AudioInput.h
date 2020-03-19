#ifndef ECHOCONNECT_AUDIOINPUT_H
#define ECHOCONNECT_AUDIOINPUT_H

#include <QtMultimedia/QAudioOutput>
#include "AudioStream.h"

class AudioInput : public AudioStream<QAudioInput> {
    Q_OBJECT
public:
    explicit AudioInput(const QAudioFormat& format);

    StatusType getStreamStatus() override;

    void handleNotify() override;

    uint64_t readBytes(char *data, qint64 maxSize);
};


#endif  // ECHOCONNECT_AUDIOINPUT_H
