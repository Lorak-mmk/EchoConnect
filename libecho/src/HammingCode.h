#ifndef ECHOCONNECT_HAMMING_CODE_H
#define ECHOCONNECT_HAMMING_CODE_H

#include <cstdint>
#include <vector>

/**
 * @brief Error correction codes using Hamming(7, 4) code.
 *
 * Explanation of algorithm can be found here: https://www.tutorialspoint.com/error-correcting-codes-hamming-codes
 *
 * This class encodes every 4 bits of message using 7 bits, so after encoding it has about 175% length of original.
 * Error correction is capable of fixing 1 bit flip of those seven bits. So after dividing encoded message
 * into 7 bit parts, one bit can be wrong in every part, and HammingCode::fixError will fix all errors.
 * That means this class is theoretically capable of fixing error rate of up to ~14%.
 */
class HammingCode {
public:
    /**
     * @brief Encode @p data vector using Hamming(7, 4) code. Returns resulting bit vector.
     *
     * Length of resulting vector is equal to 14 * data.length(). So every byte of input data is encoded as 14 bits.
     *
     * @param data  Data to be encoded.
     * @return      @p data encoded as bit vector.
     */
    std::vector<bool> encode(const std::vector<uint8_t> &data);

    /**
     * @brief Tries to fix errors in @p data bit vector.
     *
     * @p data vector is modified during call.
     * You should call this method after transferring @p data trough unreliable transmission layer.
     *
     * @param data  Bit vector to be fixed. It is modified during call.
     */
    void fixErrors(std::vector<bool> &data);

    /**
     * @brief Decodes bit vector created by HammingCode::encode.
     *
     * No error checking or fixing is performed. To try to fix possible error use HammingCode::fixErrors.
     *
     * @param data Bit vector to be decoded.
     * @return     Decoded data.
     */
    std::vector<uint8_t> decode(const std::vector<bool> &data);

    /**
     * @brief Helper function, return length of bit vector after encoding @p bytesCount bytes of data.
     *
     * Pretty much it returns bytesCount * 14.
     * @param bytesCount Number of bytes to encode.
     * @return           Length of resulting bit vector.
     */
    static size_t encodedLength(size_t bytesCount);

    /**
     * @brief Helper function, return length of byte vector after decoding bit vector of @p bitCount length.
     *
     * @p bitCount must be divisible by 14 (because you can only decode bit vectors of length divisible by 14).
     * Throws std::invalid_argument if bitCount % 14 != 0. Otherwise returns bitCount / 14.
     * @param bytesCount Number of bytes to encode.
     * @return           Length of resulting bit vector.
     */
    static size_t decodedLength(size_t bitCount);

private:
    /**
     * @brief Return bit number @p idx of binary representation of @p num.
     *
     * Bit number 1 is LSB, number 7 is MSB (Least/Most significant bit)
     * @param num   Number to extract bit from.
     * @param idx   Index of bit to return.
     * @return      bit number @p idx of @p num.
     */
    static constexpr bool nthBit(uint8_t num, uint8_t idx);

    /**
     * Internal helper function. Encodes 4 bits of data from @p data into 7 bits.
     * Those 7 bits will be saved into @p dest vector, starting at index @p index.
     * Only 4 lower bits of @p data are used.
     * @param dest          Bit vector to save encoded data in.
     * @param start_index   Save encoded data starting at this index.
     * @param data          Data to be encoded. Only 4 lower bits are used.
     */
    void encodeHalfByte(std::vector<bool> &dest, size_t start_index, uint8_t data);

    /**
     * Internal helper function. Tries to fix error 7-bit part of data.
     * Fixed part starts at index @p start_index.
     * Capable of fixing at most 1 bit flip in such 7-bit part.
     * @param data          Bit vector with data to repair.
     * @param start_index   Index of start of 7-bit part to repair.
     */
    void fixHalfByte(std::vector<bool> &data, size_t start_index);
    uint8_t decodeHalfByte(const std::vector<bool> &data, size_t start_index);

    /**
     * How many bits we need to encode 1 byte.
     */
    static constexpr size_t BITS_PER_BYTE = 14;

    /**
     * How many bits we need to encode unit (4 bits).
     */
    static constexpr size_t BITS_PER_UNIT = 7;
};

#endif  // ECHOCONNECT_HAMMING_CODE_H