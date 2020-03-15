#ifndef ECHOCONNECT_ECHO_H
#define ECHOCONNECT_ECHO_H
/**
 * @brief Public API for libecho.
 * All functions required to use the library will be declared in this file.
 */

#include <cstdint>

namespace echo {
/**
 * @brief Sends given byte sequene. Currently there is no pairing or connection implemented, so anyone can receive it.
 * @param buffer    Bytes to send.
 * @param len   Length of the buffer param.
 */
void send(uint8_t *buffer, size_t len);

/**
 * @brief Waits for the signal, returns buffer with received bytes.
 * @param len   Will be set to the length of returned buffer.
 * @return  Newly allocated buffer for received transmission.
 */
uint8_t *receive(size_t *len);
}  // namespace echo

#endif
