#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hpp>

namespace COMMANDS {

namespace GPIO {
    constexpr uint8_t COMMAND_LENGTH = 0;
    constexpr uint8_t RESPONSE_LENGTH = 3;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::GPIO);
            OL = COMMAND_LENGTH;
        }

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
            OI = static_cast<uint8_t>(COMMANDS::OI::GPIO);
            OL = RESPONSE_LENGTH;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            portB = res[2];
            portC = res[3];
            portD = res[4];
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = portB;
            response[3] = portC;
            response[4] = portD;
        }

        uint8_t getPortb()
        {
            return(portB);
        }

        void setPortb(uint8_t value)
        {
            portB = value;
        }

        uint8_t getPortc()
        {
            return(portC);
        }

        void setPortc(uint8_t value)
        {
            portC = value;
        }

        uint8_t getPortd()
        {
            return(portD);
        }

        void setPortd(uint8_t value)
        {
            portD = value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t portB;
        uint8_t portC;
        uint8_t portD;

    } response_t;
}

} // namespace commands
