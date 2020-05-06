#ifndef ECHOCONNECT_BITRECEIVER_H
#define ECHOCONNECT_BITRECEIVER_H

#include <cstdint>
#include <vector>

#include "AudioInput.h"
#include "IReceiver.h"

class BitReceiverv2 : public IReceiver {
public:
    BitReceiverv2(int win_size, int freq, double lim);
    ~BitReceiverv2();
    void start() override;
    int receive(uint8_t *buffer, int size) override;

    uint8_t *dbg = NULL;

private:
    void fetch(int len);
    uint8_t read_bit();
    static QAudioFormat getInputFormat();

    int16_t *dwindow;
    double *mags;

    double freq_ratio = 0;
    double lim = 0;
};

#endif
