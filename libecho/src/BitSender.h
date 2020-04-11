#ifndef ECHOCONNECT_BIT_SENDER_H
#define ECHOCONNECT_BIT_SENDER_H

#include "ISender.h"

using SampleType = int8_t;
/**< Type in which we keep audio samples. */

static constexpr int DEFAULT_LO_FREQ = 19000, DEFAULT_HI_FREQ = 20000;
/**< Default frequencies used to encode/decode bits, respectively for 0 and 1 values. */

/**
 * @brief Class which performs sends data through audio by encoding each bit separately.
 *
 * We specify two frequencies in which we encode every possible value of a bit
 * - respectively loFreq for 0 value and hiFreq for 1 value.
 * Then we send each bit in windowSize long time window.
 */
class BitSender : public ISender {
public:
    /**
     * @brief Constructs a BitSender instance.
     *
     * Following parameters are mostly used to determine how data should be encoded/decoded.
     * @param windowSize  Look at ISender::ISender.
     * @param lo    Frequency in which we encode bits of value 0.
     * @param hi    Frequency in which we encode bits of value 1.
     */
    BitSender(int windowSize, int lo = DEFAULT_LO_FREQ, int hi = DEFAULT_LO_FREQ);

private:
    const int loFreq, hiFreq; /**< Frequencies used to encode/decode bits, respectively for 0 and 1 values. */

    /**
     * @brief @see ISender::encode
     */
    std::vector<char> encode(const std::vector<uint8_t> &data) override;

    /**
     * @brief Encodes byte into audio format (speakers membrane amplitude) by encoding every single bit.
     *
     * Every bit is encoded as one sound lasting for windowSize / outputFormat.sampleRate seconds in one of two
     * specified frequencies.
     * Based on <a href="https://en.wikipedia.org/wiki/Pulse-code_modulation">pcm/audio</a> encoding.
     */
    std::vector<SampleType> encode_byte(uint8_t data) override;

    /**
     * @brief Used for sampling value of audio wave with passed @p frequency at @p time * π / sampleRate position.
     */
    SampleType audio_wave(int frequency, int time);

    /**
     * @brief Creates instance of QAudioFormat with with predefined setting. Parameters are set in .cc file.
     */
    QAudioFormat getOutputFormat();
};

#endif  // ECHOCONNECT_BIT_SENDER_H
