#ifndef ECHOCONNECT_BITRECEIVER_H
#define ECHOCONNECT_BITRECEIVER_H

#include <cstdint>
#include <vector>

#include "AudioInput.h"
#include "IReceiver.h"

class BitReceiver : public IReceiver {
public:
    BitReceiver(int lo_freq, int hi_freq, int win_size, int mag_lim);
    ~BitReceiver();
    virtual int receiveFirst(uint8_t *buffer, int size) override;
    virtual int receive(uint8_t *buffer, int size) override;

private:
    void readSamples(int16_t *buffer, int len);
    int stepShift(double *buffer, int size);
    int decodeBit(int16_t *window);
    int receiveFirstTwoBits();
    void clearInput();  // TODO: i don't actually know if this is needed

    std::vector<int16_t> window;
    std::vector<int16_t> sync_in;
    std::vector<double> sync_lo_out;
    std::vector<double> sync_hi_out;

    double lo_ratio;
    double hi_ratio;
    int win_size;
    int mag_lim;
};

#endif
