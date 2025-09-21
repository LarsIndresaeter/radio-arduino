#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hpp>

namespace COMMANDS {

namespace NRF24L01_READ {
    constexpr uint8_t COMMAND_LENGTH = 129;
    constexpr uint8_t RESPONSE_LENGTH = 129;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::NRF24L01_READ);
            OL = COMMAND_LENGTH;
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            length = cmd[2];
            for(uint8_t i=0; i<128; i++)
            {
                data[i] = cmd[3 + i];
            }
        }

        uint8_t getLength()
        {
            return(length);
        }

        void setLength(uint8_t value)
        {
            length = value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t length;
        uint8_t data[128];
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::NRF24L01_READ);
            OL = RESPONSE_LENGTH;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            length = res[2];
            for(uint8_t i=0; i<128; i++)
            {
                data[i] = res[3 + i];
            }
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = length;
            for(uint8_t i=0; i<128; i++)
            {
                response[3 + i] = data[i];
            }
        }

        uint8_t getLength()
        {
            return(length);
        }

        void setLength(uint8_t value)
        {
            length = value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t length;
        uint8_t data[128];

    } response_t;
}

} // namespace commands
