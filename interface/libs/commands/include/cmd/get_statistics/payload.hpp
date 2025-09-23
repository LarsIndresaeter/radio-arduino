#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hpp>

namespace COMMANDS {

namespace GET_STATISTICS {
    constexpr uint8_t COMMAND_LENGTH = 0;
    constexpr uint8_t RESPONSE_LENGTH = 10;

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
            for (uint8_t i = 0; i < 2; i++) {
                uart_rx[i] = res[4 + i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                uart_tx[i] = res[6 + i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                rf_rx[i] = res[8 + i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                rf_tx[i] = res[10 + i];
            }
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            for (uint8_t i = 0; i < 2; i++) {
                response[2 + i] = commandsParsed[i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                response[4 + i] = uart_rx[i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                response[6 + i] = uart_tx[i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                response[8 + i] = rf_rx[i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                response[10 + i] = rf_tx[i];
            }
        }

        uint16_t getCommandsparsed()
        {
            return (((uint16_t)commandsParsed[1]) << 8 | commandsParsed[0]);
        }

        void setCommandsparsed(uint16_t value)
        {
            commandsParsed[1] = (uint8_t)(value >> 8);
            commandsParsed[0] = (uint8_t)value;
        }

        uint16_t getUart_rx()
        {
            return (((uint16_t)uart_rx[1]) << 8 | uart_rx[0]);
        }

        void setUart_rx(uint16_t value)
        {
            uart_rx[1] = (uint8_t)(value >> 8);
            uart_rx[0] = (uint8_t)value;
        }

        uint16_t getUart_tx()
        {
            return (((uint16_t)uart_tx[1]) << 8 | uart_tx[0]);
        }

        void setUart_tx(uint16_t value)
        {
            uart_tx[1] = (uint8_t)(value >> 8);
            uart_tx[0] = (uint8_t)value;
        }

        uint16_t getRf_rx()
        {
            return (((uint16_t)rf_rx[1]) << 8 | rf_rx[0]);
        }

        void setRf_rx(uint16_t value)
        {
            rf_rx[1] = (uint8_t)(value >> 8);
            rf_rx[0] = (uint8_t)value;
        }

        uint16_t getRf_tx()
        {
            return (((uint16_t)rf_tx[1]) << 8 | rf_tx[0]);
        }

        void setRf_tx(uint16_t value)
        {
            rf_tx[1] = (uint8_t)(value >> 8);
            rf_tx[0] = (uint8_t)value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t commandsParsed[2];
        uint8_t uart_rx[2];
        uint8_t uart_tx[2];
        uint8_t rf_rx[2];
        uint8_t rf_tx[2];

    } response_t;
}

} // namespace commands
