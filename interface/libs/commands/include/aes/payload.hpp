#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/command_id.hpp>

namespace COMMANDS {

namespace AES {
    constexpr uint8_t COMMAND_LENGTH = 17;
    constexpr uint8_t RESPONSE_LENGTH = 17;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::AES);
            OL = COMMAND_LENGTH;
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            type = cmd[2];
            for(uint8_t i=0; i<16; i++)
            {
                data[i] = cmd[3 + i];
            }
        }

        uint8_t getType()
        {
            return(type);
        }

        void setType(uint8_t value)
        {
            type = value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t type;
        uint8_t data[16];
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::AES);
            OL = RESPONSE_LENGTH;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            type = res[2];
            for(uint8_t i=0; i<16; i++)
            {
                data[i] = res[3 + i];
            }
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = type;
            for(uint8_t i=0; i<16; i++)
            {
                response[3 + i] = data[i];
            }
        }

        uint8_t getType()
        {
            return(type);
        }

        void setType(uint8_t value)
        {
            type = value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t type;
        uint8_t data[16];

    } response_t;
}

} // namespace commands
