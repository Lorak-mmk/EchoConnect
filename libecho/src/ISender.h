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
     * @param format        Audio stream output format.
     * @param windowSize    How much of bitrate we want to use to play sound in which unit data is encoded.
     */
    ISender(QAudioFormat format, int windowSize) : format(format), windowSize(windowSize) {
        output = std::make_unique<AudioOutput>(format);
    }

    /**
     * @brief Default object destructor.
     */
    virtual ~ISender() {}

    /**
     * @brief windowSize member getter.
     */
    int getWindowSize() {
        return windowSize;
    }

    /**
     * @brief windowSize member setter.
     *
     * @param newSize   New windowSize value.
     */
    void setWindowSize(int newSize) {
        windowSize = newSize;
    }

    /**
     * @brief Sends given byte sequence.
     *
     * Anyone can receive it, and there is no guarantee that received transmission will be correct.
     * @param buffer    Bytes to send.
     */
    virtual void send(const std::vector<uint8_t> &buffer) = 0;

protected:
    std::unique_ptr<AudioOutput> output; /**< Output audio device. */
    const QAudioFormat format;           /**< Audio stream output format. */
    int windowSize;                      /**< Says how much of bitrate we want to use
                                              to play sound in which unit od data is encoded. */
};

#endif  // ECHOCONNECT_I_SENDER_H
