#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

namespace COMMANDS {

namespace SCAN_FOR_ADVERTISEMENT {
    constexpr uint8_t COMMAND_LENGTH = 6;
    constexpr uint8_t RESPONSE_LENGTH = 25;

    static_assert(COMMAND_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SCAN_FOR_ADVERTISEMENT);
            OL = COMMAND_LENGTH;
            for (uint8_t i = 0; i < 4; i++) {
                id[i] = 0;
            }
            for (uint8_t i = 0; i < 2; i++) {
                timeout[i] = 0;
            }
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            for (uint8_t i = 0; i < 4; i++) {
                id[i] = cmd[2 + i];
            }
            for (uint8_t i = 0; i < 2; i++) {
                timeout[i] = cmd[6 + i];
            }
        }

        uint32_t getId()
        {
            return (((uint32_t)id[3]) << 24 | ((uint32_t)id[2]) << 16 | ((uint32_t)id[1]) << 8 | id[0]);
        }

        void setId(uint32_t val)
        {
            id[3] = (uint8_t)(val >> 24);
            id[2] = (uint8_t)(val >> 16);
            id[1] = (uint8_t)(val >> 8);
            id[0] = (uint8_t)val;
        }

        uint16_t getTimeout()
        {
            return (((uint16_t)timeout[1]) << 8 | timeout[0]);
        }

        void setTimeout(uint16_t val)
        {
            timeout[1] = (uint8_t)(val >> 8);
            timeout[0] = (uint8_t)val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t id[4];
        uint8_t timeout[2];
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::SCAN_FOR_ADVERTISEMENT);
            OL = RESPONSE_LENGTH;
            for (uint8_t i = 0; i < 4; i++) {
                sequence_number[i] = 0;
            }
            for (uint8_t i = 0; i < 16; i++) {
                data[i] = 0;
            }
            flags = 0;
            for (uint8_t i = 0; i < 4; i++) {
                id[i] = 0;
            }
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            for (uint8_t i = 0; i < 4; i++) {
                sequence_number[i] = res[2 + i];
            }
            for (uint8_t i = 0; i < 16; i++) {
                data[i] = res[6 + i];
            }
            flags = res[22];
            for (uint8_t i = 0; i < 4; i++) {
                id[i] = res[23 + i];
            }
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            for (uint8_t i = 0; i < 4; i++) {
                response[2 + i] = sequence_number[i];
            }
            for (uint8_t i = 0; i < 16; i++) {
                response[6 + i] = data[i];
            }
            response[22] = flags;
            for (uint8_t i = 0; i < 4; i++) {
                response[23 + i] = id[i];
            }
        }

        uint32_t getSequence_number()
        {
            return (((uint32_t)sequence_number[3]) << 24 | ((uint32_t)sequence_number[2]) << 16 | ((uint32_t)sequence_number[1]) << 8 | sequence_number[0]);
        }

        void setSequence_number(uint32_t val)
        {
            sequence_number[3] = (uint8_t)(val >> 24);
            sequence_number[2] = (uint8_t)(val >> 16);
            sequence_number[1] = (uint8_t)(val >> 8);
            sequence_number[0] = (uint8_t)val;
        }

        uint8_t getFlags()
        {
            return (flags);
        }

        void setFlags(uint8_t val)
        {
            flags = val;
        }

        uint32_t getId()
        {
            return (((uint32_t)id[3]) << 24 | ((uint32_t)id[2]) << 16 | ((uint32_t)id[1]) << 8 | id[0]);
        }

        void setId(uint32_t val)
        {
            id[3] = (uint8_t)(val >> 24);
            id[2] = (uint8_t)(val >> 16);
            id[1] = (uint8_t)(val >> 8);
            id[0] = (uint8_t)val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t sequence_number[4];
        uint8_t data[16];
        uint8_t flags;
        uint8_t id[4];

    } response_t;
}

} // namespace commands
