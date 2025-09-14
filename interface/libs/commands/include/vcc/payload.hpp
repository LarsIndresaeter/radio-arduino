#pragma once

#include <common/command_id.hpp>

namespace COMMANDS {

namespace VCC {
    constexpr uint8_t COMMAND_LENGTH = 0;
    constexpr uint8_t RESPONSE_LENGTH = 2;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::VCC);
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
            OI = static_cast<uint8_t>(COMMANDS::OI::VCC);
            OL = RESPONSE_LENGTH;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            vcc_h = res[2];
            vcc_l = res[3];
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = vcc_h;
            response[3] = vcc_l;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t vcc_h;
        uint8_t vcc_l;

    } response_t;
}

} // namespace commands
