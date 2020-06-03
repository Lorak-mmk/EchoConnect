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
     * @param win_size  Number of samples used for a single bit.
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

    /**
     * @brief Starts a transmission, possibly by sending a header.
     */
    virtual void start() = 0;

    /**
     * @brief Send data asynchronously, can be called multiple times in a row.
     *
     * @param buffer  Pointer to the data to be sent.
     * @param size    Length of the data.
     */
    virtual void send(uint8_t *buffer, int size) = 0;

    /**
     * @brief Waits for all data transfers to end.
     */
    virtual void wait() = 0;

protected:
    const QAudioFormat format;           /**< Audio stream output format. */
    std::unique_ptr<AudioOutput> output; /**< Output audio device. */
    int win_size;                        /**< Number of samples used for one bit. */
};

#endif  // ECHOCONNECT_I_SENDER_H
