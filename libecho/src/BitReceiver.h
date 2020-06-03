#ifndef ECHOCONNECT_BITRECEIVER_H
#define ECHOCONNECT_BITRECEIVER_H

#include <cstdint>
#include <vector>

#include "AudioInput.h"
#include "IReceiver.h"

class BitReceiver : public IReceiver {
public:
    /**
     * @brief Constructor
     * @param win_size Number of samples per bit
     * @param lo_freq  The frequency corresponding to bit 0
     * @param hi_freq  The frequency corresponding to bit 1
     * @param left_lim Silence threshold - if the signal drops below this value we assume the sender has died
     * @param right_lim Sound threshold - We start listening once we receive something louder than this
     */
    BitReceiver(int win_size, int lo_freq, int hi_freq, int left_lim, int right_lim);

    /**
     * @brief see IReceiver
     */
    void start(std::chrono::duration<double> timeout) override;
    /**
     * @brief see IReceiver
     */
    int receive(uint8_t *buffer, int size) override;

    [[nodiscard]] long getLoFrequency() const;
    [[nodiscard]] long getHiFrequency() const;

    void setLoFrequency(int freq);
    void setHiFrequency(int freq);

private:
    void readSamples(int16_t *buffer, int len);
    int stepShift(const double *buffer, int size) const;
    int decodeBit(int16_t *windowBuffer);
    int receiveFirstTwoBits();
    void receiveBits(std::vector<bool> &vec, int offset);
    void clearInput();  // TODO: i don't actually know if this is needed
    static QAudioFormat getInputFormat();

    std::vector<int16_t> window;
    std::vector<int16_t> sync_in;
    std::vector<double> sync_lo_out;
    std::vector<double> sync_hi_out;

    double lo_ratio = 0;
    double hi_ratio = 0;
    int left_lim = 0;
    int right_lim = 0;
    int first_two_bits = -1;
};

#endif
