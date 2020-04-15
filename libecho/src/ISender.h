#ifndef ECHOCONNECT_I_SENDER_H
#define ECHOCONNECT_I_SENDER_H

#include "AudioOutput.h"

#include <QtMultimedia/QAudioFormat>
#include <cstdint>
#include <memory>
#include <vector>

/**
 * @brief Interface for classes sending data through audio in specific way.
 */
class ISender {
public:
    /**
     * @brief Generic constructor.
     *
     * Derived classes must call it.
     *
     * @param format    Audio stream output format.
     * @param winSize   How much of bitrate we want to use to play sound in which unit data is encoded.
     */
    ISender(const QAudioFormat &format, int winSize) : format(format), winSize(winSize) {
        output = std::make_unique<AudioOutput>(format);
        output->startStream();
    }

    ISender(const ISender &other) = delete;
    ISender(ISender &&other) = default;
    ISender &operator=(const ISender &other) = delete;
    ISender &operator=(ISender &&other) = delete;

    /**
     * @brief Default object destructor.
     */
    virtual ~ISender() {
        output->stopStream();
    }

    /**
     * @brief winSize member getter.
     */
    int getWindowSize() {
        return winSize;
    }

    /**
     * @brief winSize member setter.
     *
     * @param newSize   New winSize value.
     */
    void setWindowSize(int newSize) {
        winSize = newSize;
    }

    /**
     * @brief Sends given byte sequence.
     *
     * Anyone can receive it, and there is no guarantee that received transmission will be correct.
     * @param buffer    Bytes to send.
     */
    virtual void send(const std::vector<uint8_t> &buffer) = 0;

    /**
     * @brief Sends given byte sequence and blocks untill sending finishes.
     *
     * Anyone can receive it, and there is no guarantee that received transmission will be correct.
     * @param buffer    Bytes to send.
     */
    virtual void sendBlocking(const std::vector<uint8_t> &buffer) = 0;

protected:
    const QAudioFormat format;           /**< Audio stream output format. */
    std::unique_ptr<AudioOutput> output; /**< Output audio device. */
    int winSize;                         /**< Says how much of bitrate we want to use
                                              to play sound in which unit od data is encoded. */
};

#endif  // ECHOCONNECT_I_SENDER_H
