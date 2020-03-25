#ifndef ECHOCONNECT_AUDIOINPUT_H
#define ECHOCONNECT_AUDIOINPUT_H

#include "AudioStream.h"

#include <QtMultimedia/QAudioInput>

/**
 * TODO
 */
class AudioInput : public AudioStream<QAudioInput> {
public:
    /**
     * @brief Constructs AudioInput object - AudioStream with QAudioInput as internal stream.
     *
     * @param format    Parameters for stream, passed to QAudioOutput.
     */
    explicit AudioInput(const QAudioFormat &format);

    StatusType getStreamStatus() override;

    void handleNotify() override;

    /**
     * @brief Writes data from input buffer to @p data.
     *
     * @param data Pointer to buffer where data will be written.
     * @param maxSize Max amount of bytes to write.
     * @return Amount of bytes written. Can be lower than @p maxSize if there is not enough available bytes in buffer.
     * Amount of available bytes can be checked with AudioStream::getStreamStatus()
     */
    qint64 readBytes(char *data, qint64 maxSize);
};

#endif  // ECHOCONNECT_AUDIOINPUT_H
