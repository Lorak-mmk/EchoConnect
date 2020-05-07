#ifndef ECHOCONNECT_IRECEIVER_H
#define ECHOCONNECT_IRECEIVER_H

#include "AudioInput.h"

#include <QtMultimedia/QAudioFormat>
#include <chrono>
#include <cstdint>
#include <memory>

class IReceiver {
public:
    IReceiver(const QAudioFormat &format, int win_size) : format(format), win_size(win_size) {
        input = std::make_unique<AudioInput>(format);
        input->startStream();
    }

    IReceiver(const IReceiver &other) = delete;
    IReceiver(IReceiver &&other) = default;
    IReceiver &operator=(const IReceiver &other) = delete;
    IReceiver &operator=(IReceiver &&other) = default;

    virtual ~IReceiver() {
        input->stopStream();
    }

    virtual void start(std::chrono::duration<double> timeout) = 0;
    virtual int receive(uint8_t *buffer, int size) = 0;

protected:
    QAudioFormat format;
    std::unique_ptr<AudioInput> input;
    int win_size;
};

#endif
