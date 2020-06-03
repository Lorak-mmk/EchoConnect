#ifndef ECHOCONNECT_IRECEIVER_H
#define ECHOCONNECT_IRECEIVER_H

#include "AudioInput.h"

#include <QtMultimedia/QAudioFormat>
#include <chrono>
#include <cstdint>
#include <memory>

/**
 * @brief Interface for classes receiving data through audio in specific way.
 */
class IReceiver {
public:
    /**
     * @brief Generic constructor.
     *
     * Derived classes must call it.
     *
     * @param format   Audio stream input format.
     * @param win_size Number of samples used for one bit.
     */
    IReceiver(const QAudioFormat &format, int win_size) : format(format), win_size(win_size) {
        input = std::make_unique<AudioInput>(format);
        input->startStream();
    }

    IReceiver(const IReceiver &other) = delete;
    IReceiver(IReceiver &&other) = default;
    IReceiver &operator=(const IReceiver &other) = delete;
    IReceiver &operator=(IReceiver &&other) = default;

    /**
     * @brief Default object destructor.
     */
    virtual ~IReceiver() {
        input->stopStream();
    }

    /**
     * @brief Starts a transmission, possibly by receiving a header.
     */
    virtual void start(std::chrono::duration<double> timeout) = 0;

    /**
     * @brief Receive data, can be called multiple times in a row.
     *
     * @param buffer  Pointer to received data.
     * @param size    Length of the buffer.
     */
    virtual int receive(uint8_t *buffer, int size) = 0;

protected:
    QAudioFormat format;               /**< Audio stream output format. */
    std::unique_ptr<AudioInput> input; /**< Input audio device. */
    int win_size;                      /**< Number of samples used for one bit. */
};

#endif
