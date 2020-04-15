#include <gtest/gtest.h>

#include "HammingCode.h"

TEST(test_Hamming, test_correct_byte) {
    HammingCode coder;

    for (int i = 0; i < std::numeric_limits<uint8_t>::max(); i++) {
        std::vector<uint8_t> data{static_cast<unsigned char>(i)};
        auto encoded = coder.encode(data);
        auto encoded_orig = encoded;

        coder.fixErrors(encoded);
        ASSERT_EQ(encoded_orig, encoded);  // fixErros should be no-op here.

        auto decoded = coder.decode(encoded);
        ASSERT_EQ(data, decoded);
    }
}

TEST(test_Hamming, test_incorrect_byte) {
    HammingCode coder;

    for (int i = 0; i < std::numeric_limits<uint8_t>::max(); i++) {
        std::vector<uint8_t> data{static_cast<unsigned char>(i)};
        auto encoded = coder.encode(data);
        auto encoded_orig = encoded;

        for (size_t idx = 0; idx < encoded.size(); idx++) {
            encoded[idx] = !encoded[idx];
            coder.fixErrors(encoded);
            ASSERT_EQ(encoded, encoded_orig);
        }
    }
}

TEST(test_Hamming, test_correct_message) {
    constexpr size_t length = 1000;
    HammingCode coder;

    std::vector<uint8_t> data(length, 0);
    for (size_t i = 0; i < length; i++) {
        data[i] = i;
    }

    auto encoded = coder.encode(data);
    auto encoded_orig = encoded;

    coder.fixErrors(encoded);
    ASSERT_EQ(encoded, encoded_orig);

    auto decoded = coder.decode(encoded);
    ASSERT_EQ(data, decoded);
}

TEST(test_Hamming, test_incorrect_message) {
    constexpr size_t length = 20;
    HammingCode coder;

    std::vector<uint8_t> data(length, 0);
    for (size_t i = 0; i < length; i++) {
        data[i] = i;
    }

    auto encoded = coder.encode(data);
    auto encoded_orig = encoded;

    for (size_t i = 0; i < encoded.size(); i++) {
        encoded[i] = !encoded[i];
        coder.fixErrors(encoded);
        ASSERT_EQ(encoded_orig, encoded);
    }
}
