#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

namespace COMMANDS {

namespace SET_ATTACHED_DEVICES_CSV_STRING {
    constexpr uint8_t COMMAND_LENGTH = 64;
    constexpr uint8_t RESPONSE_LENGTH = 0;

    static_assert(COMMAND_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SET_ATTACHED_DEVICES_CSV_STRING);
            OL = COMMAND_LENGTH;
            for (uint8_t i = 0; i < 64; i++) {
                csvString[i] = 0;
            }
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            for (uint8_t i = 0; i < 64; i++) {
                csvString[i] = cmd[2 + i];
            }
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t csvString[64];
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SET_ATTACHED_DEVICES_CSV_STRING);
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
