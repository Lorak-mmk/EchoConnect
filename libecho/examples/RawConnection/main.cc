//#include "Echo.h"

#include <Echo.h>
#include <cstring>
#include <fstream>
#include <iostream>

void printHelp(std::string name) {
    std::cout << "Usage: " << name << " [-freq low,high] [-win num] [-magLim num] send (data_bytes))\n";
    std::cout << "Usage: " << name << " [-freq low,high] [-win num] [-magLim num] receive bytes_number\n";
    std::cout << "-freq: Set frequency of 0 bit (low) and 1 bit (high)\n";
    std::cout
        << "-win: Set window size (time in which 1 bit will be transfered). Higher - less errors, slowe transmition\n";
    std::cout
        << "-magLim: Sensitivity of transmission beginning detection. Smaller - transmission may be detected too "
           "early, higher - not at all."
           "If message is transmitted loudly, set higher value, if your microphone is silent, set smaller value.\n";
    exit(1);
}

int main(int argc, char *argv[]) {
    Echo::initEcho(argc, argv);

    int freq1 = 14000;
    int freq2 = 15000;
    int winsize = 50;
    int magLim = 80;

    std::string name = argv[0];
    argv++;
    argc--;

    if (argc > 0 && argv[0] == std::string("-freq")) {
        if (argc == 1) {
            printHelp(name);
        }
        std::string arg = argv[1];
        std::string sfreq1 = arg.substr(0, arg.find(','));
        arg.erase(0, arg.find(',') + 1);
        std::string sfreq2 = arg.substr(0, arg.find(','));

        freq1 = std::stoi(sfreq1);
        freq2 = std::stoi(sfreq2);

        argv += 2;
        argc -= 2;
    }

    if (argc > 1 && argv[0] == std::string("-win")) {
        winsize = std::stoi(argv[1]);
        argv += 2;
        argc -= 2;
    }

    if (argc > 1 && argv[0] == std::string("-magLim")) {
        magLim = std::stoi(argv[1]);
        argv += 2;
        argc -= 2;
    }

    if (argc == 0) {
        printHelp(name);
    }

    if (argv[0] == std::string("receive")) {
        argv++;
        argc--;

        if (argc == 0) {
            printHelp(name);
        }
        int bytes = std::stoi(argv[0]);
        argv++;
        argc--;

        printf("Receiving %d bytes. Low frequency: %d, high frequency: %d, window size: %d, mag limit: %d\n", bytes,
               freq1, freq2, winsize, magLim);
        auto connection = EchoRawConnection::getBitEchoRawConnection(winsize, freq1, freq2, freq1, freq2, 0, magLim);
        auto *buf = new uint8_t[bytes];
        connection->receive(buf, bytes);
        for (int i = 0; i < bytes; i++) {
            printf("%02hhx ", buf[i]);
        }
        delete connection;

    } else if (argv[0] == std::string("send")) {
        argv++;
        argc--;

        if (argc < 1) {
            printHelp(name);
        }

        size_t length = strlen(argv[0]);
        std::vector<uint8_t> data(argv[0], argv[0] + length);
        printf("Sending %zu bytes. Low frequency: %d, high frequency: %d, window size: %d, mag limit: %d\n", length,
               freq1, freq2, winsize, magLim);
        auto connection = EchoRawConnection::getBitEchoRawConnection(winsize, freq1, freq2, freq1, freq2, 0, magLim);
        connection->sendBlocking(data);
        delete connection;
        printf("Data sent succesfully!\n");
    } else {
        printHelp(name);
    }
}
