#include "Packet.h"

// #define crcpp_uint32
#include "CRC.h"
#include <arpa/inet.h>

static Packet &Packet::headerFromBytes(const std::vector<uint8_t> &bytes) {
    Packet p;
    
    if (bytes.size() != HEADER_SIZE) {
        throw IncorrectFormat();
    }

    p.flags = ntohs(bytes.data());
    p.size = ntohs(bytes.data() + sizeof(flags));
    p.number = ntohs(bytes.data() + sizeof(flags) + sizeof(size));
    
    return p;
}

Packet &Packet::loadDataFromBytes(const std::vector<uint8_t> &bytes) {
    if (bytes.size() != size) {
        throw IncorrectFormat();
    }

    data = bytes;

    return this;
}

Packet &Packet::loadCRCFromBytes(const std::vector<uint8_t> &bytes) {
    if (bytes.size() != CRC_SIZE) {
        throw IncorrectFormat();
    }

    crc32 = ntohl(bytes.data());
    if (crc32 != calculateCRC()) {
        throw IncorrectCRC();
    }

    return this;
}

std::vector<uint8_t> Packet::toBytes() {
    std::vector<uint8_t> bytes;
    bytes.reserve(FRAME_SIZE + size);
    bytes.push_back(htons(flags));
    bytes.push_back(htons(size));
    bytes.push_back(htons(number));
    bytes.insert(bytes.end(), data.begin(), data.end());
    bytes.push_back(htonl(crc32));
    return bytes;
}

bool Packet::isSet(Flag f) {
    return flags & f;
}

uint16_t Packet::getSize() {
    return size;
}

uint16_t Packet::getNumber() {
    return number;
}

const std::vector<uint8_t> &Packet::getData() {
    return data;
}

uint32_t Packet::calculateCRC() {
    uint32_t result;
    result = CRC::Calculate(flags, sizeof(flags), CRC::CRC_32());
    result = CRC::Calculate(size, sizeof(size), CRC::CRC_32(), result);
    result = CRC::Calculate(number, sizeof(number), CRC::CRC_32(), result);
    result = CRC::Calculate(data.data(), data.size(), CRC::CRC_32(), result);
    return result;
}

void Packet::updateCRC() {
    crc32 = calculateCRC();
}

PacketBuilder &PacketBuilder::setData(const std::vector<uint8_t> &data) {
    if (data.size() > MAX_DATA_SIZE) {
        throw Packet::OversizedData();
    }

    pckt.data = data;
    pckt.size = data.size();
    return this;
}

PacketBuilder &PacketBuilder::setFlag(Flag f) {
    pckt.flags |= f;
    return this;
}

PacketBuilder &PacketBuilder::setNumber(uint16_t num) {
    pckt.number = num;
    return this;
}

Packet PacketBuilder::getPacket() {
    pckt.updateCRC();
    return pckt;
}

std::vector<uint8_t> PacketBuilder::getBytes() {
    pckt.updateCRC();
    return pckt.toBytes();
}
