#include "EchoProtocol.h"
#include <cassert>
#include <QDebug>

using namespace std::chrono_literals;

EchoProtocol::EchoProtocol(int winsize, int send_freq, int recv_freq, int lim) {
    big_win_size = static_cast<double>(winsize) / 44100 * (PACKET_SIZE + 10) * 12s;
    buffer = new uint8_t[2 * PACKET_SIZE];
    closed = false;
    connection = std::unique_ptr<EchoRawConnection>(EchoRawConnection::getBitEchoRawConnection(
        winsize, send_freq, recv_freq, lim));
    thr[0] = thr[1] = nullptr;
    lastPacketAcked = 0;
    number = 0;
}

EchoProtocol::~EchoProtocol() {
    delete[] buffer;
}

void EchoProtocol::listen() {
    qDebug() << "listen";
    is_connected = false;
    thr[0] = new std::thread{ &EchoProtocol::receivingThread, this, true };
    while (!is_connected);
    thr[1] = new std::thread{ &EchoProtocol::sendingThread, this, false };
    qDebug() << "(listen) connected successfully";
}

void EchoProtocol::connect() {
    qDebug() << "connect";
    thr[1] = new std::thread{ &EchoProtocol::sendingThread, this, true };
    std::this_thread::sleep_for(big_win_size * 0.9);
    thr[0] = new std::thread{ &EchoProtocol::receivingThread, this, false };
    qDebug() << "connected successfully";
}

void EchoProtocol::close() {
    qDebug() << "close";
    status = 4;
    std::this_thread::sleep_for(2 * big_win_size);
    closed = true;
    if (thr[0] != nullptr) thr[0]->join();
    if (thr[1] != nullptr) thr[1]->join();
    delete thr[0];
    delete thr[1];
    thr[0] = thr[1] = nullptr;
    qDebug() << "close successfully finished";
}

size_t EchoProtocol::write(const void *buf, size_t count) {
    qDebug() << "write" << count << "bytes";
    size_t pos = 0;
    while (pos < count) {
        qDebug() << "write starting at position" << pos << "/" << count;
        {
            if (closed) throw std::runtime_error("connection is already closed");
            std::unique_lock<std::mutex> lock(m_send);
            while (pos < count && buffer_send.size() < PACKET_SIZE) {
                buffer_send.push_back(reinterpret_cast<const uint8_t *>(buf)[pos]);
                pos++;
            }
            while (buffer_send.size() == PACKET_SIZE) cv_send.wait(lock);
        }
        qDebug() << "write ending at position" << pos << "/" << count;
        cv_send.notify_one();
    }
    qDebug() << "write successfully finished";
    return pos;
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
        connection->sendStart();
        if (status == 4) {
            std::vector<uint8_t> vec = PacketBuilder().setFlag(Flag::FIN).getPacket().toBytes();
            connection->send(vec.data(), vec.size());
            qDebug() << "sent FIN";
        } else if (status == 3) {
            std::vector<uint8_t> vec = PacketBuilder().setFlag(Flag::DMD).getPacket().toBytes();
            connection->send(vec.data(), vec.size());
            qDebug() << "sent DMD";
        } else {
            if (b) {
                lastPacket = PacketBuilder().setNumber(++number).setFlag(Flag::SYN).getPacket();
                b = false;
                qDebug() << "sent SYN, size" << lastPacket.getSize();
            } else if (status == 0) {
                {
                    std::unique_lock<std::mutex> lock(m_send);
                    lastPacket = PacketBuilder().setData(buffer_send).setNumber(++number).getPacket();
                    buffer_send.clear();
                    qDebug() << "sent data, size" << lastPacket.getSize();
                }
                cv_send.notify_one();
            } else if (status == 1) {
                lastPacket = PacketBuilder().setNumber(number).setFlag(Flag::ACK1).getPacket();
                qDebug() << "sent ACK, size" << lastPacket.getSize();
                assert(!lastPacket.isSet(Flag::SYN));
            } else {
                qDebug() << "retransmitted last packet, size" << lastPacket.getSize();
            }
            std::vector<uint8_t> vec = lastPacket.toBytes();
            auto debug = qDebug();
            for (size_t i = 0; i < vec.size(); ++i) debug << vec[i];
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
        std::vector<uint8_t> rec;
        try {
            connection->receiveStart();
            int bytes = connection->receive(buffer, HEADER_SIZE);
            rec.insert(rec.end(), buffer, buffer + bytes);
            qDebug() << "Received" << bytes << "bytes, expected" << HEADER_SIZE;
            Packet p = Packet::loadHeaderFromBytes(std::vector<uint8_t>(buffer, buffer + bytes));
            bytes = connection->receive(buffer, p.getSize());
            rec.insert(rec.end(), buffer, buffer + bytes);
            qDebug() << "Received" << bytes << "bytes, expected" << p.getSize();
            p.loadDataFromBytes(std::vector<uint8_t>(buffer, buffer + bytes));
            bytes = connection->receive(buffer, CRC_SIZE);
            rec.insert(rec.end(), buffer, buffer + bytes);
            qDebug() << "Received" << bytes << "bytes, expected" << CRC_SIZE;
            p.loadCRCFromBytes(std::vector<uint8_t>(buffer, buffer + bytes));
            std::vector<uint8_t> vec = p.getData();
            if (p.isSet(Flag::ACK1) || (p.getSize() == 0 && 
                !p.isSet(Flag::SYN) && !p.isSet(Flag::DMD) && !p.isSet(Flag::FIN))) {
                status = 0;
                qDebug() << "received ACK or empty packet";
            } else if (p.isSet(Flag::DMD)) {
                status = 2;
                qDebug() << "received demand resend";
            } else {
                qDebug() << "ACK1:" << (int)p.isSet(Flag::ACK1) << "size:" << p.getSize()
                  << "SYN:"<<(int)p.isSet(Flag::SYN)<< "DMD:"<<(int)p.isSet(Flag::DMD)
                  << "FIN:"<<(int)p.isSet(Flag::FIN);
                if (b) {
                    assert(p.isSet(Flag::SYN));
                    qDebug() << "received SYN";
                } else if (p.isSet(Flag::FIN)) {
                    closed = true;
                    qDebug() << "received FIN";
                } else {
                    qDebug() << "received correct packet";
                }
                if (lastPacketAcked < p.getNumber()) {
                    std::unique_lock<std::mutex> lock(m_recv);
                    for (auto i: vec) buffer_recv.push(i);
                    lastPacketAcked = p.getNumber();
                }
                status = 1;
            }
        } catch (std::exception &e) {
            qDebug() << e.what() << "– received corrupted packet";
            auto debug = qDebug();
            for (size_t i = 0; i < rec.size(); ++i) debug << rec[i];
            status = 3;
        }
        auto end = std::chrono::system_clock::now();
        std::this_thread::sleep_for(big_win_size + start - end);
        if (b) {
            is_connected = true;
            b = false;
        }
    }
}
