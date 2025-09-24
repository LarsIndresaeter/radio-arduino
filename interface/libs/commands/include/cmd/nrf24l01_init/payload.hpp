#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hpp>

namespace COMMANDS {

namespace NRF24L01_INIT {
    constexpr uint8_t COMMAND_LENGTH = 12;
    constexpr uint8_t RESPONSE_LENGTH = 0;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::NRF24L01_INIT);
            OL = COMMAND_LENGTH;
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            for (uint8_t i = 0; i < 5; i++) {
                txAddr[i] = cmd[2 + i];
            }
            for (uint8_t i = 0; i < 5; i++) {
                rxAddr[i] = cmd[7 + i];
            }
            rfChannel = cmd[12];
            gateway = cmd[13];
        }

        uint8_t getRfchannel()
        {
            return (rfChannel);
        }

        void setRfchannel(uint8_t val)
        {
            rfChannel = val;
        }

        uint8_t getGateway()
        {
            return (gateway);
        }

        void setGateway(uint8_t val)
        {
            gateway = val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t txAddr[5];
        uint8_t rxAddr[5];
        uint8_t rfChannel;
        uint8_t gateway;
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::NRF24L01_INIT);
            OL = RESPONSE_LENGTH;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
        }

        uint8_t OI;
        uint8_t OL;

    } response_t;
}

} // namespace commands
