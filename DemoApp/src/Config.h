#ifndef DEMOAPP_CONFIG_H
#define DEMOAPP_CONFIG_H

#include <string>
#include <vector>

#include "Argument.h"
#include "IView.h"

#include <nlohmann/json_fwd.hpp>

class Config {
public:
    Config();

    explicit Config(const std::string& path);

    void setFile(const std::string& path);
    void load();
    void save();
    nlohmann::json& config();

    int getLimFor(int recvFreq, int winSize);
    void setLimFor(int recvFreq, int winSize, int lim, bool save = true);

private:
    std::string filename{};
    std::shared_ptr<nlohmann::json> cfg{};
};

Config *getMainConfig();

#endif  // DEMOAPP_CONFIG_H