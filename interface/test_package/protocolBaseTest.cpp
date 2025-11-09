#include <gtest/gtest.h>

#include "fakeBufferProtocol.hpp"
#include "fakeCryptoHandler.hpp"
#include <protocolBase.hpp>

class ProtocolBaseTest : public testing::Test {
public:
    void SetUp() {}

    void TearDown() {}

    void validatePacket(std::vector<uint8_t> result, std::vector<uint8_t> expected)
    {
        EXPECT_TRUE(expected.size() <= result.size());

        for (int i = 0; i < expected.size(); i++) {
            EXPECT_EQ(result.at(i), expected.at(i)) << "vector differ at: " << i;
        }
    }

    FakeCryptoHandler cryptoHandler;
};

TEST_F(ProtocolBaseTest, testCreateCommandFromProtocolBaseClass)
{
    std::vector<uint8_t> packet = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    std::vector<uint8_t> payload = { 64, 2, 1, 2 };

    FakeCryptoHandler cryptoHandler;
    ProtocolBase pb(&cryptoHandler);
    pb.createPacket(
        payload.size(),
        payload.data(),
        packet.data(),
        static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY));

    validatePacket(
        packet,
        {
            0xfe,
            0xed,
            1,
            4,
            64,
            2,
            1,
            2,
        });
}

TEST_F(ProtocolBaseTest, testCreateEncryptedPayloadFromProtocolBaseClass)
{
    std::vector<uint8_t> packet;
    std::vector<uint8_t> payload = { 64, 2, 1, 2 };
    packet.resize(64);

    FakeCryptoHandler cryptoHandler;
    ProtocolBase pb(&cryptoHandler);
    pb.createEncryptedPacket(
        payload.size(),
        payload.data(),
        packet.data(),
        static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY_ENCRYPTED));

    validatePacket(
        packet,
        {
            0xfe, 0xed, 3,    4 + 20, // packet header
            0x33, 0x33, 0x33, 0x33,   // checksum
            0x00, 0x00, 0x00, 0x00,   // message id
            0x11, 0x11, 0x11, 0x11,   // nonce
            0x0,  0x0,  0x0,  0x0,    // reserved
            64,   2,    1,    2,      // payload
            0x01, 0x23, 0x45, 0x67,   // mac
        });
}

TEST_F(ProtocolBaseTest, testDecryptPayload)
{
    std::vector<uint8_t> packet;
    std::vector<uint8_t> payload = {
        64, 2,  1,  2,  // payload
        0,  1,  2,  3,  // payload
        4,  5,  6,  7,  // payload
        8,  9,  10, 11, // payload
        12, 13, 14, 15  // payload
    };
    packet.resize(64);

    FakeCryptoHandler cryptoHandler;
    ProtocolBase pb(&cryptoHandler);
    pb.createEncryptedPacket(
        payload.size(),
        payload.data(),
        packet.data(),
        static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY_ENCRYPTED));

    validatePacket(
        packet,
        {
            0xfe, 0xed, 3,    4 + 36, // packet header
            0x33, 0x33, 0x33, 0x33,   // checksum
            0x00, 0x00, 0x00, 0x00,   // message id
            0x11, 0x11, 0x11, 0x11,   // nonce
            0x0,  0x0,  0x0,  0x0,    // reserved
            64,   2,    1,    2,      // payload
            0,    1,    2,    3,      // payload
            4,    5,    6,    7,      // payload
            8,    9,    10,   11,     // payload
            12,   13,   14,   15,     // payload
            0x01, 0x23, 0x45, 0x67,   // mac
        });

    uint8_t lengthOfDecryptedPacket = pb.decryptPackage(packet.data());

    EXPECT_EQ(20, lengthOfDecryptedPacket);

    validatePacket(
        packet,
        {
            0xfe, 0xed, 1,  20, // packet header
            64,   2,    1,  2,  // payload
            0,    1,    2,  3,  // payload
            4,    5,    6,  7,  // payload
            8,    9,    10, 11, // payload
            12,   13,   14, 15  // payload
        });
}
