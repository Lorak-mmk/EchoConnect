#ifndef ECHOCONNECT_BIT_SENDERV2_H
#define ECHOCONNECT_BIT_SENDERV2_H

#include "ISender.h"

/**< Type in which we keep audio samples. */

/**
 * @brief Class which performs sends data through audio by encoding each bit separately.
 *
 * We specify two frequencies in which we encode every possible value of a bit,
 * respectively loFreq for 0 value and hiFreq for 1 value.
 * Then we send each bit in winSize long time window.
 */
class BitSenderv2 : public ISender {
public:
    /**
     * @brief Constructs a BitSenderv2 instance.
     *
     * Following parameters are mostly used to determine how data should be encoded/decoded.
     * @param winSize   Look at ISender::ISender.
     * @param lo        Frequency in which we encode bits of value 0.
     * @param hi        Frequency in which we encode bits of value 1.
     */
    BitSenderv2(int win_size, int freq) : ISender(getOutputFormat(), win_size), freq(freq) {}

    /**
     * @brief @see ISender::send
     */
    void start() override;
    void send(uint8_t *buffer, int size) override;
    void wait() override;

private:
    int counter = 0;
    double amp = 0;
    int freq;

    void write_bit(int16_t *out, uint8_t bit);
    /**
     * @brief Creates instance of QAudioFormat with with predefined setting. Parameters are set in .cc file.
     */
    static QAudioFormat getOutputFormat();
};

#endif  // ECHOCONNECT_BIT_SENDER_H
