#ifndef ECHOCONNECT_EXCEPTIONS_H
#define ECHOCONNECT_EXCEPTIONS_H

class ConnectionBroken : public std::exception {
public:
    [[nodiscard]] const char *what() const noexcept override {
        return "connnection broken";
    }
};

#endif // ECHOCONNECT_EXCEPTIONS_H
