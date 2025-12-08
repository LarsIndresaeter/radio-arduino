#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

namespace COMMANDS {

namespace QUADRATURE_ENCODER {
    constexpr uint8_t COMMAND_LENGTH = 0;
    constexpr uint8_t RESPONSE_LENGTH = 7;

    static_assert(COMMAND_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::QUADRATURE_ENCODER);
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
            OI = static_cast<uint8_t>(COMMANDS::OI::QUADRATURE_ENCODER);
            OL = RESPONSE_LENGTH;
            for (uint8_t i = 0; i < 2; i++) {
                countnegative[i] = 0;
            }
            for (uint8_t i = 0; i < 2; i++) {
                countpositive[i] = 0;
            }
            switchposition = 0;
            for (uint8_t i = 0; i < 2; i++) {
                switchcount[i] = 0;
            }
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            for (uint8_t i = 0; i < 2; i++) {
                countnegative[i] = res[2 + i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                countpositive[i] = res[4 + i];
            }
            switchposition = res[6];
            for (uint8_t i = 0; i < 2; i++) {
                switchcount[i] = res[7 + i];
            }
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            for (uint8_t i = 0; i < 2; i++) {
                response[2 + i] = countnegative[i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                response[4 + i] = countpositive[i];
            }
            response[6] = switchposition;
            for (uint8_t i = 0; i < 2; i++) {
                response[7 + i] = switchcount[i];
            }
        }

        uint16_t getCountnegative()
        {
            return (((uint16_t)countnegative[1]) << 8 | countnegative[0]);
        }

        void setCountnegative(uint16_t val)
        {
            countnegative[1] = (uint8_t)(val >> 8);
            countnegative[0] = (uint8_t)val;
        }

        uint16_t getCountpositive()
        {
            return (((uint16_t)countpositive[1]) << 8 | countpositive[0]);
        }

        void setCountpositive(uint16_t val)
        {
            countpositive[1] = (uint8_t)(val >> 8);
            countpositive[0] = (uint8_t)val;
        }

        uint8_t getSwitchposition()
        {
            return (switchposition);
        }

        void setSwitchposition(uint8_t val)
        {
            switchposition = val;
        }

        uint16_t getSwitchcount()
        {
            return (((uint16_t)switchcount[1]) << 8 | switchcount[0]);
        }

        void setSwitchcount(uint16_t val)
        {
            switchcount[1] = (uint8_t)(val >> 8);
            switchcount[0] = (uint8_t)val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t countnegative[2];
        uint8_t countpositive[2];
        uint8_t switchposition;
        uint8_t switchcount[2];

    } response_t;
}

} // namespace commands
