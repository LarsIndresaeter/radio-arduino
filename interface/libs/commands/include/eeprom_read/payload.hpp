#pragma once

#include <common/command_id.hpp>

namespace COMMANDS {

namespace EEPROM_READ {
    constexpr uint8_t COMMAND_LENGTH = 2;
    constexpr uint8_t RESPONSE_LENGTH = 3;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::EEPROM_READ);
            OL = COMMAND_LENGTH;
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            addressHigh = cmd[2];
            addressLow = cmd[3];
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t addressHigh;
        uint8_t addressLow;
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::EEPROM_READ);
            OL = RESPONSE_LENGTH;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            addressHigh = res[2];
            addressLow = res[3];
            data = res[4];
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = addressHigh;
            response[3] = addressLow;
            response[4] = data;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t addressHigh;
        uint8_t addressLow;
        uint8_t data;

    } response_t;
}

} // namespace commands
