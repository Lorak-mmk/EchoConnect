#include "EchoProtocol.h"
#include "IReceiver.h"
#include "Packet.h"

#include <QDebug>
#include <cassert>
#include <stack>
#include <string>

using namespace std::chrono_literals;

static constexpr size_t PACKET_SIZE = 50;

EchoProtocol::EchoProtocol(int winsize, int send_freq, int recv_freq, int lim)
    : connection(std::unique_ptr<EchoRawConnection>(
          EchoRawConnection::getBitEchoRawConnectionv2(winsize, send_freq, recv_freq, lim))),
      big_win_size(static_cast<double>(winsize) / 44100 * (PACKET_SIZE + 10) * 12s),
      buffer(new uint8_t[2 * PACKET_SIZE]) {}

EchoProtocol::~EchoProtocol() {
    delete[] buffer;
}

void EchoProtocol::listen() {
    thr = new std::thread{&EchoProtocol::thread, this, false};
}

void EchoProtocol::connect() {
    thr = new std::thread{&EchoProtocol::thread, this, true};
}

void EchoProtocol::close() {
    qDebug() << "close";
    closed = true;
    thr->join();
    delete thr;
    qDebug() << "close successfully finished";
}

size_t EchoProtocol::write(const void *buf, size_t count) {
    qDebug() << "write" << count << "bytes";
    size_t pos = 0;
    while (pos < count) {
        qDebug() << "write starting at position" << pos << "/" << count;
        {
            if (closed) {
                throw IReceiver::ConnectionBroken{};
            }
            std::unique_lock<std::mutex> lock(m_send);
            size_t copied_bytes = std::min(PACKET_SIZE - buffer_send.size(), count - pos);
            buffer_send.insert(buffer_send.end(), static_cast<const uint8_t *>(buf) + pos,
                               static_cast<const uint8_t *>(buf) + pos + copied_bytes);
            pos += copied_bytes;
            cv_send.wait(lock, [&] { return buffer_send.size() != PACKET_SIZE; });
        }
        qDebug() << "write ending at position" << pos << "/" << count;
        cv_send.notify_one();
    }
    qDebug() << "write successfully finished";
    return pos;
}

size_t EchoProtocol::read(void *buf, size_t count, size_t timeout) {
    std::unique_lock<std::mutex> lock(m_recv);
    if (cv_recv.wait_for(lock, timeout * 1s, [&] { return !buffer_recv.empty(); })) {
        size_t bytes = std::min(buffer_recv.size(), count);
        std::copy(static_cast<uint8_t *>(buf), static_cast<uint8_t *>(buf) + bytes, buffer_recv.begin());
        buffer_recv.erase(buffer_recv.begin(), buffer_recv.begin() + bytes);
        return bytes;
    }
    throw IReceiver::ConnectionBroken{};
}

enum Status { READY, PLEASE_ACK, PLEASE_RESEND, CORRUPTED };

const char *str[4] = {"READY", "PLEASE_ACK", "PLEASE_RESEND", "CORRUPTED"};

void EchoProtocol::thread(bool connecting) {
    std::stack<Packet> st;
    Status status;

    if (connecting) {
        // sending SYN packet if needed
        auto start = std::chrono::system_clock::now();

        Packet p = PacketBuilder().setNumber(++number).setFlag(Flag::SYN).getPacket();
        st.push(p);
        std::vector<uint8_t> bytes = p.toBytes();

        connection->sendStart();
        connection->send(bytes.data(), bytes.size());
        connection->sendWait();

        qDebug() << "sending" << bytes;

        auto end = std::chrono::system_clock::now();
        std::this_thread::sleep_for(big_win_size + start - end);
    }

    while (true) {
        // receiving a packet
        auto start = std::chrono::system_clock::now();

        try {
            connection->receiveStart(big_win_size * 2);

            int bytes = connection->receive(buffer, HEADER_SIZE + PACKET_SIZE + CRC_SIZE);
            Packet p = Packet::loadHeaderFromBytes(std::vector<uint8_t>(buffer, buffer + HEADER_SIZE));
            p.loadDataFromBytes(std::vector<uint8_t>(buffer + HEADER_SIZE, buffer + HEADER_SIZE + p.getSize()));
            p.loadCRCFromBytes(std::vector<uint8_t>(buffer + HEADER_SIZE + p.getSize(),
                                                    buffer + HEADER_SIZE + p.getSize() + CRC_SIZE));

            uint16_t num = p.getNumber();
            while (!st.empty() && st.top().isSet(Flag::DMD) && st.top().getNumber() > num) {
                st.pop();
            }
            status = READY;
            if (p.isSet(Flag::FIN) && p.isSet(Flag::ACK1))
                return;
            if (p.isSet(Flag::SYN) || p.isSet(Flag::FIN) || p.getSize() > 0) {
                status = PLEASE_ACK;
            }
            if (p.isSet(Flag::DMD)) {
                status = PLEASE_RESEND;
            }
            if (p.isSet(Flag::ACK1)) {
                assert(st.top().getNumber() + 1 == num);
                st.pop();
            }
            if (p.getSize() > 0) {
                {
                    std::unique_lock<std::mutex> lock(m_recv);
                    std::vector<uint8_t> bytes = p.getData();
                    buffer_recv.insert(buffer_recv.end(), bytes.begin(), bytes.end());
                }
                cv_recv.notify_one();
            }
            std::vector<uint8_t> rec(buffer, buffer + HEADER_SIZE + p.getSize() + CRC_SIZE);
            qDebug() << "received" << rec;
        } catch (Packet::PacketException &e) {
            status = CORRUPTED;
            qDebug() << "corrupted";
        } catch (IReceiver::ConnectionBroken &e) {
            qDebug() << "connection broken";
            return;
        }

        auto end = std::chrono::system_clock::now();
        std::this_thread::sleep_for(big_win_size + start - end);

        qDebug() << str[(int)status];

        // sending a packet
        start = std::chrono::system_clock::now();

        if (status == CORRUPTED) {
            Packet p = PacketBuilder().setNumber(number += 2).setFlag(Flag::DMD).getPacket();
            st.push(p);
        }

        if (status == PLEASE_ACK) {
            Packet p = PacketBuilder().setNumber(number += 2).setFlag(Flag::ACK1).getPacket();
            st.push(p);
        }

        if (status == READY) {
            if (closed && buffer_send.empty()) {
                Packet p = PacketBuilder().setNumber(number += 2).setFlag(Flag::FIN).getPacket();
                st.push(p);
            } else {
                {
                    std::unique_lock<std::mutex> lock(m_send);
                    Packet p = PacketBuilder().setNumber(number += 2).setData(buffer_send).getPacket();
                    st.push(p);
                    buffer_send.clear();
                }
                cv_send.notify_one();
            }
        }

        assert(!st.empty());

        std::vector<uint8_t> bytes = st.top().toBytes();

        qDebug() << "sending" << bytes;

        connection->sendStart();
        connection->send(bytes.data(), bytes.size());
        connection->sendWait();

        end = std::chrono::system_clock::now();
        std::this_thread::sleep_for(big_win_size + start - end);
    }
}
