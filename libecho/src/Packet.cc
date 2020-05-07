#include "Packet.h"

#ifdef _WIN32
#include <winsock2.h>
#elif defined __unix__
#include <arpa/inet.h>
#endif

#include "CRC.h"

template <typename T>
union converter {
    T a;
    uint8_t b[sizeof(T)];
};

Packet Packet::loadHeaderFromBytes(const std::vector<uint8_t> &bytes) {
    Packet p;

    if (bytes.size() != HEADER_SIZE) {
        throw IncorrectFormat();
    }

    p.flags = ntohs(reinterpret_cast<const uint16_t *>(bytes.data())[0]);
    p.size = ntohs(reinterpret_cast<const uint16_t *>(bytes.data())[1]);
    p.number = ntohs(reinterpret_cast<const uint16_t *>(bytes.data())[2]);

    return p;
}

Packet &Packet::loadDataFromBytes(const std::vector<uint8_t> &bytes) {
    if (bytes.size() != size) {
        throw IncorrectFormat();
    }

    data = bytes;

    return *this;
}

Packet &Packet::loadCRCFromBytes(const std::vector<uint8_t> &bytes) {
    if (bytes.size() != CRC_SIZE) {
        throw IncorrectFormat();
    }

    crc32 = ntohl(reinterpret_cast<const uint32_t *>(bytes.data())[0]);

    if (crc32 != calculateCRC()) {
        throw IncorrectCRC();
    }

    return *this;
}

std::vector<uint8_t> Packet::toBytes() {
    std::vector<uint8_t> bytes;
    bytes.reserve(FRAME_SIZE + size);
    converter<uint16_t> converter_u16 = {0};
    converter<uint32_t> converter_u32 = {0};

    converter_u16.a = htons(flags);
    bytes.insert(bytes.end(), converter_u16.b, converter_u16.b + 2);

    converter_u16.a = htons(size);
    bytes.insert(bytes.end(), converter_u16.b, converter_u16.b + 2);

    converter_u16.a = htons(number);
    bytes.insert(bytes.end(), converter_u16.b, converter_u16.b + 2);

    bytes.insert(bytes.end(), data.begin(), data.end());

    converter_u32.a = htonl(crc32);
    bytes.insert(bytes.end(), converter_u32.b, converter_u32.b + 4);

    return bytes;
}

bool Packet::isSet(Flag f) const {
    return (flags & f) != 0;
}

uint16_t Packet::getSize() const {
    return size;
}

uint16_t Packet::getNumber() const {
    return number;
}

const std::vector<uint8_t> &Packet::getData() {
    return data;
}

void Packet::setNumber(uint16_t num) {
    number = num;
    updateCRC();
}

void Packet::setFlag(Flag f) {
    flags |= f;
    updateCRC();
}

void Packet::unsetFlag(Flag f) {
    flags &= ~f;
    updateCRC();
}

void Packet::setData(const std::vector<uint8_t> &newData) {
    if (data.size() > MAX_DATA_SIZE) {
        throw Packet::OversizedData();
    }

    this->data = newData;
    size = data.size();
    updateCRC();
}

uint32_t Packet::calculateCRC() {
    uint32_t result = CRC::Calculate(&flags, sizeof(flags), CRC::CRC_32());
    result = CRC::Calculate(&size, sizeof(size), CRC::CRC_32(), result);
    result = CRC::Calculate(&number, sizeof(number), CRC::CRC_32(), result);
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
    return *this;
}

PacketBuilder &PacketBuilder::setFlag(Flag f) {
    pckt.flags |= f;
    return *this;
}

PacketBuilder &PacketBuilder::unsetFlag(Flag f) {
    pckt.flags &= ~f;
    return *this;
}

PacketBuilder &PacketBuilder::setNumber(uint16_t num) {
    pckt.number = num;
    return *this;
}

Packet PacketBuilder::getPacket() {
    pckt.updateCRC();
    return pckt;
}

std::vector<uint8_t> PacketBuilder::getBytes() {
    pckt.updateCRC();
    return pckt.toBytes();
}
