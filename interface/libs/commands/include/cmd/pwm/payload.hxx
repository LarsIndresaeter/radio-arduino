#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

namespace COMMANDS {

namespace PWM {
    constexpr uint8_t COMMAND_LENGTH = 3;
    constexpr uint8_t RESPONSE_LENGTH = 3;

    static_assert(COMMAND_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::PWM);
            OL = COMMAND_LENGTH;
            port = 0;
            pin = 0;
            value = 0;
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            port = cmd[2];
            pin = cmd[3];
            value = cmd[4];
        }

        uint8_t getPort()
        {
            return (port);
        }

        void setPort(uint8_t val)
        {
            port = val;
        }

        uint8_t getPin()
        {
            return (pin);
        }

        void setPin(uint8_t val)
        {
            pin = val;
        }

        uint8_t getValue()
        {
            return (value);
        }

        void setValue(uint8_t val)
        {
            value = val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t port;
        uint8_t pin;
        uint8_t value;
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::PWM);
            OL = RESPONSE_LENGTH;
            port = 0;
            pin = 0;
            value = 0;
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            port = res[2];
            pin = res[3];
            value = res[4];
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = port;
            response[3] = pin;
            response[4] = value;
        }

        uint8_t getPort()
        {
            return (port);
        }

        void setPort(uint8_t val)
        {
            port = val;
        }

        uint8_t getPin()
        {
            return (pin);
        }

        void setPin(uint8_t val)
        {
            pin = val;
        }

        uint8_t getValue()
        {
            return (value);
        }

        void setValue(uint8_t val)
        {
            value = val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t port;
        uint8_t pin;
        uint8_t value;

    } response_t;
}

} // namespace commands
