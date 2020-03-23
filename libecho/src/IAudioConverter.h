#ifndef ECHOCONNECT_I_AUDIO_CONVERTER_H
#define ECHOCONNECT_I_AUDIO_CONVERTER_H

/**
 * @brief Interface for classes which converts between audio and data.
 */

#include <vector>

template <typename T>
class IAudioConverter {
public:
    IAudioConverter(QAudioFormat inputFormat, QAudioFormat outputFormat, double windowLength)
        : inputFormat(inputFormat), outputFormat(outputFormat), windowLength(windowLength){};

    std::vector<char> encode(std::vector<uint8_t> data) {
        std::vector<T> result;
        for (auto byte : data) {
            auto encoded_byte = encode_byte(byte);
            result.insert(result.end(), encoded_byte.begin(), encoded_byte.end());
        }
        return std::vector<char>(result.data(), result.data() + result.size());
    }

    std::vector<uint8_t> decode(std::vector<char> data);

protected:
    QAudioFormat const inputFormat, outputFormat;
    double const windowLength;

private:
    virtual std::vector<T> encode_byte(uint8_t data) = 0;
    virtual uint8_t decode_byte(std::vector<char>) = 0;
};

#endif  // ECHOCONNECT_I_AUDIO_CONVERTER_H
