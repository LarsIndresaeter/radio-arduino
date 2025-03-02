#pragma once

#include <common/command_id.hpp>

namespace COMMANDS {

namespace WS2812B {
    constexpr uint8_t LEDS = 45;
    constexpr uint8_t COMMAND_LENGTH = 3*LEDS;
    constexpr uint8_t RESPONSE_LENGTH = 0;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct rgb_color {
        uint8_t red, green, blue;
    } rgb_color_t;

    typedef struct command {
        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            for(uint8_t i=0;i<LEDS;i++)
            {
            data[i].red = cmd[2 + 3*i];
            data[i].green = cmd[3 + 3*i];
            data[i].blue = cmd[4 + 3*i];
            }
        }

        uint8_t OI;
        uint8_t OL;
        rgb_color_t data[LEDS];
    } command_t;

    typedef struct response {
        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
        }

        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::WS2812B);
            OL = RESPONSE_LENGTH;
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
