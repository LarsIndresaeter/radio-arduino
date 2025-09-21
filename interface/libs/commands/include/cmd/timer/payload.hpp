#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hpp>

namespace COMMANDS {

namespace TIMER {
    constexpr uint8_t COMMAND_LENGTH = 0;
    constexpr uint8_t RESPONSE_LENGTH = 2;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::TIMER);
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
            OI = static_cast<uint8_t>(COMMANDS::OI::TIMER);
            OL = RESPONSE_LENGTH;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            for(uint8_t i=0; i<2; i++)
            {
                pulseWidth[i] = res[2 + i];
            }
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            for(uint8_t i=0; i<2; i++)
            {
                response[2 + i] = pulseWidth[i];
            }
        }

        uint16_t getPulsewidth()
        {
            return(((uint16_t)pulseWidth[1]) << 8 | pulseWidth[0]);
        }

        void setPulsewidth(uint16_t value)
        {
            pulseWidth[1] = (uint8_t)(value >> 8);
            pulseWidth[0] = (uint8_t)value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t pulseWidth[2];

    } response_t;
}

} // namespace commands
