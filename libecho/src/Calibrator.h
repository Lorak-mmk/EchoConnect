#ifndef ECHOCONNECT_CALIBRATOR_H
#define ECHOCONNECT_CALIBRATOR_H

#include <QAudioFormat>
#include <memory>
#include "AudioInput.h"
#include "AudioOutput.h"
#include "EchoCalibration.h"

/**
 * @brief A Class for calibration signal playback and decoding
 */
class Calibrator : public EchoCalibration {
public:
    /**
     * @brief Constructor
     * @param win_size Number of samples per bit
     * @param freq     The frequency
     */
    Calibrator(int win_size, int freq) : win_size(win_size), freq(freq) {}

    /**
     * @brief Gets an instance
     * @param win_size Number of samples per bit
     * @param freq     The frequency
     */
    static Calibrator *getCalibrator(int win_size, int freq);

    /**
     * @brief See EchoCalibration
     */
    void startPlayback() override;

    /**
     * @brief See EchoCalibration
     */
    void stopPlayback() override;

    /**
     * @brief See EchoCalibration
     */
    double getLim(int skip_ms, int record_ms) override;

    /**
     * @brief Returns input/output format of the calibrator
     */
    static QAudioFormat getFormat();

    int win_size = 0;
    int freq = 0;
    std::atomic<bool> playing = false;
    std::unique_ptr<AudioInput> input;
    std::unique_ptr<AudioOutput> output;
};

#endif  // ECHOCONNECT_CALIBRATOR_H
