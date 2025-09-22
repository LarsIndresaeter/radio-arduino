#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hpp>

namespace COMMANDS {

namespace GET_STATISTICS {
    constexpr uint8_t COMMAND_LENGTH = 0;
    constexpr uint8_t RESPONSE_LENGTH = 6;

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
                bytesReceived[i] = res[4 + i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                bytesSent[i] = res[6 + i];
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
                response[4 + i] = bytesReceived[i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                response[6 + i] = bytesSent[i];
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

        uint16_t getBytesreceived()
        {
            return (((uint16_t)bytesReceived[1]) << 8 | bytesReceived[0]);
        }

        void setBytesreceived(uint16_t value)
        {
            bytesReceived[1] = (uint8_t)(value >> 8);
            bytesReceived[0] = (uint8_t)value;
        }

        uint16_t getBytessent()
        {
            return (((uint16_t)bytesSent[1]) << 8 | bytesSent[0]);
        }

        void setBytessent(uint16_t value)
        {
            bytesSent[1] = (uint8_t)(value >> 8);
            bytesSent[0] = (uint8_t)value;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t commandsParsed[2];
        uint8_t bytesReceived[2];
        uint8_t bytesSent[2];

    } response_t;
}

} // namespace commands
