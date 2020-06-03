#include <iostream>
#include <sstream>

#include "Echo.h"
#include "views.h"
#include "Utils.h"

namespace {
    std::vector<std::string> getOptions(int argc, char *argv[]) {
        std::vector<std::string> res;
        for(int i = 1; i < argc; i++) {
            res.emplace_back(argv[i]);
        }

        return res;
    }

    bool startsWith(const std::string& s, const std::string& prefix) {
        if(s.length() < prefix.length()) {
            return false;
        }

        for(size_t i = 0; i < prefix.length(); i++) {
            if(s[i] != prefix[i]) {
                return false;
            }
        }

        return true;
    }

    std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
}



std::map<std::string, std::string> optionsMap(std::vector<std::string> args) {
    std::map<std::string, std::string> params;

    for(auto& arg : args) {
        if(!startsWith(arg, "--")) {
            throw std::runtime_error("Invalid argument: " + arg);
        }
        auto val = split(arg, '=');
        if(val.size() != 2) {
            throw std::runtime_error("Invalid argument: " + arg);
        }

        params[val[0].substr(2)] = val[1];
    }

    return params;
}

int main(int argc, char *argv[]) {
    Echo::initEcho(argc, argv);

    // clang-format off
    Menu mainMenu{"Main Menu",{
        new Menu{"Send", {
            new SendText{"sendText", "Send text"},
            new SendFile{"sendFile", "Send file"}
        }},
        new Menu{"Receive", {
            new ReceiveText{"recvText", "Receive text"},
            new ReceiveFile{"recvFile", "Receive file"}
        }},
        new Menu{"Calibrate", {
            new CalibrateReceive{"caliRecv", "Calibrate receiver"},
            new CalibrateSend{"caliSend", "Send callibration signal"}
        }},
        new Menu{"Experimental", {}},
        new Settings{"settings", "Settings"},
        new Help{"help", "Help"}
    }};
    // clang-format on

    auto opt = getOptions(argc, argv);
    if(opt.empty()) {
        ViewPtr currentView = &mainMenu;

        while (currentView) {
            currentView = currentView->execute();
        }
        return 0;
    }

    Utils::setCLI(true);

    if(opt[0] == "--help") {
        std::cout << "Usage: " << argv[0] << " [action] [--paramName=paramValue]...\n";
        std::cout << "Available actions and parameters:\n";
        mainMenu.printHelp();
        return 0;
    }

    std::string action = opt[0];
    opt.erase(opt.begin());

    try {
        auto args = optionsMap(opt);
        if(!mainMenu.executeCLI(action, args)) {
            std::cerr << "Invalid action name!";
            return 1;
        }
        return 0;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what();
        return 1;
    }


}