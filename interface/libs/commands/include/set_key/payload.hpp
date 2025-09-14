#pragma once

#include <common/command_id.hpp>

namespace COMMANDS {

namespace SET_KEY {
    constexpr uint8_t COMMAND_LENGTH = 17;
    constexpr uint8_t RESPONSE_LENGTH = 1;
    constexpr uint8_t UNKNOWN_KEY_ID = 0; // Unknown Key ID
    constexpr uint8_t TK = 1; // Transport Key
    constexpr uint8_t HK = 2; // HMAC Key
    constexpr uint8_t HOTP_KEY = 3; // HMAC Key
    constexpr uint8_t EK = 4; // Encryption Key, used for AES commands
    //constexpr uint8_t KEK = 3; // Key Encryption Key
    //constexpr uint8_t SK = 4; // Storage Key

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            key_id = cmd[2];
            for(uint8_t i=0; i<16; i++)
            {
                key_value[i] = cmd[3 + i];
            }
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t key_id;
        uint8_t key_value[16];
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SET_KEY);
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
