#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

namespace COMMANDS {

namespace WAKEUP {
    constexpr uint8_t COMMAND_LENGTH = 1;
    constexpr uint8_t RESPONSE_LENGTH = 1;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::WAKEUP);
            OL = COMMAND_LENGTH;
            checkAttentionFlag = 0;
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

        void setCheckattentionflag(uint8_t val)
        {
            checkAttentionFlag = val;
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
            discovered = 0;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            discovered = res[2];
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = discovered;
        }

        uint8_t getDiscovered()
        {
            return (discovered);
        }

        void setDiscovered(uint8_t val)
        {
            discovered = val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t discovered;

    } response_t;
}

} // namespace commands
