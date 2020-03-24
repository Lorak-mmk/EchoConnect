#ifndef ECHOCONNECT_ECHO_H
#define ECHOCONNECT_ECHO_H
/**
 * @brief Public API for libecho.
 * All functions required to use the library will be declared in this file.
 */

#include <cstdint>
#include <vector>

namespace echo {

void initEcho(int a_argc, char *a_argv[]);

/**
 * @brief Sends given byte sequence.
 * Currently there is no pairing or connection implemented, so anyone can receive it, and there is no guarantee
 * that received transmission will be correct.
 * @param buffer    Bytes to send.
 */
void send(const std::vector<uint8_t> &buffer);

/**
 * @brief Waits for the signal, returns buffer with received bytes.
 * There is no guarantee regarding correctness of transmission.
 * @return  Received bytes.
 */
std::vector<uint8_t> receive();
}  // namespace echo

#endif  // ECHOCONNECT_ECHO_H
