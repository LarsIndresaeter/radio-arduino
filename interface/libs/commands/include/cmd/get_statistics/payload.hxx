#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

namespace COMMANDS {

namespace GET_STATISTICS {
    constexpr uint8_t COMMAND_LENGTH = 0;
    constexpr uint8_t RESPONSE_LENGTH = 20;

    static_assert(COMMAND_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::GET_STATISTICS);
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
            OI = static_cast<uint8_t>(COMMANDS::OI::GET_STATISTICS);
            OL = RESPONSE_LENGTH;
            for (uint8_t i = 0; i < 2; i++) {
                commandsParsed[i] = 0;
            }
            for (uint8_t i = 0; i < 4; i++) {
                uartRx[i] = 0;
            }
            for (uint8_t i = 0; i < 4; i++) {
                uartTx[i] = 0;
            }
            for (uint8_t i = 0; i < 4; i++) {
                radioRx[i] = 0;
            }
            for (uint8_t i = 0; i < 4; i++) {
                radioTx[i] = 0;
            }
            for (uint8_t i = 0; i < 2; i++) {
                restarts[i] = 0;
            }
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            for (uint8_t i = 0; i < 2; i++) {
                commandsParsed[i] = res[2 + i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                uartRx[i] = res[4 + i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                uartTx[i] = res[8 + i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                radioRx[i] = res[12 + i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                radioTx[i] = res[16 + i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                restarts[i] = res[20 + i];
            }
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            for (uint8_t i = 0; i < 2; i++) {
                response[2 + i] = commandsParsed[i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                response[4 + i] = uartRx[i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                response[8 + i] = uartTx[i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                response[12 + i] = radioRx[i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                response[16 + i] = radioTx[i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                response[20 + i] = restarts[i];
            }
        }

        uint16_t getCommandsparsed()
        {
            return (((uint16_t)commandsParsed[1]) << 8 | commandsParsed[0]);
        }

        void setCommandsparsed(uint16_t val)
        {
            commandsParsed[1] = (uint8_t)(val >> 8);
            commandsParsed[0] = (uint8_t)val;
        }

        uint32_t getUartrx()
        {
            return (((uint32_t)uartRx[3]) << 24 | ((uint32_t)uartRx[2]) << 16 | ((uint32_t)uartRx[1]) << 8 | uartRx[0]);
        }

        void setUartrx(uint32_t val)
        {
            uartRx[3] = (uint8_t)(val >> 24);
            uartRx[2] = (uint8_t)(val >> 16);
            uartRx[1] = (uint8_t)(val >> 8);
            uartRx[0] = (uint8_t)val;
        }

        uint32_t getUarttx()
        {
            return (((uint32_t)uartTx[3]) << 24 | ((uint32_t)uartTx[2]) << 16 | ((uint32_t)uartTx[1]) << 8 | uartTx[0]);
        }

        void setUarttx(uint32_t val)
        {
            uartTx[3] = (uint8_t)(val >> 24);
            uartTx[2] = (uint8_t)(val >> 16);
            uartTx[1] = (uint8_t)(val >> 8);
            uartTx[0] = (uint8_t)val;
        }

        uint32_t getRadiorx()
        {
            return (((uint32_t)radioRx[3]) << 24 | ((uint32_t)radioRx[2]) << 16 | ((uint32_t)radioRx[1]) << 8 | radioRx[0]);
        }

        void setRadiorx(uint32_t val)
        {
            radioRx[3] = (uint8_t)(val >> 24);
            radioRx[2] = (uint8_t)(val >> 16);
            radioRx[1] = (uint8_t)(val >> 8);
            radioRx[0] = (uint8_t)val;
        }

        uint32_t getRadiotx()
        {
            return (((uint32_t)radioTx[3]) << 24 | ((uint32_t)radioTx[2]) << 16 | ((uint32_t)radioTx[1]) << 8 | radioTx[0]);
        }

        void setRadiotx(uint32_t val)
        {
            radioTx[3] = (uint8_t)(val >> 24);
            radioTx[2] = (uint8_t)(val >> 16);
            radioTx[1] = (uint8_t)(val >> 8);
            radioTx[0] = (uint8_t)val;
        }

        uint16_t getRestarts()
        {
            return (((uint16_t)restarts[1]) << 8 | restarts[0]);
        }

        void setRestarts(uint16_t val)
        {
            restarts[1] = (uint8_t)(val >> 8);
            restarts[0] = (uint8_t)val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t commandsParsed[2];
        uint8_t uartRx[4];
        uint8_t uartTx[4];
        uint8_t radioRx[4];
        uint8_t radioTx[4];
        uint8_t restarts[2];

    } response_t;
}

} // namespace commands
