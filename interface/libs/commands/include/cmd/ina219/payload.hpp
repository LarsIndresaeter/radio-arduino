#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hpp>

namespace COMMANDS {

namespace INA219 {
    constexpr uint8_t COMMAND_LENGTH = 0;
    constexpr uint8_t RESPONSE_LENGTH = 5;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::INA219);
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
            OI = static_cast<uint8_t>(COMMANDS::OI::INA219);
            OL = RESPONSE_LENGTH;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            for (uint8_t i = 0; i < 2; i++) {
                current[i] = res[2 + i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                voltage[i] = res[4 + i];
            }
            status = res[6];
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            for (uint8_t i = 0; i < 2; i++) {
                response[2 + i] = current[i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                response[4 + i] = voltage[i];
            }
            response[6] = status;
        }

        uint16_t getCurrent()
        {
            return (((uint16_t)current[1]) << 8 | current[0]);
        }

        void setCurrent(uint16_t value)
        {
            current[1] = (uint8_t)(value >> 8);
            current[0] = (uint8_t)value;
        }

        uint16_t getVoltage()
        {
            return (((uint16_t)voltage[1]) << 8 | voltage[0]);
        }

        void setVoltage(uint16_t value)
        {
            voltage[1] = (uint8_t)(value >> 8);
            voltage[0] = (uint8_t)value;
        }

        uint8_t getStatus()
        {
            return (status);
        }

        void setStatus(uint8_t value)
        {
            status = value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t current[2];
        uint8_t voltage[2];
        uint8_t status;

    } response_t;
}

} // namespace commands
