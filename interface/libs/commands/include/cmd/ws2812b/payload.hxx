#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

namespace COMMANDS {

namespace WS2812B {
    constexpr uint8_t COMMAND_LENGTH = 24;
    constexpr uint8_t RESPONSE_LENGTH = 0;

    static_assert(COMMAND_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::WS2812B);
            OL = COMMAND_LENGTH;
            for (uint8_t i = 0; i < 8; i++) {
                red[i] = 0;
            }
            for (uint8_t i = 0; i < 8; i++) {
                green[i] = 0;
            }
            for (uint8_t i = 0; i < 8; i++) {
                blue[i] = 0;
            }
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            for (uint8_t i = 0; i < 8; i++) {
                red[i] = cmd[2 + i];
            }
            for (uint8_t i = 0; i < 8; i++) {
                green[i] = cmd[10 + i];
            }
            for (uint8_t i = 0; i < 8; i++) {
                blue[i] = cmd[18 + i];
            }
        }

        uint64_t getRed()
        {
            return (((uint64_t)red[7]) << 56 | ((uint64_t)red[6]) << 48 | ((uint64_t)red[5]) << 40 | ((uint64_t)red[4]) << 32 | ((uint64_t)red[3]) << 24 | ((uint64_t)red[2]) << 16 | ((uint64_t)red[1]) << 8 | red[0]);
        }

        void setRed(uint64_t val)
        {
            red[7] = (uint8_t)(val >> 56);
            red[6] = (uint8_t)(val >> 48);
            red[5] = (uint8_t)(val >> 40);
            red[4] = (uint8_t)(val >> 32);
            red[3] = (uint8_t)(val >> 24);
            red[2] = (uint8_t)(val >> 16);
            red[1] = (uint8_t)(val >> 8);
            red[0] = (uint8_t)val;
        }

        uint64_t getGreen()
        {
            return (((uint64_t)green[7]) << 56 | ((uint64_t)green[6]) << 48 | ((uint64_t)green[5]) << 40 | ((uint64_t)green[4]) << 32 | ((uint64_t)green[3]) << 24 | ((uint64_t)green[2]) << 16 | ((uint64_t)green[1]) << 8 | green[0]);
        }

        void setGreen(uint64_t val)
        {
            green[7] = (uint8_t)(val >> 56);
            green[6] = (uint8_t)(val >> 48);
            green[5] = (uint8_t)(val >> 40);
            green[4] = (uint8_t)(val >> 32);
            green[3] = (uint8_t)(val >> 24);
            green[2] = (uint8_t)(val >> 16);
            green[1] = (uint8_t)(val >> 8);
            green[0] = (uint8_t)val;
        }

        uint64_t getBlue()
        {
            return (((uint64_t)blue[7]) << 56 | ((uint64_t)blue[6]) << 48 | ((uint64_t)blue[5]) << 40 | ((uint64_t)blue[4]) << 32 | ((uint64_t)blue[3]) << 24 | ((uint64_t)blue[2]) << 16 | ((uint64_t)blue[1]) << 8 | blue[0]);
        }

        void setBlue(uint64_t val)
        {
            blue[7] = (uint8_t)(val >> 56);
            blue[6] = (uint8_t)(val >> 48);
            blue[5] = (uint8_t)(val >> 40);
            blue[4] = (uint8_t)(val >> 32);
            blue[3] = (uint8_t)(val >> 24);
            blue[2] = (uint8_t)(val >> 16);
            blue[1] = (uint8_t)(val >> 8);
            blue[0] = (uint8_t)val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t red[8];
        uint8_t green[8];
        uint8_t blue[8];
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::WS2812B);
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
