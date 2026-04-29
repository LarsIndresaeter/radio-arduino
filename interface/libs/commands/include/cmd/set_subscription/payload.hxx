#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

namespace COMMANDS {

namespace SET_SUBSCRIPTION {
    constexpr uint8_t COMMAND_LENGTH = 3;
    constexpr uint8_t RESPONSE_LENGTH = 0;

    static_assert(COMMAND_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SET_SUBSCRIPTION);
            OL = COMMAND_LENGTH;
            id = 0;
            for (uint8_t i = 0; i < 2; i++) {
                interval[i] = 0;
            }
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            id = cmd[2];
            for (uint8_t i = 0; i < 2; i++) {
                interval[i] = cmd[3 + i];
            }
        }

        uint8_t getId()
        {
            return (id);
        }

        void setId(uint8_t val)
        {
            id = val;
        }

        uint16_t getInterval()
        {
            return (((uint16_t)interval[1]) << 8 | interval[0]);
        }

        void setInterval(uint16_t val)
        {
            interval[1] = (uint8_t)(val >> 8);
            interval[0] = (uint8_t)val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t id;
        uint8_t interval[2];
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SET_SUBSCRIPTION);
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
