#include "CalibrateSend.h"
#include "Console.h"
#include "EchoCalibration.h"
#include "Utils.h"

ViewPtr CalibrateSend::runAction() {
    int winSize = std::get<int>(arguments.at(winSizeKey).value);
    int freq =  std::get<int>(arguments.at(sendFreqKey).value);
    std::cout << setFormatting({ConsoleFormat::T_BLUE});
    std::cout << " Initializing calibration...\n";

    EchoCalibration *calibration = EchoCalibration::getEchoCalibration(winSize, freq);

    std::cout << " Starting calibration signal sending for window size " << winSize << " and sending frequency " << freq << " \n";

    calibration->startPlayback();

    std::cout << clearFormatting() << "\n Press enter to stop playback and return from this view\n";
    Utils::waitForEnter();

    calibration->stopPlayback();

    return parent;
}