//#include "Echo.h"

#include <iostream>
#include <fstream>
#include <Echo.h>

void printHelp(std::string name) {
    std::cout << "Usage: " << name << "[-freq num1,num2] [-win num] send (data_bytes))\n";
    std::cout << "Usage: " << name << "[-freq num1,num2] [-win num] receive bytes_number";
    exit(1);
}

int main(int argc, char *argv[]) {
    int freq1;
    int freq2;
    int winsize;

    std::string name = argv[0];
    argv++; argc--;

    if(argc > 0 && argv[0] == std::string("-freq")) {
        if(argc == 1) {
            printHelp(name);
        }
        std::string arg = argv[1];
        std::string sfreq1 = arg.substr(0, arg.find(','));
        arg.erase(0, arg.find(',') + 1);
        std::string sfreq2 = arg.substr(0, arg.find(','));

        freq1 = std::stoi(sfreq1);
        freq2 = std::stoi(sfreq2);

        argv += 2;argc -= 2;
    }

    if(argc > 0 && argv[0] == std::string("-win")) {
        if(argc == 1) {
            printHelp(name);
        }
        winsize = std::stoi(argv[1]);

        argv+= 2; argc-= 2;
    }

    if(argc == 0) {
        printHelp(name);
    }

    if(argv[0] == std::string("receive")) {
        argv++; argc--;

        if(argc == 0) {
            printHelp(name);
        }
        int bytes = std::stoi(argv[0]);
        argv++; argc--;

        // TODO: Receiving & writing;
    } else if(argv[0] == std::string("send")){
        argv++; argc--;

        if(argc == 0) {
            printHelp(name);
        }

    } else {
        printHelp(name);
    }

}


