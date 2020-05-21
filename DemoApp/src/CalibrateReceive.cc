#include "CalibrateReceive.h"
#include "Config.h"
#include "Console.h"
#include "EchoCalibration.h"
#include "Utils.h"

constexpr size_t SkipMs = 500;
constexpr size_t RecordMs = 1000;

ViewPtr CalibrateReceive::runAction() {
    int winSize = std::get<int>(arguments.at(winSizeKey).value);
    int freq = std::get<int>(arguments.at(recvFreqKey).value);

    std::cout << setFormatting({ConsoleFormat::T_BLUE});
    std::cout << " Initializing calibration...\n";

    EchoCalibration *calibration = EchoCalibration::getEchoCalibration(winSize, freq);

    std::cout << " Starting receiver calibration for window size " << winSize << " and receiving frequency " << freq
              << " \n";

    double lim = calibration->getLim(SkipMs, RecordMs);

    std::cout << clearFormatting() << setFormatting({ConsoleFormat::BOLD}) << "\n Finished calibration! Resulting lim "
              << lim << "\n"
              << clearFormatting();

    Utils::printOption(0, "Return from this view");
    Utils::printOption(1, "Save lim & return from this view");

    std::string prompt = setFormatting({ConsoleFormat::T_YELLOW}) + " Option: " + clearFormatting();
    size_t i = 0;
    while ((i = Utils::readValue<size_t>(prompt)) > 1) {
        Utils::invalidValue(" Not a valid option, press enter to continue...");
        std::cout << cursorUp(3) << clearLinesBelow();
    }

    if (i == 1) {
        getMainConfig()->setLimFor(freq, winSize, lim);
    }

    return parent;
}