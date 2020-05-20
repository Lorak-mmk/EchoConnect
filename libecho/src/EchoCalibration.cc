#include "EchoCalibration.h"
#include "Calibrator.h"

EchoCalibration *EchoCalibration::getEchoCalibration(int win_size, int freq) {
    return Calibrator::getCalibrator(win_size, freq);
}
