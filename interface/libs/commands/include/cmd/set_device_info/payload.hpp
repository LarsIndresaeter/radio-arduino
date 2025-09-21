#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hpp>

namespace COMMANDS {

namespace SET_DEVICE_INFO {
    constexpr uint8_t COMMAND_LENGTH = 16;
    constexpr uint8_t RESPONSE_LENGTH = 1;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SET_DEVICE_INFO);
            OL = COMMAND_LENGTH;
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            for (uint8_t i = 0; i < 16; i++) {
                name[i] = cmd[2 + i];
            }
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t name[16];
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SET_DEVICE_INFO);
            OL = RESPONSE_LENGTH;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            status = res[2];
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = status;
        }

        uint8_t getStatus()
        {
            return (status);
        }

        void setStatus(uint8_t value)
        {
            status = value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t status;

    } response_t;
}

} // namespace commands
