#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

namespace COMMANDS {

namespace GET_LAST_DEVICE_ID_SEEN {
    constexpr uint8_t COMMAND_LENGTH = 0;
    constexpr uint8_t RESPONSE_LENGTH = 4;

    static_assert(COMMAND_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::GET_LAST_DEVICE_ID_SEEN);
            OL = COMMAND_LENGTH;
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
        }

        uint8_t OI;
        uint8_t OL;
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::GET_LAST_DEVICE_ID_SEEN);
            OL = RESPONSE_LENGTH;
            for (uint8_t i = 0; i < 4; i++) {
                id[i] = 0;
            }
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            for (uint8_t i = 0; i < 4; i++) {
                id[i] = res[2 + i];
            }
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            for (uint8_t i = 0; i < 4; i++) {
                response[2 + i] = id[i];
            }
        }

        uint32_t getId()
        {
            return (((uint32_t)id[3]) << 24 | ((uint32_t)id[2]) << 16 | ((uint32_t)id[1]) << 8 | id[0]);
        }

        void setId(uint32_t val)
        {
            id[3] = (uint8_t)(val >> 24);
            id[2] = (uint8_t)(val >> 16);
            id[1] = (uint8_t)(val >> 8);
            id[0] = (uint8_t)val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t id[4];

    } response_t;
}

} // namespace commands
