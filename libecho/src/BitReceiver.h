#ifndef ECHOCONNECT_BITRECEIVER_H
#define ECHOCONNECT_BITRECEIVER_H

#include <cstdint>
#include <vector>

#include "AudioInput.h"
#include "IReceiver.h"

class BitReceiver : public IReceiver {
public:
    BitReceiver(int win_size, int lo_freq, int hi_freq, int mag_lim);
    virtual int receiveFirst(uint8_t *buffer, int size) override;
    virtual int receive(uint8_t *buffer, int size) override;
    int getLoFrequency();
    int getHiFrequency();
    void setLoFrequency(int freq);
    void setHiFrequency(int freq);

private:
    void readSamples(int16_t *buffer, int len);
    int stepShift(double *buffer, int size);
    int decodeBit(int16_t *window);
    int receiveFirstTwoBits();
    void clearInput();  // TODO: i don't actually know if this is needed
    static QAudioFormat getInputFormat();

    std::vector<int16_t> window;
    std::vector<int16_t> sync_in;
    std::vector<double> sync_lo_out;
    std::vector<double> sync_hi_out;

    double lo_ratio;
    double hi_ratio;
    int mag_lim;
};

#endif
