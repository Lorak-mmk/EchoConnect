#ifndef ECHOCONNECT_ECHO_H
#define ECHOCONNECT_ECHO_H

/**
 * @brief Public API for libecho.
 * All functionalities provided by the library will be usable through Echo class methods.
 */

#include "AudioInput.h"
#include "AudioOutput.h"
#include "IAudioConverter.h"

#include <cstdint>
#include <vector>

class Echo {
public:
    /**
     * @brief Default constructor.
     */
    Echo();

    /**
     * TODO for @Lorak-mmk: add a comment here
     */
    static void initEcho(int a_argc, char** a_argv);

    /**
     * @brief Sends given byte sequence.
     * Currently there is no pairing or connection implemented, so anyone can receive it, and there is no guarantee
     * that received transmission will be correct.
     * @param buffer    Bytes to send.
     */
    void send(const std::vector<uint8_t>& buffer);

    /**
     * @brief Waits for the signal, returns buffer with received bytes.
     * There is no guarantee regarding correctness of transmission.
     * @return  Received bytes.
     */
    std::vector<uint8_t> receive();

private:
    using SampleType = int16_t;
    std::unique_ptr<AudioInput> input;
    std::unique_ptr<AudioOutput> output;
    std::unique_ptr<IAudioConverter<SampleType>> converter;
};

#endif  // ECHOCONNECT_ECHO_H
