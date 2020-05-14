#ifndef ECHOCONNECT_ECHO_CALIBRATION_H
#define ECHOCONNECT_ECHO_CALIBRATION_H

/**
 * @brief Public API for parameter calibration.
 */
class EchoCalibration {
public:
    static EchoCalibration *getEchoCalibration(int win_size, int freq);

    virtual void startPlayback() = 0;
    virtual void stopPlayback() = 0;
    virtual double getLim(int skip_ms, int record_ms) = 0;
};

#endif  // ECHOCONNECT_ECHO_CALIBRATION_H
