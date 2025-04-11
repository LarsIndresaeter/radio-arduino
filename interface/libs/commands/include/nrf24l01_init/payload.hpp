#pragma once

#include <common/command_id.hpp>

namespace COMMANDS {

namespace NRF24L01_INIT {
    constexpr uint8_t ADDR_SIZE = 5;
    constexpr uint8_t COMMAND_LENGTH = 2*ADDR_SIZE + 2;
    constexpr uint8_t RESPONSE_LENGTH = 1;

    static_assert(COMMAND_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH < COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            for (int i = 0; i < ADDR_SIZE; i++) {
                tx_addr[i] = cmd[2 + i];
            }
            for (int j = 0; j < ADDR_SIZE; j++) {
                rx_addr[j] = cmd[7 + j];
            }
            rf_channel = cmd[12];
            gateway = cmd[13];
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t tx_addr[ADDR_SIZE];
        uint8_t rx_addr[ADDR_SIZE];
        uint8_t rf_channel;
        uint8_t gateway;
    } command_t;

    typedef struct response {
        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            status = res[2];
        }

        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::NRF24L01_INIT);
            OL = RESPONSE_LENGTH;
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            response[2] = status;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t status;

    } response_t;
}

} // namespace commands
