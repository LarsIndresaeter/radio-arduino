#include <gtest/gtest.h>

#include <streamProtocol.hpp>

#include "fakeStreamProtocol.hpp"

class ProtocolClassTest : public testing::Test {
public:
    void SetUp() {}

    void TearDown() {}

    void validatePacket(
        std::vector<uint8_t> result, std::vector<uint8_t> expected)
    {
        EXPECT_TRUE(expected.size() <= result.size());

        for (int i = 0; i < expected.size(); i++) {
            EXPECT_EQ(result.at(i), expected.at(i))
                << "vector differ at: " << i;
        }
    }

    FakeStreamProtocol arduinoProtocol;
};

TEST_F(ProtocolClassTest, testStreamProtocol)
{
    std::vector<uint8_t> payload = { 0, 1, 2, 3 };
    std::vector<uint8_t> command(
        PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + payload.size());
    arduinoProtocol.createPacket(
        payload.size(),
        payload.data(),
        command.data(),
        static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::BINARY_AND_TEXT));

    arduinoProtocol.appendData(command);
    arduinoProtocol
        .getChar(); // first byte already popped by parser in arduino code
    std::vector<uint8_t> responsePayload(5);
    uint8_t protocolVersionLastReceivedMessage
        = static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::UNDEFINED);
    uint8_t length = arduinoProtocol.searchForMessage(
        responsePayload.data(), &protocolVersionLastReceivedMessage);

    validatePacket(responsePayload, { 0, 1, 2, 3 });
}

