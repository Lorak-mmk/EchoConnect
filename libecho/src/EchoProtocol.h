#ifndef ECHOCONNECT_ECHOPROTOCOL_H
#define ECHOCONNECT_ECHOPROTOCOL_H

#include "Packet.h"
#include "EchoRawConnection.h"

#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include <queue>
#include <memory>
#include <chrono>

static constexpr size_t PACKET_SIZE = 200;

class EchoProtocol {
public:
    EchoProtocol(int winsize, int send_freq, int recv_freq, int lim);
    ~EchoProtocol();
    void listen();
    void connect();
    void close();
    size_t read(void *buf, size_t count, size_t timeout);
    size_t write(const void *buf, size_t count);

private:
    std::unique_ptr<EchoRawConnection> connection;
    std::chrono::duration<double> big_win_size;
    
    std::vector<uint8_t> buffer_send;
    std::queue<uint8_t> buffer_recv;
    uint8_t *buffer;
    size_t lastPacketAcked;
    
    std::mutex m_send, m_recv;
    std::condition_variable cv_send, cv_recv;
    std::thread *thr[3];

    std::atomic<bool> closed, is_connected;
    std::atomic<int> needs_response;
    uint16_t number;
    Packet lastPacket;
    
    void sendingThread(bool b);
    
    void receivingThread(bool b);
};

#endif // ECHOCONNECT_ECHOPROTOCOL_H
