#include <stdexcept>
#include "HammingCode.h"


std::vector<bool> HammingCode::encode(const std::vector<uint8_t> &data) {
    std::vector<bool> result(encodedLength(data.size()), false);

    for(size_t i = 0; i < data.size(); i++) {
        encodeHalfByte(result, i * BITS_PER_BYTE, data[i]);
        encodeHalfByte(result, i * BITS_PER_BYTE + BITS_PER_UNIT, data[i] >> 4U);
    }
    return result;
}

void HammingCode::fixErrors(std::vector<bool> &data) {
    for(size_t i = 0; i < data.size(); i += BITS_PER_UNIT) {
        fixHalfByte(data, i);
    }
}


std::vector<uint8_t> HammingCode::decode(const std::vector<bool> &data) {
    std::vector<uint8_t> result(decodedLength(data.size()), 0);
    for(size_t i = 0; i < data.size(); i += BITS_PER_BYTE) {
        size_t idx = i / BITS_PER_BYTE;
        result[idx] = decodeHalfByte(data, i);
        result[idx] |= decodeHalfByte(data, i + BITS_PER_UNIT) << 4U;
    }

    return result;
}

constexpr size_t HammingCode::encodedLength(size_t bytesCount) {
    return bytesCount * BITS_PER_BYTE;
}

constexpr size_t HammingCode::decodedLength(size_t bitCount) {
    if (bitCount % BITS_PER_BYTE) {
        throw std::invalid_argument("Bits number must be divisible by BITS_PER_BYTE");
    }

    return bitCount / BITS_PER_BYTE;
}

constexpr bool HammingCode::nthBit(uint8_t num, uint8_t idx) {
    return num >> idx & 1U;
}

void HammingCode::encodeHalfByte(std::vector<bool> &dest, size_t start, uint8_t data) {
    start--; // It's easier to think in 1-indexed array here;
    // Store data bits
    dest[start + 3] = nthBit(data, 0);
    dest[start + 5] = nthBit(data, 1);
    dest[start + 6] = nthBit(data, 2);
    dest[start + 7] = nthBit(data, 3);

    // Compute parity bits
    dest[start + 1] = dest[start + 3] ^ dest[start + 5] ^ dest[start + 7];
    dest[start + 2] = dest[start + 3] ^ dest[start + 6] ^ dest[start + 7];
    dest[start + 4] = dest[start + 5] ^ dest[start + 6] ^ dest[start + 7];
}

void HammingCode::fixHalfByte(std::vector<bool> &data, size_t start) {
    start--;
    size_t fixidx = 0;

    if(data[start + 1] != (data[start + 3] ^ data[start + 5] ^ data[start + 7])) {
        fixidx += 1;
    };
    if(data[start + 2] != (data[start + 3] ^ data[start + 6] ^ data[start + 7])) {
        fixidx += 2;
    };
    if(data[start + 4] != (data[start + 5] ^ data[start + 6] ^ data[start + 7])) {
        fixidx += 4;
    };

    if(fixidx) {
        data[start + fixidx] =  !data[start + fixidx];
    }
}

uint8_t HammingCode::decodeHalfByte(const std::vector<bool>& data, size_t start_index) {
    start_index--;
    uint8_t result = 0;
    result |= data[start_index + 3] << 0U;
    result |= data[start_index + 5] << 1U;
    result |= data[start_index + 6] << 2U;
    result |= data[start_index + 7] << 3U;

    return result;
}




