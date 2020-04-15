#ifndef ECHOCONNECT_HAMMING_CODE_H
#define ECHOCONNECT_HAMMING_CODE_H

#include <cstdint>
#include <vector>

class HammingCode {
public:
    std::vector<bool> encode(const std::vector<uint8_t> &data);

    void fixErrors(std::vector<bool> &data);

    std::vector<uint8_t> decode(const std::vector<bool> &data);

private:
    static constexpr size_t encodedLength(size_t bytesCount);
    static constexpr size_t decodedLength(size_t bitCount);
    static constexpr bool nthBit(uint8_t num, uint8_t idx);

    void encodeHalfByte(std::vector<bool> &dest, size_t start_index, uint8_t data);
    void fixHalfByte(std::vector<bool> &data, size_t start_index);
    uint8_t decodeHalfByte(const std::vector<bool> &data, size_t start_index);

    static constexpr size_t BITS_PER_BYTE = 14;
    static constexpr size_t BITS_PER_UNIT = 7;
};

#endif  // ECHOCONNECT_HAMMING_CODE_H