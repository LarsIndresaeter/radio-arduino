#pragma once

#include <common/command_id.hpp>

namespace COMMANDS {

namespace SHA1 {
    constexpr uint8_t COMMAND_LENGTH = 16;
    constexpr uint8_t RESPONSE_LENGTH = 20;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];

            for (int i = 0; i < OL; i++) {
                if (i >= COMMAND_LENGTH) {
                    break;
                }
                data[i] = cmd[i + 2];
            }
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t data[COMMAND_LENGTH];
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SHA1);
            OL = RESPONSE_LENGTH;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];

            for (int i = 0; i < RESPONSE_LENGTH; i++) {
                if (i >= RESPONSE_LENGTH) {
                    break;
                }
                data[i] = res[i + 2];
            }
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            for (int i = 0; i < OL; i++) {
                response[i + 2] = data[i];
            }
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t data[RESPONSE_LENGTH];

    } response_t;
}

} // namespace commands
