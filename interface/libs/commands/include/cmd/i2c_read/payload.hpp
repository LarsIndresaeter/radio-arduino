#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hpp>

namespace COMMANDS {

namespace I2C_READ {
    constexpr uint8_t COMMAND_LENGTH = 4;
    constexpr uint8_t RESPONSE_LENGTH = 20;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::I2C_READ);
            OL = COMMAND_LENGTH;
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            device = cmd[2];
            for (uint8_t i = 0; i < 2; i++) {
                registerAddress[i] = cmd[3 + i];
            }
            length = cmd[5];
        }

        uint8_t getDevice()
        {
            return (device);
        }

        void setDevice(uint8_t val)
        {
            device = val;
        }

        uint16_t getRegisteraddress()
        {
            return (((uint16_t)registerAddress[1]) << 8 | registerAddress[0]);
        }

        void setRegisteraddress(uint16_t val)
        {
            registerAddress[1] = (uint8_t)(val >> 8);
            registerAddress[0] = (uint8_t)val;
        }

        uint8_t getLength()
        {
            return (length);
        }

        void setLength(uint8_t val)
        {
            length = val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t device;
        uint8_t registerAddress[2];
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
            for (uint8_t i = 0; i < 2; i++) {
                registerAddress[i] = res[3 + i];
            }
            length = res[5];
            for (uint8_t i = 0; i < 16; i++) {
                data[i] = res[6 + i];
            }
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = device;
            for (uint8_t i = 0; i < 2; i++) {
                response[3 + i] = registerAddress[i];
            }
            response[5] = length;
            for (uint8_t i = 0; i < 16; i++) {
                response[6 + i] = data[i];
            }
        }

        uint8_t getDevice()
        {
            return (device);
        }

        void setDevice(uint8_t val)
        {
            device = val;
        }

        uint16_t getRegisteraddress()
        {
            return (((uint16_t)registerAddress[1]) << 8 | registerAddress[0]);
        }

        void setRegisteraddress(uint16_t val)
        {
            registerAddress[1] = (uint8_t)(val >> 8);
            registerAddress[0] = (uint8_t)val;
        }

        uint8_t getLength()
        {
            return (length);
        }

        void setLength(uint8_t val)
        {
            length = val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t device;
        uint8_t registerAddress[2];
        uint8_t length;
        uint8_t data[16];

    } response_t;
}

} // namespace commands
