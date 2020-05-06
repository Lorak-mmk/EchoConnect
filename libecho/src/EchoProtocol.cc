#include "EchoProtocol.h"
#include <cassert>

using namespace std::chrono_literals;

EchoProtocol::EchoProtocol(int winsize, int send_freq, int recv_freq, int lim) {
    big_win_size = static_cast<double>(winsize) / 44100 * (PACKET_SIZE + 10) * 12s;
    buffer = new uint8_t[PACKET_SIZE];
    closed = false;
    connection = std::unique_ptr<EchoRawConnection>(EchoRawConnection::getBitEchoRawConnection(
        winsize, send_freq, recv_freq, lim));
    thr[0] = thr[1] = nullptr;
    lastPacketAcked = 0;
    number = 0;
}

void EchoProtocol::listen() {
    is_connected = false;
    thr[0] = new std::thread{ &EchoProtocol::receivingThread, this, true };
    while (!is_connected);
    thr[1] = new std::thread{ &EchoProtocol::sendingThread, this, false };
}

void EchoProtocol::connect() {
    thr[1] = new std::thread{ &EchoProtocol::sendingThread, this, true };
    std::this_thread::sleep_for(big_win_size * 0.9);
    thr[0] = new std::thread{ &EchoProtocol::receivingThread, this, false };
}

void EchoProtocol::close() {
    closed = true;
    if (thr[0] != nullptr) thr[0]->join();
    if (thr[1] != nullptr) thr[1]->join();
    delete thr[0];
    delete thr[1];
    delete[] buffer;
}

size_t EchoProtocol::write(const void *buf, size_t count) {
    size_t pos = 0;
    while (pos < count) {
        {
            if (closed) throw std::runtime_error("connection is already closed");
            std::unique_lock<std::mutex> lock(m_send);
            while (pos < count && buffer_send.size() < PACKET_SIZE) {
                buffer_send.push_back(reinterpret_cast<const uint8_t *>(buf)[pos]);
                pos++;
            }
            while (buffer_send.size() > 0) cv_send.wait(lock);
        }
        cv_send.notify_one();
    }
}

size_t EchoProtocol::read(void *buf, size_t count, size_t timeout) {
    size_t pos = 0;
    std::unique_lock<std::mutex> lock(m_recv);
    if (cv_recv.wait_for(lock, timeout * 1s, [&]{ return !buffer_recv.empty(); })) {
        while (pos < count && !buffer_recv.empty()) {
            reinterpret_cast<uint8_t *>(buf)[pos++] = buffer_recv.front();
            buffer_recv.pop();
        }
        return pos;
    } else {
        throw std::runtime_error("connection broken");
    }
}

void EchoProtocol::sendingThread(bool b) {
    while (!closed) {
        auto start = std::chrono::system_clock::now();
        if (needs_response == 3) {
            std::vector<uint8_t> vec = PacketBuilder().setFlag(Flag::DMD).getPacket().toBytes();
            connection->send(vec.data(), vec.size());
        } else {
            if (b) {
                lastPacket = PacketBuilder().setNumber(++number).setFlag(Flag::SYN).getPacket();
                b = false;
            } else if (needs_response == 0) {
                std::unique_lock<std::mutex> lock(m_send);
                lastPacket = PacketBuilder().setData(buffer_send).setNumber(++number).getPacket();
                buffer_send.clear();
            } else if (needs_response == 1) {
                lastPacket = PacketBuilder().setNumber(number).setFlag(Flag::ACK1).getPacket();
            }
            std::vector<uint8_t> vec = lastPacket.toBytes();
            connection->send(vec.data(), vec.size());
        }
        connection->sendWait();
        auto end = std::chrono::system_clock::now();
        std::this_thread::sleep_for(big_win_size + start - end);
    }
}

void EchoProtocol::receivingThread(bool b) {
    while (!closed) {
        auto start = std::chrono::system_clock::now();
        try {
            connection->receiveStart();
            std::vector<uint8_t> vec(std::max(PACKET_SIZE, FRAME_SIZE));
            int bytes = connection->receive(vec.data(), HEADER_SIZE);
            Packet p = Packet::loadHeaderFromBytes(vec);
            bytes = connection->receive(vec.data(), p.getSize());
            p.loadDataFromBytes(vec);
            bytes = connection->receive(vec.data(), CRC_SIZE);
            p.loadCRCFromBytes(vec);
            vec = p.getData();
            if (p.isSet(Flag::ACK1) || (p.getSize() == 0 && 
                !p.isSet(Flag::SYN) && !p.isSet(Flag::DMD) && !p.isSet(Flag::FIN))) {
                needs_response = 0;
            } else if (p.isSet(Flag::DMD)) {
                needs_response = 2;
            } else {
                if (b) {
                    assert(p.isSet(Flag::SYN));
                }
                if (lastPacketAcked < p.getNumber()) {
                    std::unique_lock<std::mutex> lock(m_recv);
                    for (auto i: vec) buffer_recv.push(i);
                    lastPacketAcked = p.getNumber();
                }
                needs_response = 1;
            }
        } catch (std::exception &e) {
            needs_response = 3;
        }
        auto end = std::chrono::system_clock::now();
        std::this_thread::sleep_for(big_win_size + start - end);
        if (b) is_connected = true;
    }
}
