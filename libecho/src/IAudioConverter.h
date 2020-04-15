#ifndef ECHOCONNECT_I_AUDIO_CONVERTER_H
#define ECHOCONNECT_I_AUDIO_CONVERTER_H

#include <vector>

/**
 * @brief Interface for classes which performs conversion between data we want to send and audio input/output.
 */
template <typename T>
class IAudioConverter {
public:
    /**
     * @brief Used to simplify constructors for derived classes.
     *
     * Following parameters are mostly used to determine how data should be encoded/decoded.
     * @param inputFormat   Audio stream input format.
     * @param outputFormat  Audio stream output format.
     * @param windowSize  How much of bitrate we want to use to play sound in which unit od data is encoded.
     * @param soundsPerByte How many sounds (units of encoded data) is needed to send a byte - useful when improving
     * performance.
     */
    IAudioConverter(const QAudioFormat &inputFormat, const QAudioFormat &outputFormat, int windowSize,
                    size_t soundsPerByte)
        : inputFormat(inputFormat), outputFormat(outputFormat), windowSize(windowSize) {
        encryptedByteSize = soundsPerByte * windowSize;
    }

    /**
     * @brief Default object destructor.
     */
    virtual ~IAudioConverter() = default;

    /**
     * @brief Encodes data to send into audio format.
     */
    virtual std::vector<char> encode(const std::vector<uint8_t> &data) {
        std::vector<T> result;
        result.reserve(data.size() * encryptedByteSize);

        for (auto byte : data) {
            auto encoded_byte = encode_byte(byte);
            result.insert(result.end(), encoded_byte.begin(), encoded_byte.end());
        }

        return std::vector<char>(result.data(), result.data() + result.size());
    }

    /**
     * @brief Decodes audio into primary data form.
     */
    virtual std::vector<uint8_t> decode(const std::vector<char> &data) = 0;

protected:
    size_t encryptedByteSize;        /**< Says how much space of type T we need to encode one byte of data. */
    const QAudioFormat inputFormat;  /**< Audio stream output format. */
    const QAudioFormat outputFormat; /**< Audio stream output format. */
    const int windowSize;            /**< Says how much of bitrate we want to use
                                          to play sound in which unit od data is encoded. */

private:
    /**
     * @brief Encodes one byte to send into audio format.
     */
    virtual std::vector<T> encode_byte(uint8_t data) = 0;

    /**
     * @brief Decodes audio into primary byte which was send.
     */
    virtual uint8_t decode_byte(const std::vector<char> &data) = 0;
};

#endif  // ECHOCONNECT_I_AUDIO_CONVERTER_H
