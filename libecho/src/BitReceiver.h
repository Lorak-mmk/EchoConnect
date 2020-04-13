#ifndef BITRECEIVER_H
#define BITRECEIVER_H

#include <QtMultimedia/QAudioFormat>
#include <memory>
#include "AudioInput.h"
#include "IReceiver.h"

class BitReceiver : public IReceiver {
public:
    BitReceiver(int lo_freq, int hi_freq, int win_size, int mag_lim);
    int receiveFirst(uint8_t *buffer, int size);
    int receive(uint8_t *buffer, int size);

private:
    void readSamples(int16_t *buffer, int len);
    int stepShift(double *buffer, int size);
    int decodeBit(int16_t *window);
    int receiveFirstTwoBits();
    void clearInput();  // TODO: i don't actually know if this is needed

    QAudioFormat format;
    std::unique_ptr<AudioInput> input;

    std::vector<int16_t> window;
    std::vector<int16_t> sync_in;
    std::vector<double> sync_lo_out;
    std::vector<double> sync_hi_out;

    int lo_ratio;
    int hi_ratio;
    int win_size;
    int mag_lim;
};

#endif
