#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

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
            for (uint8_t i = 0; i < 2; i++) {
                address[i] = 0;
            }
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            for (uint8_t i = 0; i < 2; i++) {
                address[i] = cmd[2 + i];
            }
        }

        uint16_t getAddress()
        {
            return (((uint16_t)address[1]) << 8 | address[0]);
        }

        void setAddress(uint16_t val)
        {
            address[1] = (uint8_t)(val >> 8);
            address[0] = (uint8_t)val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t address[2];
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
            for (uint8_t i = 0; i < 2; i++) {
                address[i] = res[2 + i];
            }
            data = res[4];
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            for (uint8_t i = 0; i < 2; i++) {
                response[2 + i] = address[i];
            }
            response[4] = data;
        }

        uint16_t getAddress()
        {
            return (((uint16_t)address[1]) << 8 | address[0]);
        }

        void setAddress(uint16_t val)
        {
            address[1] = (uint8_t)(val >> 8);
            address[0] = (uint8_t)val;
        }

        uint8_t getData()
        {
            return (data);
        }

        void setData(uint8_t val)
        {
            data = val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t address[2];
        uint8_t data;

    } response_t;
}

} // namespace commands
