#include <gtest/gtest.h>

#include "fakeBufferProtocol.hpp"

class LinuxProtocolTest : public testing::Test {
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

    FakeBufferProtocol linuxProtocol;
};

TEST_F(LinuxProtocolTest, testParseBinary)
{
    auto packet = linuxProtocol.createBinaryCommand(
        { 0, 1, 2, 3 }, static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::ENCRYPTED_BINARY_AND_TEXT));

    validatePacket(
        packet,
        {
            0xfe, 0xed, 2,    4 + 20, // packet header
            0x33, 0x33, 0x33, 0x33,   // checksum
            0x00, 0x00, 0x00, 0x00,   // message id
            0x11, 0x11, 0x11, 0x11,   // nonce
            0x00, 0x00, 0x00, 0x00,   // reserved
            0,    1,    2,    3,      // payload
            0x01, 0x23, 0x45, 0x67,   // mac
        });
}

TEST_F(LinuxProtocolTest, testParsingOfIncompletePacket)
{
    auto buffer = linuxProtocol.createBinaryCommand(
        { 0, 1, 2, 3 }, static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::ENCRYPTED_BINARY_AND_TEXT));
    std::vector<uint8_t> packet;

    validatePacket(
        buffer,
        {
            0xfe, 0xed, 2,    4 + 20, // packet header
            0x33, 0x33, 0x33, 0x33,   // checksum
            0x00, 0x00, 0x00, 0x00,   // message id
            0x11, 0x11, 0x11, 0x11,   // nonce
            0x00, 0x00, 0x00, 0x00,   // reserved
            0,    1,    2,    3,      // payload
            0x01, 0x23, 0x45, 0x67,   // mac
            0x85, 0x7c, 0x49, 0xd6    // crc
        });

    // remove last byte
    EXPECT_EQ(buffer.size(), 32);
    uint8_t last_byte = buffer.at(buffer.size() - 1);
    buffer.pop_back();
    EXPECT_EQ(buffer.size(), 31);

    // read packet, expect no packet to be found
    packet = linuxProtocol.searchForSync(buffer);
    EXPECT_EQ(packet.size(), 0);

    // add back last byte
    buffer.push_back(last_byte);

    // read packet, expect packet to be found
    packet = linuxProtocol.searchForSync(buffer);
    EXPECT_EQ(packet.size(), 12);
    validatePacket(packet, { 0xfe, 0xed, 1, 4, 0, 1, 2, 3 });
}

TEST_F(LinuxProtocolTest, testParseLargeBinary)
{
    auto packet = linuxProtocol.createBinaryCommand(
        {
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        },
        static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::ENCRYPTED_BINARY_AND_TEXT));

    validatePacket(
        packet,
        {
            0xfe, 0xed, 2,    static_cast<uint8_t>(packet.size() - 8),
            0x33, 0x33, 0x33, 0x33, // checksum
            0x00, 0x00, 0x00, 0x00, // message id
            0x11, 0x11, 0x11, 0x11, // nonce
            0x00, 0x00, 0x00, 0x00, // reserved
            0,    1,    2,    3,
            4,    5,    6,    7,
            8,    9,    0,    1,
            2,    3,    4,    5,
            6,    7,    8,    9,
            0,    1,    2,    3,
            4,    5,    6,    7,
            8,    9,    0,    1,
            2,    3,    4,    5,
            6,    7,    8,    9,
            0x01, 0x23, 0x45, 0x67, // mac
        });
}
