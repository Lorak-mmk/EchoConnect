#ifndef ECHOCONNECT_CALIBRATOR_H
#define ECHOCONNECT_CALIBRATOR_H

#include <QAudioFormat>
#include <memory>
#include "AudioInput.h"
#include "AudioOutput.h"
#include "EchoCalibration.h"

class Calibrator : public EchoCalibration {
public:
    Calibrator(int win_size, int freq) : win_size(win_size), freq(freq) {}
    static Calibrator *getCalibrator(int win_size, int freq);

    void startPlayback() override;
    void stopPlayback() override;
    double getLim(int skip_ms, int record_ms) override;

    static QAudioFormat getFormat();

    int win_size = 0;
    int freq = 0;
    std::atomic<bool> playing = false;
    std::unique_ptr<AudioInput> input;
    std::unique_ptr<AudioOutput> output;
};

#endif  // ECHOCONNECT_CALIBRATOR_H