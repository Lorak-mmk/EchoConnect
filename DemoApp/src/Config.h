#ifndef DEMOAPP_CONFIG_H
#define DEMOAPP_CONFIG_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Config {
public:
    Config();

    explicit Config(const std::string &path);

    void setFile(const std::string &path);
    void load();
    void save();
    nlohmann::json &config();

    int getLimFor(int recvFreq, int winSize);
    void setLimFor(int recvFreq, int winSize, int lim, bool save = true);

    template<class T, typename... S>
    T getValue(T def, S ... path) {
        try {
            return (this->config() / ... / path).template get<T>( );
        } catch (const nlohmann::json::type_error& e) {
            return def;
        } catch (const nlohmann::json::out_of_range& e) {
            return def;
        }
    }

    template<typename T, typename... S>
    void setValue(T value, S ... path) {
        (this->config() | ... | path) = value;
        this->save();
    }

private:
    std::string filename{};
    std::shared_ptr<nlohmann::json> cfg{};
};

template<typename T>
nlohmann::json& operator/ (nlohmann::json& a, T b) {
    return a.at(b);
}

template<typename T>
nlohmann::json& operator| (nlohmann::json& a, T b) {
    return a[b];
}

Config *getMainConfig();

const std::string sendFreqKey = "sendFreq";
const std::string recvFreqKey = "recvFreq";
const std::string winSizeKey = "winSize";


#endif  // DEMOAPP_CONFIG_H