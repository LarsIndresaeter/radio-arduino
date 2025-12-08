#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

namespace COMMANDS {

namespace GET_LSM303D {
    constexpr uint8_t COMMAND_LENGTH = 0;
    constexpr uint8_t RESPONSE_LENGTH = 6;

    static_assert(COMMAND_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::GET_LSM303D);
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
            OI = static_cast<uint8_t>(COMMANDS::OI::GET_LSM303D);
            OL = RESPONSE_LENGTH;
            for (uint8_t i = 0; i < 2; i++) {
                accelerometerX[i] = 0;
            }
            for (uint8_t i = 0; i < 2; i++) {
                accelerometerY[i] = 0;
            }
            for (uint8_t i = 0; i < 2; i++) {
                accelerometerZ[i] = 0;
            }
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            for (uint8_t i = 0; i < 2; i++) {
                accelerometerX[i] = res[2 + i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                accelerometerY[i] = res[4 + i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                accelerometerZ[i] = res[6 + i];
            }
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            for (uint8_t i = 0; i < 2; i++) {
                response[2 + i] = accelerometerX[i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                response[4 + i] = accelerometerY[i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                response[6 + i] = accelerometerZ[i];
            }
        }

        uint16_t getAccelerometerx()
        {
            return (((uint16_t)accelerometerX[1]) << 8 | accelerometerX[0]);
        }

        void setAccelerometerx(uint16_t val)
        {
            accelerometerX[1] = (uint8_t)(val >> 8);
            accelerometerX[0] = (uint8_t)val;
        }

        uint16_t getAccelerometery()
        {
            return (((uint16_t)accelerometerY[1]) << 8 | accelerometerY[0]);
        }

        void setAccelerometery(uint16_t val)
        {
            accelerometerY[1] = (uint8_t)(val >> 8);
            accelerometerY[0] = (uint8_t)val;
        }

        uint16_t getAccelerometerz()
        {
            return (((uint16_t)accelerometerZ[1]) << 8 | accelerometerZ[0]);
        }

        void setAccelerometerz(uint16_t val)
        {
            accelerometerZ[1] = (uint8_t)(val >> 8);
            accelerometerZ[0] = (uint8_t)val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t accelerometerX[2];
        uint8_t accelerometerY[2];
        uint8_t accelerometerZ[2];

    } response_t;
}

} // namespace commands
