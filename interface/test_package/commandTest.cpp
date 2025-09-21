#include <gtest/gtest.h>

#include "fakeBufferProtocol.hpp"
#include "fakeStreamProtocol.hpp"
#include <commands.hpp>

class commandTest : public testing::Test {
public:
    void SetUp() {}

    void TearDown() {}

    std::vector<uint8_t> simulateTarget(std::vector<uint8_t> payload)
    {
        std::vector<uint8_t> buffer(4 + payload.size() + 4);
        linuxProtocol.createPacket(
            payload.size(),
            payload.data(),
            buffer.data(),
            static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::BINARY_AND_TEXT));
        arduinoProtocol.appendData(buffer);

        std::vector<uint8_t> commandPayload(256);
        std::vector<uint8_t> responsePayload(256);
        uint8_t protocolVersionLastReceivedMessage
            = static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::UNDEFINED);

        while (arduinoProtocol.hasData()) {
            if (arduinoProtocol.searchForMessage(
                    commandPayload.data(), &protocolVersionLastReceivedMessage)) {
                switch (commandPayload.at(0)) {
                case static_cast<int>(COMMANDS::OI::BLINK): {
                    COMMANDS::BLINK::command_t command(commandPayload.data());
                    COMMANDS::BLINK::response_t response {};
                    response.serialize(responsePayload.data());
                } break;
                case static_cast<int>(COMMANDS::OI::SHA1): {
                    COMMANDS::SHA1::command_t command(commandPayload.data());
                    COMMANDS::SHA1::response_t response {};
                    for (int i = 0; i < 20; i++) {
                        response.data[i] = i + 100;
                    }
                    response.serialize(responsePayload.data());
                } break;
                case static_cast<int>(COMMANDS::OI::HOTP): {
                    COMMANDS::HOTP::command_t command(commandPayload.data());
                    COMMANDS::HOTP::response_t response {};
                    for (int i = 0; i < 16; i++) {
                        response.data[i] = i + 200;
                    }
                    response.serialize(responsePayload.data());
                } break;
                case static_cast<int>(COMMANDS::OI::EEPROM_WRITE): {
                    COMMANDS::EEPROM_WRITE::command_t command(
                        commandPayload.data());
                    COMMANDS::EEPROM_WRITE::response_t response {};
                    response.address[1] = command.address[1];
                    response.address[0] = command.address[0];
                    response.data = command.data;
                    response.serialize(responsePayload.data());
                } break;
                case static_cast<int>(COMMANDS::OI::EEPROM_READ): {
                    COMMANDS::EEPROM_READ::command_t command(
                        commandPayload.data());
                    COMMANDS::EEPROM_READ::response_t response {};
                    response.address[1] = command.address[1];
                    response.address[0] = command.address[0];
                    response.data = command.address[0] + 1;
                    response.serialize(responsePayload.data());
                } break;
                case static_cast<int>(COMMANDS::OI::AES): {
                    COMMANDS::AES::command_t command(commandPayload.data());
                    COMMANDS::AES::response_t response {};
                    response.type = command.type;
                    for (int i = 0; i < 16; i++) {
                        response.data[i] = i + 150;
                    }
                    response.serialize(responsePayload.data());
                } break;
                case static_cast<int>(COMMANDS::OI::PWM): {
                    COMMANDS::PWM::command_t command(commandPayload.data());
                    COMMANDS::PWM::response_t response {};
                    response.port = commandPayload.at(
                        offsetof(COMMANDS::PWM::response_t, port));
                    response.pin = commandPayload.at(
                        offsetof(COMMANDS::PWM::response_t, pin));
                    response.value = commandPayload.at(
                        offsetof(COMMANDS::PWM::response_t, value));
                    response.serialize(responsePayload.data());
                } break;
                case static_cast<int>(COMMANDS::OI::RANDOM): {
                    COMMANDS::RANDOM::command_t command(commandPayload.data());
                    COMMANDS::RANDOM::response_t response {};
                    for (int i = 0; i < 16; i++) {
                        response.data[i] = i + 220;
                    }
                    response.serialize(responsePayload.data());
                } break;
                case static_cast<int>(COMMANDS::OI::DEBUG): {
                    COMMANDS::DEBUG::command_t command(commandPayload.data());
                    COMMANDS::DEBUG::response_t response {};
                    response.data[0] = 10;
                    response.data[1] = 11;
                    response.data[2] = 12;
                    response.data[3] = 13;
                    response.serialize(responsePayload.data());
                } break;
                }
            }
        }

        responsePayload.resize(responsePayload[1] + 2);
        std::vector<uint8_t> packet(4 + responsePayload.size() + 4);
        linuxProtocol.createPacket(
            responsePayload.size(),
            responsePayload.data(),
            packet.data(),
            static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::BINARY_AND_TEXT));
        return packet;
    }

    FakeBufferProtocol linuxProtocol;
    FakeStreamProtocol arduinoProtocol;
};

TEST_F(commandTest, commandBlink)
{
    UartCommandBlink cmd {};
    cmd.setResponse(simulateTarget(cmd.getPayload()));

    cmd.setReplyStatus(UartCommandBase::ReplyStatus::Complete);
    cmd.validateResponse();

    EXPECT_TRUE(cmd.getReplyStatus() == UartCommandBase::ReplyStatus::Complete);

    EXPECT_EQ(2, cmd.responseStruct().OI);
    EXPECT_EQ(0, cmd.responseStruct().OL);
}

TEST_F(commandTest, commandSha1)
{
    UartCommandSha1 cmd({ 't', 'e', 's', 't' });
    cmd.setResponse(simulateTarget(cmd.getPayload()));

    cmd.setReplyStatus(UartCommandBase::ReplyStatus::Complete);
    cmd.validateResponse();

    EXPECT_TRUE(cmd.getReplyStatus() == UartCommandBase::ReplyStatus::Complete);

    EXPECT_EQ(3, cmd.responseStruct().OI);
    EXPECT_EQ(20, cmd.responseStruct().OL);
    EXPECT_EQ(100, cmd.responseStruct().data[0]);
    EXPECT_EQ(101, cmd.responseStruct().data[1]);
    EXPECT_EQ(102, cmd.responseStruct().data[2]);
    EXPECT_EQ(103, cmd.responseStruct().data[3]);
    EXPECT_EQ(104, cmd.responseStruct().data[4]);
    EXPECT_EQ(105, cmd.responseStruct().data[5]);
    EXPECT_EQ(106, cmd.responseStruct().data[6]);
    EXPECT_EQ(107, cmd.responseStruct().data[7]);
    EXPECT_EQ(108, cmd.responseStruct().data[8]);
    EXPECT_EQ(109, cmd.responseStruct().data[9]);
    EXPECT_EQ(110, cmd.responseStruct().data[10]);
    EXPECT_EQ(111, cmd.responseStruct().data[11]);
    EXPECT_EQ(112, cmd.responseStruct().data[12]);
    EXPECT_EQ(113, cmd.responseStruct().data[13]);
    EXPECT_EQ(114, cmd.responseStruct().data[14]);
    EXPECT_EQ(115, cmd.responseStruct().data[15]);
    EXPECT_EQ(116, cmd.responseStruct().data[16]);
    EXPECT_EQ(117, cmd.responseStruct().data[17]);
    EXPECT_EQ(118, cmd.responseStruct().data[18]);
    EXPECT_EQ(119, cmd.responseStruct().data[19]);
}

TEST_F(commandTest, commandHotp)
{
    UartCommandHotp cmd {};
    cmd.setResponse(simulateTarget(cmd.getPayload()));

    cmd.setReplyStatus(UartCommandBase::ReplyStatus::Complete);
    cmd.validateResponse();

    EXPECT_TRUE(cmd.getReplyStatus() == UartCommandBase::ReplyStatus::Complete);

    EXPECT_EQ(4, cmd.responseStruct().OI);
    EXPECT_EQ(16, cmd.responseStruct().OL);
    EXPECT_EQ(200, cmd.responseStruct().data[0]);
    EXPECT_EQ(201, cmd.responseStruct().data[1]);
    EXPECT_EQ(202, cmd.responseStruct().data[2]);
    EXPECT_EQ(203, cmd.responseStruct().data[3]);
    EXPECT_EQ(204, cmd.responseStruct().data[4]);
    EXPECT_EQ(205, cmd.responseStruct().data[5]);
    EXPECT_EQ(206, cmd.responseStruct().data[6]);
    EXPECT_EQ(207, cmd.responseStruct().data[7]);
    EXPECT_EQ(208, cmd.responseStruct().data[8]);
    EXPECT_EQ(209, cmd.responseStruct().data[9]);
    EXPECT_EQ(210, cmd.responseStruct().data[10]);
    EXPECT_EQ(211, cmd.responseStruct().data[11]);
    EXPECT_EQ(212, cmd.responseStruct().data[12]);
    EXPECT_EQ(213, cmd.responseStruct().data[13]);
    EXPECT_EQ(214, cmd.responseStruct().data[14]);
    EXPECT_EQ(215, cmd.responseStruct().data[15]);
}

TEST_F(commandTest, commandEepromWriteLow)
{
    UartCommandEepromWrite cmd(12, 13);
    cmd.setResponse(simulateTarget(cmd.getPayload()));

    cmd.setReplyStatus(UartCommandBase::ReplyStatus::Complete);
    cmd.validateResponse();

    EXPECT_TRUE(cmd.getReplyStatus() == UartCommandBase::ReplyStatus::Complete);

    EXPECT_EQ(5, cmd.responseStruct().OI);
    EXPECT_EQ(3, cmd.responseStruct().OL);
    EXPECT_EQ(0, cmd.responseStruct().address[1]);
    EXPECT_EQ(12, cmd.responseStruct().address[0]);
    EXPECT_EQ(13, cmd.responseStruct().data);
}

TEST_F(commandTest, commandEepromWriteHigh)
{
    UartCommandEepromWrite cmd(600, 13);
    cmd.setResponse(simulateTarget(cmd.getPayload()));

    cmd.setReplyStatus(UartCommandBase::ReplyStatus::Complete);
    cmd.validateResponse();

    EXPECT_TRUE(cmd.getReplyStatus() == UartCommandBase::ReplyStatus::Complete);

    EXPECT_EQ(5, cmd.responseStruct().OI);
    EXPECT_EQ(3, cmd.responseStruct().OL);
    EXPECT_EQ(2, cmd.responseStruct().address[1]);
    EXPECT_EQ(88, cmd.responseStruct().address[0]);
    EXPECT_EQ(600, cmd.responseStruct().getAddress());
    EXPECT_EQ(13, cmd.responseStruct().data);
}

TEST_F(commandTest, commandEepromReadLow)
{
    UartCommandEepromRead cmd(22);
    cmd.setResponse(simulateTarget(cmd.getPayload()));

    cmd.setReplyStatus(UartCommandBase::ReplyStatus::Complete);
    cmd.validateResponse();

    EXPECT_TRUE(cmd.getReplyStatus() == UartCommandBase::ReplyStatus::Complete);

    EXPECT_EQ(6, cmd.responseStruct().OI);
    EXPECT_EQ(3, cmd.responseStruct().OL);
    EXPECT_EQ(0, cmd.responseStruct().address[1]);
    EXPECT_EQ(22, cmd.responseStruct().address[0]);
    EXPECT_EQ(22, cmd.responseStruct().getAddress());
    EXPECT_EQ(23, cmd.responseStruct().data);
}

TEST_F(commandTest, commandEepromReadHigh)
{
    UartCommandEepromRead cmd(600);
    cmd.setResponse(simulateTarget(cmd.getPayload()));

    cmd.setReplyStatus(UartCommandBase::ReplyStatus::Complete);
    cmd.validateResponse();

    EXPECT_TRUE(cmd.getReplyStatus() == UartCommandBase::ReplyStatus::Complete);

    EXPECT_EQ(6, cmd.responseStruct().OI);
    EXPECT_EQ(3, cmd.responseStruct().OL);
    EXPECT_EQ(2, cmd.responseStruct().address[1]);
    EXPECT_EQ(88, cmd.responseStruct().address[0]);
    EXPECT_EQ(600, cmd.responseStruct().getAddress());
    EXPECT_EQ(89, cmd.responseStruct().data);
}

TEST_F(commandTest, commandAes)
{
    UartCommandAes cmd('d', { 0, 0 });
    cmd.setResponse(simulateTarget(cmd.getPayload()));

    cmd.setReplyStatus(UartCommandBase::ReplyStatus::Complete);
    cmd.validateResponse();

    EXPECT_TRUE(cmd.getReplyStatus() == UartCommandBase::ReplyStatus::Complete);

    EXPECT_EQ(7, cmd.responseStruct().OI);
    EXPECT_EQ(17, cmd.responseStruct().OL);
    EXPECT_EQ('d', cmd.responseStruct().type);
    EXPECT_EQ(150, cmd.responseStruct().data[0]);
    EXPECT_EQ(151, cmd.responseStruct().data[1]);
    EXPECT_EQ(152, cmd.responseStruct().data[2]);
    EXPECT_EQ(153, cmd.responseStruct().data[3]);
    EXPECT_EQ(154, cmd.responseStruct().data[4]);
    EXPECT_EQ(155, cmd.responseStruct().data[5]);
    EXPECT_EQ(156, cmd.responseStruct().data[6]);
    EXPECT_EQ(157, cmd.responseStruct().data[7]);
    EXPECT_EQ(158, cmd.responseStruct().data[8]);
    EXPECT_EQ(159, cmd.responseStruct().data[9]);
    EXPECT_EQ(160, cmd.responseStruct().data[10]);
    EXPECT_EQ(161, cmd.responseStruct().data[11]);
    EXPECT_EQ(162, cmd.responseStruct().data[12]);
    EXPECT_EQ(163, cmd.responseStruct().data[13]);
    EXPECT_EQ(164, cmd.responseStruct().data[14]);
    EXPECT_EQ(165, cmd.responseStruct().data[15]);
}

TEST_F(commandTest, commandPwm)
{
    UartCommandPwm cmd('a', 6, 64);
    cmd.setResponse(simulateTarget(cmd.getPayload()));

    cmd.setReplyStatus(UartCommandBase::ReplyStatus::Complete);
    cmd.validateResponse();

    EXPECT_TRUE(cmd.getReplyStatus() == UartCommandBase::ReplyStatus::Complete);

    EXPECT_EQ(8, cmd.responseStruct().OI);
    EXPECT_EQ(3, cmd.responseStruct().OL);
    EXPECT_EQ('a', cmd.responseStruct().port);
    EXPECT_EQ(6, cmd.responseStruct().pin);
    EXPECT_EQ(64, cmd.responseStruct().value);
}

TEST_F(commandTest, commandRandom)
{
    UartCommandRandom cmd {};
    cmd.setResponse(simulateTarget(cmd.getPayload()));

    cmd.setReplyStatus(UartCommandBase::ReplyStatus::Complete);
    cmd.validateResponse();

    EXPECT_TRUE(cmd.getReplyStatus() == UartCommandBase::ReplyStatus::Complete);

    EXPECT_EQ(9, cmd.responseStruct().OI);
    EXPECT_EQ(16, cmd.responseStruct().OL);
    EXPECT_EQ(220, cmd.responseStruct().data[0]);
    EXPECT_EQ(221, cmd.responseStruct().data[1]);
    EXPECT_EQ(222, cmd.responseStruct().data[2]);
    EXPECT_EQ(223, cmd.responseStruct().data[3]);
    EXPECT_EQ(224, cmd.responseStruct().data[4]);
    EXPECT_EQ(225, cmd.responseStruct().data[5]);
    EXPECT_EQ(226, cmd.responseStruct().data[6]);
    EXPECT_EQ(227, cmd.responseStruct().data[7]);
    EXPECT_EQ(228, cmd.responseStruct().data[8]);
    EXPECT_EQ(229, cmd.responseStruct().data[9]);
    EXPECT_EQ(230, cmd.responseStruct().data[10]);
    EXPECT_EQ(231, cmd.responseStruct().data[11]);
    EXPECT_EQ(232, cmd.responseStruct().data[12]);
    EXPECT_EQ(233, cmd.responseStruct().data[13]);
    EXPECT_EQ(234, cmd.responseStruct().data[14]);
    EXPECT_EQ(235, cmd.responseStruct().data[15]);
}

TEST_F(commandTest, commandDebug)
{
    UartCommandDebug cmd {};
    cmd.setResponse(simulateTarget(cmd.getPayload()));

    cmd.setReplyStatus(UartCommandBase::ReplyStatus::Complete);
    cmd.validateResponse();

    EXPECT_TRUE(cmd.getReplyStatus() == UartCommandBase::ReplyStatus::Complete);

    EXPECT_EQ(10, cmd.responseStruct().OI);
    EXPECT_EQ(32, cmd.responseStruct().OL);
    EXPECT_EQ(10, cmd.responseStruct().data[0]);
    EXPECT_EQ(11, cmd.responseStruct().data[1]);
    EXPECT_EQ(12, cmd.responseStruct().data[2]);
    EXPECT_EQ(13, cmd.responseStruct().data[3]);
}

