//#include "Echo.h"

#include <Echo.h>
#include <cstring>
#include <fstream>
#include <iostream>

void printHelp(std::string name) {
    std::cout << "Usage: " << name << " [-freq num] [-win num] [-lim num] send (data_bytes))\n";
    std::cout << "Usage: " << name << " [-freq num] [-win num] [-lim num] receive bytes_number\n";
    std::cout << "-freq: Set frequency\n";
    std::cout
        << "-win: Set window size (number of samples per bit). Higher - less errors, slowe transmition\n";
    std::cout
        << "-lim: Sensitivity of transmission beginning detection. Smaller - transmission may be detected too "
           "early, higher - not at all."
           "If message is transmitted loudly, set higher value, if your microphone is silent, set smaller value.\n";
    exit(1);
}

int main(int argc, char *argv[]) {
    Echo::initEcho(argc, argv);

    int freq = -1;
    int winsize = -1;
    int lim = -1;

    std::string name = argv[0];
    argv++;
    argc--;

    if (argc > 1 && argv[0] == std::string("-freq")) {
        freq = std::stoi(argv[1]);
        argv += 2;
        argc -= 2;
    }

    if (argc > 1 && argv[0] == std::string("-win")) {
        winsize = std::stoi(argv[1]);
        argv += 2;
        argc -= 2;
    }

    if (argc > 1 && argv[0] == std::string("-lim")) {
        lim = std::stoi(argv[1]);
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

        auto connection = EchoRawConnection::getBitEchoRawConnection(winsize, freq, freq, lim);

        auto *buf = new uint8_t[bytes];
        connection->receiveStart();
        connection->receive(buf, bytes);
        for (int i = 0; i < bytes; i++) {
            printf("%02hhx ", buf[i]);
        }
        delete connection;
        delete[] buf;

    } else if (argv[0] == std::string("send")) {
        argv++;
        argc--;

        if (argc < 1) {
            printHelp(name);
        }

        auto connection = EchoRawConnection::getBitEchoRawConnection(winsize, freq, freq, lim);
        connection->send((uint8_t *)argv[0], strlen(argv[0]));
        connection->sendWait();

        delete connection;

        printf("Data sent succesfully!\n");
    } else {
        printHelp(name);
    }
}
