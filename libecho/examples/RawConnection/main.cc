//#include "Echo.h"
#define _GLIBCXX_USE_C99 1
#include <Echo.h>
#include <cstdlib>
#include <iostream>
#include <string>

long to_long(const std::string &s) {
    return strtol(s.c_str(), NULL, 10);
}

void printHelp(const std::string &name) {
    std::cout << "Usage: " << name << " [-freq low,high] [-win num] [-magLim num] send (data_bytes))\n";
    std::cout << "Usage: " << name << " [-freq low,high] [-win num] [-magLim num] receive bytes_number\n\n\n";
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

    long freq1 = 14000;
    long freq2 = 15000;
    long winsize = 50;
    long magLim = 80;

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

        freq1 = to_long(sfreq1);
        freq2 = to_long(sfreq2);

        argv += 2;
        argc -= 2;
    }

    if (argc > 1 && argv[0] == std::string("-win")) {
        winsize = to_long(argv[1]);
        argv += 2;
        argc -= 2;
    }

    if (argc > 1 && argv[0] == std::string("-magLim")) {
        magLim = to_long(argv[1]);
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
        int bytes = to_long(argv[0]);
        argv++;
        argc--;

        printf("Receiving %d bytes. Low frequency: %ld, high frequency: %ld, window size: %ld, mag limit: %ld\n", bytes,
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

        size_t length = std::string(argv[0]).length();
        std::vector<uint8_t> data(argv[0], argv[0] + length);
        printf("Sending %zu bytes. Low frequency: %ld, high frequency: %ld, window size: %ld, mag limit: %ld\n", length,
               freq1, freq2, winsize, magLim);
        auto connection = EchoRawConnection::getBitEchoRawConnection(winsize, freq1, freq2, freq1, freq2, 0, magLim);
        connection->sendBlocking(data);
        delete connection;
        printf("Data sent succesfully!\n");
    } else {
        printHelp(name);
    }
}
