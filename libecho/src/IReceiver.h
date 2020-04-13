#ifndef ECHOCONNECT_IRECEIVER_H
#define ECHOCONNECT_IRECEIVER_H

#include <QtMultimedia/QAudioFormat>
#include <memory>

class IReceiver {
public:
    virtual int receiveFirst(uint8_t *buffer, int size) = 0;
    virtual int receive(uint8_t *buffer, int size) = 0;
    virtual ~IReceiver() = default;

protected:
    QAudioFormat format;
    std::unique_ptr<AudioInput> input;
};

#endif
