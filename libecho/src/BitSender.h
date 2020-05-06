#ifndef ECHOCONNECT_BIT_SENDER_H
#define ECHOCONNECT_BIT_SENDER_H

#include "ISender.h"

using SampleType = uint8_t;
/**< Type in which we keep audio samples. */

/**
 * @brief Class which performs sends data through audio by encoding each bit separately.
 *
 * We specify two frequencies in which we encode every possible value of a bit,
 * respectively loFreq for 0 value and hiFreq for 1 value.
 * Then we send each bit in winSize long time window.
 */
class BitSender : public ISender {
public:
    /**
     * @brief Constructs a BitSender instance.
     *
     * Following parameters are mostly used to determine how data should be encoded/decoded.
     * @param winSize   Look at ISender::ISender.
     * @param lo        Frequency in which we encode bits of value 0.
     * @param hi        Frequency in which we encode bits of value 1.
     */
    BitSender(int winSize, int lo, int hi) : ISender(getOutputFormat(), winSize), loFreq(lo), hiFreq(hi) {}

    /**
     * @brief loFreq member getter.
     */
    int getLoFreq() {
        return loFreq;
    }

    /**
     * @brief hiFreq member getter.
     */
    int getHiFreq() {
        return hiFreq;
    }

    /**
     * @brief loFreq member setter.
     *
     * @param newFreq   New loFreq value.
     */
    void setLoFreq(int newFreq) {
        loFreq = newFreq;
    }

    /**
     * @brief hiFreq member setter.
     *
     * @param newFreq   New hiFreq value.
     */
    void setHiFreq(int newFreq) {
        hiFreq = newFreq;
    }

    virtual void start() override;
    virtual void send(uint8_t *buffer, int size) override;
    virtual void wait() override;

private:
    int loFreq, hiFreq; /**< Frequencies used to encode/decode bits, respectively for 0 and 1 values. */

    /**
     * @brief Encodes data to send into audio format.
     *
     * @param data    Bytes to encode.
     */
    std::vector<char> encode(const std::vector<uint8_t> &data);

    /**
     * @brief Encodes byte into audio format (speakers membrane amplitude) by encoding every single bit.
     *
     * Every bit is encoded as one sound lasting for winSize / format.sampleRate seconds in one of two
     * specified frequencies.
     * Based on <a href="https://en.wikipedia.org/wiki/Pulse-code_modulation">pcm/audio</a> encoding.
     */
    std::vector<SampleType> encodeByte(uint8_t data);

    /**
     * @brief Used for sampling value of audio wave with passed @p frequency at @p time * π / sampleRate position.
     */
    SampleType audioWave(int frequency, int time);

    /**
     * @brief Creates instance of QAudioFormat with with predefined setting. Parameters are set in .cc file.
     */
    static QAudioFormat getOutputFormat();
};

#endif  // ECHOCONNECT_BIT_SENDER_H
