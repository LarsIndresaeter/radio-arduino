#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

namespace COMMANDS {

namespace SLEEP {
    constexpr uint8_t COMMAND_LENGTH = 4;
    constexpr uint8_t RESPONSE_LENGTH = 0;

    static_assert(COMMAND_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SLEEP);
            OL = COMMAND_LENGTH;
            for (uint8_t i = 0; i < 4; i++) {
                delay[i] = 0;
            }
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            for (uint8_t i = 0; i < 4; i++) {
                delay[i] = cmd[2 + i];
            }
        }

        uint32_t getDelay()
        {
            return (((uint32_t)delay[3]) << 24 | ((uint32_t)delay[2]) << 16 | ((uint32_t)delay[1]) << 8 | delay[0]);
        }

        void setDelay(uint32_t val)
        {
            delay[3] = (uint8_t)(val >> 24);
            delay[2] = (uint8_t)(val >> 16);
            delay[1] = (uint8_t)(val >> 8);
            delay[0] = (uint8_t)val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t delay[4];
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SLEEP);
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
