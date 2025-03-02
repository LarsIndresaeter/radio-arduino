#pragma once

#include <stdint.h>

#include <common/command_id.hpp>

namespace COMMANDS {

namespace SLEEP {
    constexpr uint8_t COMMAND_LENGTH = 4;
    constexpr uint8_t RESPONSE_LENGTH = 1;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            delay_0 = cmd[2];
            delay_1 = cmd[3];
            delay_2 = cmd[4];
            delay_3 = cmd[5];
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t delay_0;
        uint8_t delay_1;
        uint8_t delay_2;
        uint8_t delay_3;
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
            status = res[2];
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = status;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t status;

    } response_t;
}

} // namespace commands
