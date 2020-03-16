#ifndef ECHOCONNECT_ECHO_H
#define ECHOCONNECT_ECHO_H
/**
 * @brief Public API for libecho.
 * All functions required to use the library will be declared in this file.
 */

#include <cstdint>
#include <vector>

namespace echo {
/**
 * @brief Sends given byte sequence. Currently there is no pairing or connection implemented, so anyone can receive it.
 * @param buffer    Bytes to send.
 * @param len   Length of the buffer param.
 */
void send(const std::vector<uint8_t>& buffer);

/**
 * @brief Waits for the signal, returns buffer with received bytes.
 * @param len   Will be set to the length of returned buffer.
 * @return  Newly allocated buffer for received transmission.
 */
std::vector<uint8_t> receive();
}  // namespace echo

#endif
