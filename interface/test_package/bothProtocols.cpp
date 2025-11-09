#include <gtest/gtest.h>

#include "fakeBufferProtocol.hpp"
#include "fakeStreamProtocol.hpp"

class BothProtocolsTest : public testing::Test {
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
    FakeStreamProtocol arduinoProtocol;
};

TEST_F(BothProtocolsTest, testParseBinary)
{
    auto packet = linuxProtocol.createBinaryCommand(
        { 64, 2, 0, 1 }, static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::GATEWAY_ENCRYPTED));

    validatePacket(
        packet,
        {
            0xfe, 0xed, 3,    4 + 20, // packet header
            0x33, 0x33, 0x33, 0x33,   // checksum
            0x00, 0x00, 0x00, 0x00,   // message id
            0x11, 0x11, 0x11, 0x11,   // nonce
            0x0,  0x0,  0x0,  0x0,    // reserved
            64,   2,    0,    1,      // payload
            0x01, 0x23, 0x45, 0x67,   // mac
        });

    arduinoProtocol.appendData(packet);

    std::vector<uint8_t> responsePayload(100);
    uint8_t protocolVersionLastReceivedMessage = static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::UNDEFINED);
    uint8_t length = arduinoProtocol.searchForMessage(
        responsePayload.data(),
        &protocolVersionLastReceivedMessage); // get first packet
    length = arduinoProtocol.searchForMessage(
        responsePayload.data(),
        &protocolVersionLastReceivedMessage); // get second packet

    EXPECT_EQ(length, 4);
    validatePacket(responsePayload, { 64, 2, 0, 1 });
}
