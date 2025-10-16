#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

namespace COMMANDS {

namespace GET_STATISTICS {
    constexpr uint8_t COMMAND_LENGTH = 0;
    constexpr uint8_t RESPONSE_LENGTH = 20;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

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
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            for (uint8_t i = 0; i < 2; i++) {
                commandsParsed[i] = res[2 + i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                uart_rx[i] = res[4 + i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                uart_tx[i] = res[8 + i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                rf_rx[i] = res[12 + i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                rf_tx[i] = res[16 + i];
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
                response[4 + i] = uart_rx[i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                response[8 + i] = uart_tx[i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                response[12 + i] = rf_rx[i];
            }
            for (uint8_t i = 0; i < 4; i++) {
                response[16 + i] = rf_tx[i];
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

        uint32_t getUart_rx()
        {
            return (((uint32_t)uart_rx[3]) << 24 | ((uint32_t)uart_rx[2]) << 16 | ((uint32_t)uart_rx[1]) << 8 | uart_rx[0]);
        }

        void setUart_rx(uint32_t val)
        {
            uart_rx[3] = (uint8_t)(val >> 24);
            uart_rx[2] = (uint8_t)(val >> 16);
            uart_rx[1] = (uint8_t)(val >> 8);
            uart_rx[0] = (uint8_t)val;
        }

        uint32_t getUart_tx()
        {
            return (((uint32_t)uart_tx[3]) << 24 | ((uint32_t)uart_tx[2]) << 16 | ((uint32_t)uart_tx[1]) << 8 | uart_tx[0]);
        }

        void setUart_tx(uint32_t val)
        {
            uart_tx[3] = (uint8_t)(val >> 24);
            uart_tx[2] = (uint8_t)(val >> 16);
            uart_tx[1] = (uint8_t)(val >> 8);
            uart_tx[0] = (uint8_t)val;
        }

        uint32_t getRf_rx()
        {
            return (((uint32_t)rf_rx[3]) << 24 | ((uint32_t)rf_rx[2]) << 16 | ((uint32_t)rf_rx[1]) << 8 | rf_rx[0]);
        }

        void setRf_rx(uint32_t val)
        {
            rf_rx[3] = (uint8_t)(val >> 24);
            rf_rx[2] = (uint8_t)(val >> 16);
            rf_rx[1] = (uint8_t)(val >> 8);
            rf_rx[0] = (uint8_t)val;
        }

        uint32_t getRf_tx()
        {
            return (((uint32_t)rf_tx[3]) << 24 | ((uint32_t)rf_tx[2]) << 16 | ((uint32_t)rf_tx[1]) << 8 | rf_tx[0]);
        }

        void setRf_tx(uint32_t val)
        {
            rf_tx[3] = (uint8_t)(val >> 24);
            rf_tx[2] = (uint8_t)(val >> 16);
            rf_tx[1] = (uint8_t)(val >> 8);
            rf_tx[0] = (uint8_t)val;
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
        uint8_t uart_rx[4];
        uint8_t uart_tx[4];
        uint8_t rf_rx[4];
        uint8_t rf_tx[4];
        uint8_t restarts[2];

    } response_t;
}

} // namespace commands
