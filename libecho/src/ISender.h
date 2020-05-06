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
     * @param win_size   How much of bitrate we want to use to play sound in which unit data is encoded.
     */
    ISender(const QAudioFormat &format, int win_size) : format(format), win_size(win_size) {
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
     * @brief win_size member getter.
     */
    [[nodiscard]] int getWindowSize() const {
        return win_size;
    }

    /**
     * @brief win_size member setter.
     *
     * @param newSize   New win_size value.
     */
    void setWindowSize(int newSize) {
        win_size = newSize;
    }

    virtual void start() = 0;
    virtual void send(uint8_t *buffer, int size) = 0;
    virtual void wait() = 0;

protected:
    const QAudioFormat format;           /**< Audio stream output format. */
    std::unique_ptr<AudioOutput> output; /**< Output audio device. */
    int win_size;                        /**< Says how much of bitrate we want to use
                                             to play sound in which unit od data is encoded. */
};

#endif  // ECHOCONNECT_I_SENDER_H
