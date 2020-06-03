#include "ReceiveFile.h"
#include <fstream>
#include "EchoProtocol.h"
#include "Utils.h"

constexpr int DefaultLim = 200;
constexpr size_t BufSize = 65535;
constexpr int timeout = 10;

ViewPtr ReceiveFile::runAction() {
    int winSize = std::get<int>(arguments.at(winSizeKey).value);
    int sendFreq = std::get<int>(arguments.at(sendFreqKey).value);
    int recvFreq = std::get<int>(arguments.at(recvFreqKey).value);
    double lim = getMainConfig()->getLimFor(recvFreq, winSize, DefaultLim);
    std::string file = std::get<std::string>(arguments.at("filename").value);

    std::ofstream output;
    output.exceptions(std::ifstream::badbit);
    size_t size = 0;

    std::cout << setFormatting({ConsoleFormat::T_BLUE});
    std::cout << " Opening file " << file << "\n";
    try {
        output.open(file, std::ios::out | std::ios::binary);
    } catch (const std::ifstream::failure &e) {
        std::cout << setFormatting({ConsoleFormat::T_RED}) << " Error while opening file: " << e.what()
                  << "\n Press enter to return to previous menu";
        Utils::waitForEnter();
        return parent;
    }

    std::cout << " Initializing protocol\n";
    EchoProtocol protocol(winSize, sendFreq, recvFreq, (int)lim);

    std::cout << " Listening for connections\n";
    try {
        protocol.listen();
    } catch (const std::exception &e) {  // TODO: right type
        output.close();
        std::cout << setFormatting({ConsoleFormat::T_RED}) << " Error while connecting to other host: " << e.what()
                  << "\n Press enter to return to previous menu";
        Utils::waitForEnter();
        return parent;
    }

    std::cout << " Receiving file length\n";
    try {
        uint64_t netSize = 0;
        protocol.read(&netSize, sizeof(netSize), timeout);
        size = NTOHLL(netSize);
    } catch (const std::exception &e) {
        output.close();
        std::cout << setFormatting({ConsoleFormat::T_RED}) << " Error while receiving length: " << e.what()
                  << "\n Press enter to return to previous menu";
        Utils::waitForEnter();
        return parent;
    }
    std::cout << " Received file size: " << size << " bytes\n";

    char buffer[BufSize];

    std::cout << " Receiving...\n";
    try {
        ssize_t r = 0;
        while (size > 0 && r != -1) {
            r = protocol.read(buffer, BufSize, timeout);
            output.write(buffer, r);
            size -= r;
        }
        if (r == -1) {
            output.close();
            std::cout << setFormatting({ConsoleFormat::T_RED}) << " Connection end before end of file"
                      << "\n Press enter to return to previous menu";
            Utils::waitForEnter();

            return parent;
        }
    } catch (const std::ifstream::failure &e) {
        output.close();
        std::cout << setFormatting({ConsoleFormat::T_RED}) << " Error while saving data to file: " << e.what()
                  << "\n Press enter to return to previous menu";
        Utils::waitForEnter();
        return parent;
    } catch (const std::exception &e) {
        output.close();
        std::cout << setFormatting({ConsoleFormat::T_RED})
                  << " Error while receiving data from other host: " << e.what()
                  << "\n Press enter to return to previous menu";
        Utils::waitForEnter();
        return parent;
    }

    std::cout << " Closing connection...\n";
    try {
        protocol.close();
        output.close();
    } catch (const std::exception &e) {  // TODO
        std::cout << setFormatting({ConsoleFormat::T_RED}) << " Error while ending connection: " << e.what()
                  << "\n Press enter to return to previous menu";
        Utils::waitForEnter();
        return parent;
    }

    std::cout << clearFormatting() << setFormatting({ConsoleFormat::BOLD}) << "\n File successfully received!\n\n"
              << clearFormatting();

    if (!Utils::isCLIMode()) {
        std::cout << setFormatting({ConsoleFormat::T_YELLOW}) << "\n\n Press enter to return from this view..."
                  << clearFormatting();
        Utils::waitForEnter();
    }

    return parent;
}
