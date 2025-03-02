#pragma once

#include <common/command_id.hpp>

namespace COMMANDS {

namespace SSD1306 {
    constexpr uint8_t STRING_LENGTH = 16;
    constexpr uint8_t COMMAND_LENGTH = STRING_LENGTH + 1;
    constexpr uint8_t RESPONSE_LENGTH = 0;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            line = cmd[2];

            for (int i = 0; i < OL; i++) {
                if (i >= COMMAND_LENGTH) {
                    break;
                }
                data[i] = cmd[i + 3];
            }
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t line;
        uint8_t data[COMMAND_LENGTH];
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SSD1306);
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
