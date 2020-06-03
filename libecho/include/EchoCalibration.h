#ifndef ECHOCONNECT_ECHO_CALIBRATION_H
#define ECHOCONNECT_ECHO_CALIBRATION_H

/**
 * @brief Public API for parameter calibration.
 */
class EchoCalibration {
public:
    /**
     * @brief Returns an instance
     * @param win_size Number of samples per bit
     * @param freq     The frequency
     */
    static EchoCalibration *getEchoCalibration(int win_size, int freq);

    /**
     * @brief Starts the calibration signal in a seperate thread
     */
    virtual void startPlayback() = 0;

    /**
     * @brief Stops the calibration signal
     */
    virtual void stopPlayback() = 0;

    /**
     * @brief Does the calibration - blocks for a second and returns a lim value
     * The calibration signal should be running at the other side of the transmission
     */
    virtual double getLim(int skip_ms, int record_ms) = 0;
};

#endif  // ECHOCONNECT_ECHO_CALIBRATION_H
