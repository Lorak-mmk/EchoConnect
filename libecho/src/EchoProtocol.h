#ifndef ECHOCONNECT_ECHOPROTOCOL_H
#define ECHOCONNECT_ECHOPROTOCOL_H

#include "EchoRawConnection.h"
#include "Packet.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

enum Status { READY, PLEASE_ACK, PLEASE_RESEND, CORRUPTED, CLOSED };

class EchoProtocol {
public:
    EchoProtocol(int winsize, int send_freq, int recv_freq, int lim);
    ~EchoProtocol();

    EchoProtocol(const EchoProtocol &other) = delete;
    EchoProtocol(EchoProtocol &&other) = delete;
    EchoProtocol &operator=(const EchoProtocol &other) = delete;
    EchoProtocol &operator=(EchoProtocol &&other) = delete;

    void listen();
    void connect();
    void close();

    size_t read(void *buf, size_t count, size_t timeout);
    size_t write(const void *buf, size_t count);

    class ConnectionBroken : public std::exception {
    public:
        [[nodiscard]] const char *what() const noexcept override {
            return "connnection broken";
        }
    };

private:
    std::atomic<bool> closed = false, is_connected = false;
    std::atomic<Status> status = READY;
    uint16_t number = 0, lastPacketAcked = 0;

    std::mutex m_send, m_recv;
    std::condition_variable cv_send, cv_recv;
    std::thread *thr[2] = {nullptr, nullptr};

    std::unique_ptr<EchoRawConnection> connection;
    std::chrono::duration<double> big_win_size;

    std::vector<uint8_t> buffer_send;
    std::queue<uint8_t> buffer_recv;
    uint8_t *buffer;

    Packet lastPacket;

    /**
     * @brief Thread sending data
     *
     * @param first       boolean value that indicates if this thread
     *                    is being started first (before receiving thread)
     */
    void sendingThread(bool first);

    /**
     * @brief Thread receiving data
     *
     * @param first       boolean value that indicates if this thread
     *                    is being started first (before receiving thread)
     */
    void receivingThread(bool first);
};

#endif  // ECHOCONNECT_ECHOPROTOCOL_H
