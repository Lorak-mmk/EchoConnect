#include <gtest/gtest.h>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

#ifdef _WIN32
#include <winsock2.h>
#elif defined __unix__
#include <arpa/inet.h>
#endif

#include "CRC.h"
#include "Packet.h"

uint16_t SIZE = 3, NUM = 2137;
const std::vector<Flag> SET_FLAGS{Flag::ACK, Flag::DMD, Flag::LPC}, UNSET_FLAGS{Flag::SYN, Flag::FIN, Flag::RST};
const std::vector<uint8_t> header{0, 38, 0, 3, 8, 89}, data{13, 4, 55}, incorrect_header{10, 37, 0, 4, 1, 2, 70},
    incorrect_data_small{1}, incorrect_data_big{12, 34, 56, 78}, incorrect_crc_format_small{12},
    incorrect_crc_format_big{12, 13, 14, 16, 17}, incorrect_crc{1, 1, 1, 1};

template <typename T>
union converter {
    T a;
    uint8_t b[sizeof(T)];
};

std::vector<uint8_t> calc_crc32(std::vector<uint8_t> v) {
    converter<uint32_t> conv;
    conv.a = htonl(CRC::Calculate(v.data(), v.size(), CRC::CRC_32()));
    return std::vector<uint8_t>(conv.b, conv.b + 4);
}

TEST(tests_packet, test_loadHeaderFromBytes) {
    auto p = Packet::loadHeaderFromBytes(header);
    ASSERT_EQ(SIZE, p.getSize());
    ASSERT_EQ(NUM, p.getNumber());
    for (auto f : SET_FLAGS)
        ASSERT_TRUE(p.isSet(f));
    for (auto f : UNSET_FLAGS)
        ASSERT_FALSE(p.isSet(f));
}

TEST(tests_packet, test_incorrect_loadHeaderFromBytes) {
    ASSERT_THROW(Packet::loadHeaderFromBytes(incorrect_header), Packet::IncorrectFormat);
}

TEST(tests_packet, test_loadDataFromBytes) {
    uint8_t t[] = {13, 4, 55};
    auto p = Packet::loadHeaderFromBytes(header);
    p.loadDataFromBytes(data);
    auto data = p.getData();
    ASSERT_EQ(data.size(), 3);
    for (size_t i = 0; i < 3; i++) {
        ASSERT_EQ(t[i], data[i]);
    }
}

TEST(tests_packet, test_incorrect_loadDataFromBytes) {
    auto p = Packet::loadHeaderFromBytes(header);
    ASSERT_THROW(p.loadDataFromBytes(incorrect_data_small), Packet::IncorrectFormat);
    ASSERT_THROW(p.loadDataFromBytes(incorrect_data_big), Packet::IncorrectFormat);
}

TEST(tests_packet, test_loadCRCFromBytes) {
    auto p = Packet::loadHeaderFromBytes(header);
    p.loadDataFromBytes(data);

    std::vector<uint8_t> crc_bytes;
    crc_bytes.insert(crc_bytes.end(), header.begin(), header.end());
    crc_bytes.insert(crc_bytes.end(), data.begin(), data.end());
    std::reverse(crc_bytes.data(), crc_bytes.data() + 2);
    std::reverse(crc_bytes.data() + 2, crc_bytes.data() + 4);
    std::reverse(crc_bytes.data() + 4, crc_bytes.data() + 6);

    p.loadCRCFromBytes(calc_crc32(crc_bytes));
}

TEST(tests_packet, test_incorrect_loadCRCFromBytes) {
    auto p = Packet::loadHeaderFromBytes(header);
    p.loadDataFromBytes(data);
    ASSERT_THROW(p.loadCRCFromBytes(incorrect_crc_format_small), Packet::IncorrectFormat);
    ASSERT_THROW(p.loadCRCFromBytes(incorrect_crc_format_big), Packet::IncorrectFormat);
    ASSERT_THROW(p.loadCRCFromBytes(incorrect_crc), Packet::IncorrectCRC);
}

TEST(tests_packet, test_toBytes) {
    auto pb = PacketBuilder().setData(data).setNumber(NUM);
    for (auto f : SET_FLAGS) {
        pb.setFlag(f);
    }

    std::vector<uint8_t> correct_bytes;
    correct_bytes.insert(correct_bytes.end(), header.begin(), header.end());
    correct_bytes.insert(correct_bytes.end(), data.begin(), data.end());

    std::vector<uint8_t> crc_bytes;
    crc_bytes.insert(crc_bytes.end(), header.begin(), header.end());
    crc_bytes.insert(crc_bytes.end(), data.begin(), data.end());
    std::reverse(crc_bytes.data(), crc_bytes.data() + 2);
    std::reverse(crc_bytes.data() + 2, crc_bytes.data() + 4);
    std::reverse(crc_bytes.data() + 4, crc_bytes.data() + 6);
    std::vector<uint8_t> crc = calc_crc32(crc_bytes);
    correct_bytes.insert(correct_bytes.end(), crc.begin(), crc.end());

    auto bytes = pb.getPacket().toBytes();
    ASSERT_EQ(correct_bytes.size(), bytes.size());
    for (size_t i = 0; i < correct_bytes.size(); i++) {
        ASSERT_EQ(correct_bytes[i], bytes[i]) << i;
    }
}

TEST(tests_packet, test_isSet) {
    auto pb = PacketBuilder().setData(data).setNumber(NUM);
    for (auto f : SET_FLAGS) {
        pb.setFlag(f);
    }

    auto p = pb.getPacket();

    for (auto f : SET_FLAGS)
        ASSERT_TRUE(p.isSet(f));

    for (auto f : UNSET_FLAGS)
        ASSERT_FALSE(p.isSet(f));
}

TEST(tests_packet_builder, test_PacketBuilder) {
    auto pb = PacketBuilder();
    auto p = pb.getPacket();
    auto d = p.getData();

    ASSERT_EQ(p.getSize(), 0);
    ASSERT_EQ(p.getNumber(), 0);
    ASSERT_EQ(d.size(), 0);

    for (auto f : SET_FLAGS)
        ASSERT_FALSE(p.isSet(f));

    for (auto f : UNSET_FLAGS)
        ASSERT_FALSE(p.isSet(f));
}

TEST(tests_packet_builder, test_setData) {
    auto pb = PacketBuilder();
    pb.setData(data);
    auto p = pb.getPacket();

    auto d = p.getData();
    auto s = p.getSize();

    ASSERT_EQ(s, data.size());
    ASSERT_EQ(s, d.size());

    for (size_t i = 0; i < data.size(); i++)
        ASSERT_EQ(data[i], d[i]);

    std::vector<uint8_t> v(MAX_DATA_SIZE + 10);
    ASSERT_THROW(pb.setData(v), Packet::OversizedData);

    p = pb.getPacket();

    d = p.getData();
    s = p.getSize();

    ASSERT_EQ(s, data.size());
    ASSERT_EQ(s, d.size());

    for (size_t i = 0; i < data.size(); i++)
        ASSERT_EQ(data[i], d[i]);

    pb.setData(incorrect_data_big);
    p = pb.getPacket();

    d = p.getData();
    s = p.getSize();

    ASSERT_EQ(s, incorrect_data_big.size());
    ASSERT_EQ(s, d.size());

    for (size_t i = 0; i < data.size(); i++)
        ASSERT_EQ(incorrect_data_big[i], d[i]);
}

TEST(tests_packet_builder, test_setFlag) {
    auto pb = PacketBuilder();

    for (auto f : SET_FLAGS)
        pb.setFlag(f);

    auto p = pb.getPacket();

    for (auto f : SET_FLAGS)
        ASSERT_TRUE(p.isSet(f));
    for (auto f : UNSET_FLAGS)
        ASSERT_FALSE(p.isSet(f));

    for (auto f : SET_FLAGS)
        pb.setFlag(f);

    p = pb.getPacket();

    for (auto f : SET_FLAGS)
        ASSERT_TRUE(p.isSet(f));
    for (auto f : UNSET_FLAGS)
        ASSERT_FALSE(p.isSet(f));
}

TEST(tests_packet_builder, test_setNumber) {
    auto pb = PacketBuilder().setNumber(NUM);
    auto p = pb.getPacket();

    ASSERT_EQ(p.getNumber(), NUM);

    pb.setNumber(SIZE);
    p = pb.getPacket();

    ASSERT_EQ(p.getNumber(), SIZE);
}

TEST(tests_packet_builder, test_getPacket) {
    auto pb = PacketBuilder().setData(data).setNumber(NUM);
    for (auto f : SET_FLAGS) {
        pb.setFlag(f);
    }

    auto p = pb.getPacket();
    auto d = p.getData();

    ASSERT_EQ(p.getNumber(), NUM);
    ASSERT_EQ(p.getSize(), SIZE);
    for (size_t i = 0; i < SIZE; i++) {
        ASSERT_EQ(d[i], data[i]);
    }

    for (auto f : SET_FLAGS)
        ASSERT_TRUE(p.isSet(f));
    for (auto f : UNSET_FLAGS)
        ASSERT_FALSE(p.isSet(f));
}

TEST(tests_packet_builder, test_getBytes) {
    auto pb = PacketBuilder().setData(data).setNumber(NUM);
    for (auto f : SET_FLAGS) {
        pb.setFlag(f);
    }

    std::vector<uint8_t> correct_bytes;
    correct_bytes.insert(correct_bytes.end(), header.begin(), header.end());
    correct_bytes.insert(correct_bytes.end(), data.begin(), data.end());
    std::vector<uint8_t> crc_bytes;
    crc_bytes.insert(crc_bytes.end(), header.begin(), header.end());
    crc_bytes.insert(crc_bytes.end(), data.begin(), data.end());
    std::reverse(crc_bytes.data(), crc_bytes.data() + 2);
    std::reverse(crc_bytes.data() + 2, crc_bytes.data() + 4);
    std::reverse(crc_bytes.data() + 4, crc_bytes.data() + 6);
    std::vector<uint8_t> crc = calc_crc32(crc_bytes);
    correct_bytes.insert(correct_bytes.end(), crc.begin(), crc.end());

    auto bytes = pb.getBytes();
    ASSERT_EQ(correct_bytes.size(), bytes.size());
    for (size_t i = 0; i < correct_bytes.size(); i++) {
        ASSERT_EQ(correct_bytes[i], bytes[i]);
    }
}
