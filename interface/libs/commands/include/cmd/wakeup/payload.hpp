#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hpp>

namespace COMMANDS {

namespace WAKEUP {
    constexpr uint8_t COMMAND_LENGTH = 1;
    constexpr uint8_t RESPONSE_LENGTH = 2;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::WAKEUP);
            OL = COMMAND_LENGTH;
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            checkAttentionFlag = cmd[2];
        }

        uint8_t getCheckattentionflag()
        {
            return (checkAttentionFlag);
        }

        void setCheckattentionflag(uint8_t value)
        {
            checkAttentionFlag = value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t checkAttentionFlag;
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::WAKEUP);
            OL = RESPONSE_LENGTH;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            status = res[2];
            attention = res[3];
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = status;
            response[3] = attention;
        }

        uint8_t getStatus()
        {
            return (status);
        }

        void setStatus(uint8_t value)
        {
            status = value;
        }

        uint8_t getAttention()
        {
            return (attention);
        }

        void setAttention(uint8_t value)
        {
            attention = value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t status;
        uint8_t attention;

    } response_t;
}

} // namespace commands
