#ifndef ECHOCONNECT_ECHO_H
#define ECHOCONNECT_ECHO_H

#include <cstdint>

namespace echo {
    void send(uint8_t *buffer, size_t len);
    uint8_t *receive(size_t *len);
}

#endif
