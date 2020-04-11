#ifndef ECHOCONNECT_I_SENDER_H
#define ECHOCONNECT_I_SENDER_H

#include "AudioOutput.h"

#include <QDebug>
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
    void send(const std::vector<uint8_t> &buffer) {
        auto encoded = encode(buffer);
        // TODO: try to make atOnce smaller
        const size_t atOnce = 2600; /* < Temporary - this constant was created empirically.
                                        Shortly - it says how much audio data need to be pushed to stream buffer
                                        every time so there won't be pause in transmission. */
        output->startStream();
        /* At the beggining we enqueue surplus audio data to play
        to fill out the buffer so the pause in transmission won't happen. */
        output->enqueueData(encoded.data(), std::min(encoded.size(), atOnce));
        for (size_t i = atOnce; i < encoded.size(); i += atOnce) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            output->enqueueData(encoded.data() + i, std::min(atOnce, encoded.size() - i));
            auto status = output->getStreamStatus();
            qDebug() << "Status: " << status.first << " " << status.second;
            output->waitForTick();
        }

        output->waitForState(QAudio::State::IdleState);
        output->stopStream();
    }

protected:
    std::unique_ptr<AudioOutput> output; /**< Output audio device. */
    const QAudioFormat format;           /**< Audio stream output format. */
    int windowSize;                      /**< Says how much of bitrate we want to use
                                              to play sound in which unit od data is encoded. */

private:
    /**
     * @brief Encodes data to send into audio format.
     *
     * @param data    Bytes to encode.
     */
    virtual std::vector<char> encode(const std::vector<uint8_t> &data) = 0;
};

#endif  // ECHOCONNECT_I_SENDER_H
