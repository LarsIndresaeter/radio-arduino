#pragma once

#include <common/command_id.hpp>

namespace COMMANDS {

namespace SPI_WRITE {
    constexpr uint8_t MAX_DATA_LENGTH = 32;
    constexpr uint8_t COMMAND_LENGTH = 2 + MAX_DATA_LENGTH;
    constexpr uint8_t RESPONSE_LENGTH = 1;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            reg = cmd[2];
            length = cmd[3];
            for (int i = 0; i < MAX_DATA_LENGTH; i++) {
                data[i] = cmd[4 + i];
            }
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t reg;
        uint8_t length;
        uint8_t data[MAX_DATA_LENGTH];
    } command_t;

    typedef struct response {
        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            status = res[2];
        }

        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SPI_WRITE);
            OL = RESPONSE_LENGTH;
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
