#pragma once

#include <stdint.h>

#include <common/command_id.hpp>

namespace COMMANDS {

namespace QUADRATURE_ENCODER {
    constexpr uint8_t COMMAND_LENGTH = 0;
    constexpr uint8_t RESPONSE_LENGTH = 5;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
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
            OI = static_cast<uint8_t>(COMMANDS::OI::QUADRATURE_ENCODER);
            OL = RESPONSE_LENGTH;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            cnt_neg_high = res[2];
            cnt_neg_low = res[3];
            cnt_pos_high = res[4];
            cnt_pos_low = res[5];
            status = res[6];
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = cnt_neg_high;
            response[3] = cnt_neg_low;
            response[4] = cnt_pos_high;
            response[5] = cnt_pos_low;
            response[6] = status;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t cnt_pos_high;
        uint8_t cnt_pos_low;
        uint8_t cnt_neg_high;
        uint8_t cnt_neg_low;
        uint8_t status;

    } response_t;
}

} // namespace commands
