#ifndef ECHOCONNECT_BITRECEIVERV2_H
#define ECHOCONNECT_BITRECEIVERV2_H

#include <cstdint>
#include <vector>

#include "AudioInput.h"
#include "IReceiver.h"

// NOLINTNEXTLINE
class BitReceiverv2 : public IReceiver {
public:
    /**
     * @brief Constructor
     * @param win_size Number of samples per bit
     * @param freq     The frequency of transmission
     * @param lim      The initial limit between a 0 and a 1
     */
    BitReceiverv2(int win_size, int freq, double lim);

    /**
     * @brief Destructor
     */
    ~BitReceiverv2() override;

    /**
     * @brief see IReceiver
     */
    void start(std::chrono::duration<double> timeout) override;

    /**
     * @brief see IReceiver
     */
    int receive(uint8_t *buffer, int size) override;

    uint8_t *dbg = nullptr;

private:
    void fetch(int len);
    uint8_t read_bit();
    static QAudioFormat getInputFormat();

    int16_t *dwindow;
    double *mags;

    double freq_ratio = 0;
    double lim = 0;

    uint8_t prev_bit = 0;
    double low_mag = 0;
    double high_mag = 0;
};

#endif
