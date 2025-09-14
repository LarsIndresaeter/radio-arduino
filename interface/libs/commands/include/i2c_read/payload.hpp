#pragma once

#include <common/command_id.hpp>

namespace COMMANDS {

namespace I2C_READ {
    constexpr uint8_t MAX_DATA_LENGTH = 16;
    constexpr uint8_t COMMAND_LENGTH = 4;
    constexpr uint8_t RESPONSE_LENGTH = 5 + MAX_DATA_LENGTH;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            device = cmd[2];
            registerHigh = cmd[3];
            registerLow = cmd[4];
            length = cmd[5];
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t device;
        uint8_t registerHigh;
        uint8_t registerLow;
        uint8_t length;
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::I2C_READ);
            OL = RESPONSE_LENGTH;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            device = res[2];
            registerHigh = res[3];
            registerLow = res[4];
            status = res[5];
            length = res[6];
            for(uint8_t i=0; i<16; i++)
            {
                data[i] = res[7 + i];
            }
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = device;
            response[3] = registerHigh;
            response[4] = registerLow;
            response[5] = status;
            response[6] = length;
            for(uint8_t i=0; i<16; i++)
            {
                response[7 + i] = data[i];
            }
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t device;
        uint8_t registerHigh;
        uint8_t registerLow;
        uint8_t status;
        uint8_t length;
        uint8_t data[16];

    } response_t;
}

} // namespace commands
