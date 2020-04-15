#ifndef ECHOCONNECT_PACKET_H
#define ECHOCONNECT_PACKET_H

#include <cstdint>
#include <exception>
#include <limits>
#include <vector>

static constexpr size_t HEADER_SIZE = 6;                     /**< Size of packet header in bytes. */
static constexpr size_t CRC_SIZE = 4;                        /**< Size of control sum of packet in bytes. */
static constexpr size_t FRAME_SIZE = HEADER_SIZE + CRC_SIZE; /**< Size of packet frame in bytes. */
static constexpr size_t MAX_DATA_SIZE =
    std::numeric_limits<uint16_t>::max(); /**< Maximum size of data send in packet in bytes. */

/**
 * @brief Enum representing flags with which packet may be marked.
 */
enum Flag : uint16_t {
    SYN = 1U << 0U,  /**< Synchronization: used in connection initialization, only first send packet should have that
                    flag  set. */
    ACK1 = 1U << 1U, /**< Acknowledgement 1: used to confirm something, ex. creating a connection or that packets were
                    received coffectly. */
    DMD = 1U << 2U, /**< Demand resend: used to inform that some packet from were incorrect or lost and there is need to
                   resend them. */
    FIN = 1U << 3U, /**< Finish: used to inform that party is ending connection. */
    RST = 1U << 4U, /**< Reset: used to break the connection brutally. */
    LPC = 1U << 5U, /**< Last packet: informs that this is the last packet in a group of consecutive packets. */
    ACK2 = 1U << 6U /**< Acknowledgement 2: used to confirm receiving of packet with flag AC1. */
};

/**
 * @brief Class representing packets used to encapsulate and send data through audio.
 */
class Packet {
public:
    /**
     * @brief Constructs empty packet.
     */
    Packet() = default;

    /**
     * @brief Constructs (deserializes) a packet with only flags, size and number set from raw bytes.
     *
     * @param bytes             Raw bytes from which we build packet header.
     * @throws IncorrectFormat  Thrown when size of @p bytes is different than expected size of packet header.
     */
    static Packet loadHeaderFromBytes(const std::vector<uint8_t> &bytes);

    /**
     * @brief Loads (deserializes) data to packet from given raw bytes.
     *
     * @param bytes             Raw bytes from which we restore packet data.
     * @throws IncorrectFormat  Thrown when size of @p bytes is different than size set in packet.
     */
    Packet &loadDataFromBytes(const std::vector<uint8_t> &bytes);

    /**
     * @brief Loads (deserializes) CRC sum to packet from given raw bytes and check whether gained packet is vaild.
     *
     * @param bytes             Raw bytes from which we restore packet CRC.
     * @throws IncorrectFormat  Thrown when size of @p bytes is different than expected size of CRC.
     * @throws IncorrectCRC     Thrown when leaded CRC sum is different than one calculated from data.
     */
    Packet &loadCRCFromBytes(const std::vector<uint8_t> &bytes);

    /**
     * @brief Serializes packet.
     *
     * @return Bytes representing serialized packet.
     */
    std::vector<uint8_t> toBytes();

    /**
     * @brief Checks whether given flag is set in the packet.
     *
     * @param f Checked flag.
     * @return  Returns @p true if given flag is set, otherwise @p false.
     */
    bool isSet(Flag f);

    /**
     * @brief Returns size of packet encapsulated data.
     */
    uint16_t getSize();

    /**
     * @brief Returns sequential number of packet.
     */
    uint16_t getNumber();

    /**
     * @brief Returns data encapsulated in packet.
     */
    const std::vector<uint8_t> &getData();

    /**
     * @brief Sets sequential number of packet.
     *
     * @param num    Number to set.
     */
    void setNumber(uint16_t num);

    /**
     * @brief Marks packet with given flag.
     *
     * @param f Flag to set.
     */
    void setFlag(Flag f);

    /**
     * @brief Unmarks packet with given flag.
     *
     * @param f Flag to unset.
     */
    void unsetFlag(Flag f);

    /**
     * @brief Setter for data member.
     *
     * @param data              Data to encapsulate.
     * @throws OversizedData    Throws when given @p data size exceeds MAX_DATA_SIZE.
     */
    void setData(const std::vector<uint8_t> &data);

    /**
     * @brief Indicates that CRC in packet is invalid.
     */
    class IncorrectCRC : public std::exception {
        [[nodiscard]] const char *what() const noexcept override {
            return "Incorrect package CRC";
        }
    };

    /**
     * @brief Thrown when trying to deserialize packet from incorrect data.
     */
    class IncorrectFormat : public std::exception {
        [[nodiscard]] const char *what() const noexcept override {
            return "Incorrect package format";
        }
    };

    /**
     * @brief Thrown when someone is trying to encapsulate to large data in packet.
     */
    class OversizedData : public std::exception {
        [[nodiscard]] const char *what() const noexcept override {
            return "Too large data";
        }
    };

private:
    uint32_t calculateCRC();
    void updateCRC();
    uint16_t flags = 0;        /**< Respresents flags set in packet. */
    uint16_t size = 0;         /**< Size of data encapsulated in packet. */
    uint16_t number = 0;       /**< Sequential number of packet. */
    std::vector<uint8_t> data; /**< Data encapsulated in packet. */
    uint32_t crc32 = 0;        /**< Control sum CRC32 of packet. */

    friend class PacketBuilder;
};

/**
 * @brief Builder class for Packets.
 */
class PacketBuilder {
public:
    /**
     * @brief Initializes PacketBuilder instance.
     */
    PacketBuilder() = default;

    /**
     * @brief Puts data into packet.
     *
     * @param data              Data to encapsulate.
     * @return                  Reference to itself.
     * @throws OversizedData    Throws when given @p data size exceeds MAX_DATA_SIZE.
     */
    PacketBuilder &setData(const std::vector<uint8_t> &data);

    /**
     * @brief Marks packet with given flag.
     *
     * @param f Flag to set.
     * @return  Reference to itself.
     */
    PacketBuilder &setFlag(Flag f);

    /**
     * @brief Unmarks packet with given flag.
     *
     * @param f Flag to unset.
     * @return  Reference to itself.
     */
    PacketBuilder &unsetFlag(Flag f);

    /**
     * @brief Sets sequential number of packet.
     *
     * @param num    Number to set.
     * @return          Reference to itself.
     */
    PacketBuilder &setNumber(uint16_t num);

    /**
     * @brief Builds a packet created using PacketBuilder.
     *
     * @return  Created packet.
     */
    Packet getPacket();

    /**
     * @brief Builds and serializes packet created using PacketBuilder.
     *
     * @return Serialization of created packet.
     */
    std::vector<uint8_t> getBytes();

private:
    Packet pckt;
};

#endif  // ECHOCONNECT_PACKET_H
