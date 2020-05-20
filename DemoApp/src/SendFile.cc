#include "SendFile.h"
#include <fstream>
#include "EchoProtocol.h"
#include "Utils.h"

#ifdef _WIN32
#include <winsock2.h>
#elif defined __unix__
#include <arpa/inet.h>
#endif


uint64_t HTONLL(uint64_t x) {
    return ((1 == htonl(1)) ? (x) : (((uint64_t)htonl((x)&0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)));
}

uint64_t NTOHLL(uint64_t x) {
    return ((1 == ntohl(1)) ? (x) : (((uint64_t) ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t) ((x) >> 32)));
}

constexpr int DefaultLim = 200;
constexpr size_t BufSize = 65535;

ViewPtr SendFile::runAction() {
    int winSize = std::get<int>(arguments.at(winSizeKey).value);
    int sendFreq = std::get<int>(arguments.at(sendFreqKey).value);
    int recvFreq = std::get<int>(arguments.at(recvFreqKey).value);
    double lim = getMainConfig()->getLimFor(recvFreq, winSize, DefaultLim);
    std::string file = std::get<std::string>(arguments.at("filename").value);

    std::ifstream input;
    input.exceptions(std::ifstream::badbit);
    size_t size = 0;

    std::cout << setFormatting({ConsoleFormat::T_BLUE});
    std::cout << " Opening file " << file << "\n";
    try {
        input.open(file, std::ios::in | std::ios::binary);
        size = Utils::fileSize(input);
    } catch (const std::ifstream::failure &e) {
        std::cout << setFormatting({ConsoleFormat::T_RED}) << " Error while opening file: " << e.what()
                  << "\n Press enter to return to previous menu";
        Utils::waitForEnter();
        return parent;
    }

    std::cout << " Initializing protocol\n";
    EchoProtocol protocol(winSize, sendFreq, recvFreq, (int)lim);

    try {
        std::cout << " Connecting to other host\n";
        protocol.connect();
        uint64_t netSize = HTONLL(size);
        std::cout << " Sending file size (" << size << " bytes)\n";
        protocol.write(&netSize, sizeof(netSize));
    } catch (const std::exception &e) {  // TODO: right type
        std::cout << setFormatting({ConsoleFormat::T_RED})
                  << " Error while connecting to other host or sending length: " << e.what()
                  << "\n Press enter to return to previous menu";
        Utils::waitForEnter();
        return parent;
    }

    char buffer[BufSize];

    std::cout << " Sending data...\n";
    try {
        while (size > 0) {
            input.read(buffer, BufSize);
            auto r = input.gcount();
            std::cout << " Sending " << r << " bytes\n";
            protocol.write(buffer, r);
            size -= r;
        }
    } catch (const std::ifstream::failure &e) {
        std::cout << setFormatting({ConsoleFormat::T_RED}) << " Error while reading data from file: " << e.what()
                  << "\n Press enter to return to previous menu";
        Utils::waitForEnter();
        return parent;
    } catch (const std::exception &e) {
        std::cout << setFormatting({ConsoleFormat::T_RED}) << " Error while sending data to other host: " << e.what()
                  << "\n Press enter to return to previous menu";
        Utils::waitForEnter();
        return parent;
    }

    std::cout << " Closing connection...\n";
    try {
        protocol.close();
        input.close();
    } catch (const std::exception &e) {  // TODO
        std::cout << setFormatting({ConsoleFormat::T_RED}) << " Error while ending connection: " << e.what()
                  << "\n Press enter to return to previous menu";
        Utils::waitForEnter();
        return parent;
    }

    std::cout << clearFormatting() << setFormatting({ConsoleFormat::BOLD}) << "\n File successfully sent!\n\n"
              << clearFormatting();

    std::cout << setFormatting({ConsoleFormat::T_YELLOW}) << "\n\n Press enter to return from this view..."
              << clearFormatting();
    Utils::waitForEnter();

    return parent;
}
