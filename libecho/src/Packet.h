#ifndef ECHOCONNECT_PACKET_H
#define ECHOCONNECT_PACKET_H

#include <vector>
#include <limits>
#include <cstdint>
#include <stdexcept>

static constexpr size_t HEADER_SIZE = 6;
static constexpr size_t CRC_SIZE = 4;
static constexpr size_t FRAME_SIZE = HEADER_SIZE + CRC_SIZE;
static constexpr size_t MAX_DATA_SIZE = std::numeric_limits<uint16_t>::max();

enum class Flag : public uint16_t {
    SYN = 1 << 0,  // Synchronization
    ACK = 1 << 1,  // Acknowledgement
    DMD = 1 << 2,  // Demand resend
    FIN = 1 << 3,  // Finish
    RST = 1 << 4,  // Reset
    LPC = 1 << 5   // Last packet
};

class Packet {
public:
    static Packet &headerFromBytes(const std::vector<uint8_t> &bytes);
    Packet &loadDataFromBytes(const std::vector<uint8_t> &bytes);
    Packet &loadCRCFromBytes(const std::vector<uint8_t> &bytes);
    std::vector<uint8_t> toBytes();
    bool isSet(Flag f);
    uint16_t getSize();
    uint16_t getNumber();
    const std::vector<uint8_t> &getData();

private:
    Packet() = default;
    uint32_t calculateCRC();
    void updateCRC();
    uint16_t flags;
    uint16_t size;
    uint16_t number;
    std::vector<uint8_t> data;
    uint32_t crc32;

    friend class PacketBuilder;

    class IncorrectCRC : public std::runtime_error {
        const char *what() const throw() {
            return "Incorrect package CRC";
        }
    };

    class IncorrectFormat : public std::runtime_error {
        const char *what() const throw() {
            return "Incorrect package format";
        }
    };

    class OversizedData : public std::runtime_error {
        const char *what() const throw() {
            return "Too large data";
        }
    };
};

class PacketBuilder {
public:
    PacketBuilder() = default;
    PacketBuilder(const PacketBuilder &pb) = delete;
    PacketBuilder &setData(const std::vector<uint8_t> &data);
    PacketBuilder &setFlag(Flag f);
    PacketBuilder &setNumber(uint16_t num);
    Packet getPacket();
    std::vector<uint8_t> getBytes();

private:
    Packet pckt;
};

#endif  // ECHOCONNECT_PACKET_H
