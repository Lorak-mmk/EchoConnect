#include "Config.h"
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>

namespace {
std::string getDefaultConfigLocation() {
    return "config.json";
}

nlohmann::json defaultCfg = {
    {sendFreqKey, 14000},
    {recvFreqKey, 16000},
    {winSizeKey, 150},
    {"lim", {}},
};
}  // namespace

Config::Config() {
    this->cfg = std::make_shared<nlohmann::json>();
}

Config::Config(const std::string &path) : Config() {
    this->setFile(path);
    this->load();
}

void Config::load() {
    std::ifstream input;
    input.exceptions(std::ifstream::badbit);
    input.open(this->filename);
    try {
        input >> *this->cfg;
    } catch (const nlohmann::detail::parse_error &ex) {
        *this->cfg = nlohmann::json();
        this->save();
    }
}

void Config::save() {
    std::ofstream output;
    output.exceptions(std::ofstream::badbit);
    output.open(this->filename);
    output << std::setw(4) << *this->cfg << std::endl;
}

nlohmann::json &Config::config() {
    return *this->cfg;
}

void Config::setFile(const std::string &path) {
    this->filename = path;
}

int Config::getLimFor(int recvFreq, int winSize) {
    try {
        return this->config().at("lim").at(recvFreq).at(winSize).get<int>();
    } catch (const nlohmann::json::type_error &e) {
        return -1;
    } catch (const nlohmann::json::out_of_range &e) {
        return -1;
    }
}

void Config::setLimFor(int recvFreq, int winSize, int lim, bool save) {
    this->config()["lim"][recvFreq][winSize] = lim;
    if (save) {
        this->save();
    }
}

Config *getMainConfig() {
    static std::string location = getDefaultConfigLocation();
    static Config config(location);
    if (config.config().empty()) {
        config.config() = defaultCfg;
        config.save();
    }

    return &config;
}
