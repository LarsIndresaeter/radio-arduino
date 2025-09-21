#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/command_id.hpp>

namespace COMMANDS {

namespace SPI_WRITE {
    constexpr uint8_t COMMAND_LENGTH = 34;
    constexpr uint8_t RESPONSE_LENGTH = 1;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SPI_WRITE);
            OL = COMMAND_LENGTH;
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            reg = cmd[2];
            length = cmd[3];
            for(uint8_t i=0; i<32; i++)
            {
                data[i] = cmd[4 + i];
            }
        }

        uint8_t getReg()
        {
            return(reg);
        }

        void setReg(uint8_t value)
        {
            reg = value;
        }

        uint8_t getLength()
        {
            return(length);
        }

        void setLength(uint8_t value)
        {
            length = value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t reg;
        uint8_t length;
        uint8_t data[32];
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SPI_WRITE);
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

        uint8_t getStatus()
        {
            return(status);
        }

        void setStatus(uint8_t value)
        {
            status = value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t status;

    } response_t;
}

} // namespace commands
