#ifndef ECHOCONNECT_AUDIOOUTPUT_H
#define ECHOCONNECT_AUDIOOUTPUT_H

#include "AudioStream.h"

#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioOutput>

/**
 * TODO
 */
class AudioOutput : public AudioStream<QAudioOutput> {
public:
    /**
     * @brief Constructs AudioOutput object - AudioStream with QAudioOutput as internal stream.
     *
     * @param format    Parameters for stream, passed to QAudioOutput.
     */
    explicit AudioOutput(const QAudioFormat &format);

    /**
     * @brief Appends data to temporary buffer. Tries to push data from temporary buffer to output buffer.
     *
     * @param data  Byte array with data to play.
     * @param length    Length of @p data.
     */
    void enqueueData(const char *data, int length);

    StatusType getStreamStatus() override;

    void handleNotify() override;

private:
    /**
     * @brief Tries to push data from temporary buffer to output buffer.
     */
    void tryWriteData();

    /**
     * @brief Temporary buffer to allow pushing more data than normal buffer allows.
     *
     * Data from it will be pushed to normal buffer on every notify() signal and after calling enqueueData.
     */
    QByteArray buffer{};
};

#endif  // ECHOCONNECT_AUDIOOUTPUT_H
