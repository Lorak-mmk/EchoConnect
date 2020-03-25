#ifndef ECHOCONNECT_ECHO_H
#define ECHOCONNECT_ECHO_H

#include "AudioInput.h"
#include "AudioOutput.h"
#include "IAudioConverter.h"

#include <cstdint>
#include <vector>

/**
 * @brief Public API for libecho.
 *
 * All functionalities provided by the library will be usable through Echo class methods.
 */
class Echo {
public:
    /**
     * @brief Initializes an object to call library functions.
     */
    Echo();

    /**
     * @brief Initializes library, required if program doesn't need QT.
     *
     * If program doesn't use QT (by using QT I mean creating QApplication),
     * it needs to call this method before using this library.
     * If program uses QT, calling this method is unnecessary.
     * This method must never be called before creating QApplication manually!
     * @param a_argc    Program's argc from main().
     * @param a_argv    Program's argv from main().
     */
    static void initEcho(int a_argc, char **a_argv);

    /**
     * @brief Sends given byte sequence.
     *
     * Currently there is no pairing or connection implemented, so anyone can receive it, and there is no guarantee
     * that received transmission will be correct.
     * @param buffer    Bytes to send.
     */
    void send(const std::vector<uint8_t> &buffer);

    /**
     * @brief Waits for the signal, returns buffer with received bytes.
     * Works by reading fixed-sized windows from the input stream and computing
     * the discrene fourier transform for each one for 2 specific frequencies.
     * If they're both sufficiently low, it registers no signal. Otherwise the
     * larger one gets interpreted as the sent bit. The recording stops after
     * receiving a non-registered bit again. For larger robustness, the
     * function actually reads windows twice as short and discards every other
     * one, because the sender and receiver may be aligned differently.
     * For now, there is no guarantee regarding correctness of transmission.
     * @return  Received bytes.
     */
    std::vector<uint8_t> receive();

private:
    void getbuff(int bytes, char *buffer);
    void clearInput();

    using SampleType = int8_t;
    std::unique_ptr<AudioInput> input;
    std::unique_ptr<AudioOutput> output;
    std::unique_ptr<IAudioConverter<SampleType>> converter;
};

#endif  // ECHOCONNECT_ECHO_H
