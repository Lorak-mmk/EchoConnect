#ifndef ECHOCONNECT_ECHO_PROTOCOL_H
#define ECHOCONNECT_ECHO_PROTOCOL_H

#include "EchoRawConnection.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <thread>


class EchoProtocol {
public:
    /**
     * Constructs an EchoProtocol object, which can be used for sending
     * and receiving data, providing their correctness.
     * There should exist at most one EchoProtocol object at a time.
     */
    EchoProtocol(int winsize, int send_freq, int recv_freq, int lim);
    ~EchoProtocol();

    EchoProtocol(const EchoProtocol &other) = delete;
    EchoProtocol(EchoProtocol &&other) = delete;
    EchoProtocol &operator=(const EchoProtocol &other) = delete;
    EchoProtocol &operator=(EchoProtocol &&other) = delete;

    /**
     * @brief Waits for signal start from the other computer.
     */
    void listen();

    /**
     * @brief Connects with another computer, which should be waiting for signal.
     */
    void connect();

    /**
     * @brief Closes a connection.
     */
    void close();

    /**
     * Reads at most count bytes from audio input.
     * Can receive 0 bytes, but if it doesn't receive any signal for timeout seconds,
     * it throws an exception.
     */
    size_t read(void *buf, size_t count, size_t timeout);

    /**
     * Writes count bytes to audio output.
     * Can throw an exception when sent data isn't being confirmed.
     */
    size_t write(const void *buf, size_t count);

private:
    std::atomic<bool> closed = false, is_connected = false;
    uint16_t number = 0;

    std::mutex m_send, m_recv;
    std::condition_variable cv_send, cv_recv;
    std::thread *thr = nullptr;

    std::unique_ptr<EchoRawConnection> connection;
    std::chrono::duration<double> big_win_size;

    std::vector<uint8_t> buffer_send;
    std::deque<uint8_t> buffer_recv;
    uint8_t *buffer;

    /**
     * @brief Thread sending data
     *
     * @param first       boolean value that indicates if this thread
     *                    is being started first (before receiving thread)
     */
    void thread(bool connecting);
};

#endif  // ECHOCONNECT_ECHO_PROTOCOL_H
