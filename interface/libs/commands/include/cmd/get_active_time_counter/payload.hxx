#pragma once

// This file is generated with the script: `interface/libs/commands/generate.py`

#include <cmd/command_id.hxx>

namespace COMMANDS {

namespace GET_ACTIVE_TIME_COUNTER {
    constexpr uint8_t COMMAND_LENGTH = 8;
    constexpr uint8_t RESPONSE_LENGTH = 16;

    static_assert(COMMAND_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "COMMAND_LENGTH larger than max payload");
    static_assert(RESPONSE_LENGTH <= COMMANDS::MAX_PAYLOAD_LENGTH, "RESPONSE_LENGTH larger than max payload");

    typedef struct command {
        command()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::GET_ACTIVE_TIME_COUNTER);
            OL = COMMAND_LENGTH;
            for (uint8_t i = 0; i < 8; i++) {
                timestamp[i] = 0;
            }
        }

        command(uint8_t* cmd)
        {
            OI = cmd[0];
            OL = cmd[1];
            for (uint8_t i = 0; i < 8; i++) {
                timestamp[i] = cmd[2 + i];
            }
        }

        uint64_t getTimestamp()
        {
            return (((uint64_t)timestamp[7]) << 56 | ((uint64_t)timestamp[6]) << 48 | ((uint64_t)timestamp[5]) << 40 | ((uint64_t)timestamp[4]) << 32 | ((uint64_t)timestamp[3]) << 24 | ((uint64_t)timestamp[2]) << 16 | ((uint64_t)timestamp[1]) << 8 | timestamp[0]);
        }

        void setTimestamp(uint64_t val)
        {
            timestamp[7] = (uint8_t)(val >> 56);
            timestamp[6] = (uint8_t)(val >> 48);
            timestamp[5] = (uint8_t)(val >> 40);
            timestamp[4] = (uint8_t)(val >> 32);
            timestamp[3] = (uint8_t)(val >> 24);
            timestamp[2] = (uint8_t)(val >> 16);
            timestamp[1] = (uint8_t)(val >> 8);
            timestamp[0] = (uint8_t)val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t timestamp[8];
    } command_t;

    typedef struct response {
        response()
        {
            OI = static_cast<uint8_t>(COMMANDS::OI::GET_ACTIVE_TIME_COUNTER);
            OL = RESPONSE_LENGTH;
            for (uint8_t i = 0; i < 8; i++) {
                active_time[i] = 0;
            }
            for (uint8_t i = 0; i < 8; i++) {
                sleep_time[i] = 0;
            }
        }

        response(uint8_t* res)
        {
            OI = res[0];
            OL = res[1];
            for (uint8_t i = 0; i < 8; i++) {
                active_time[i] = res[2 + i];
            }
            for (uint8_t i = 0; i < 8; i++) {
                sleep_time[i] = res[10 + i];
            }
        }

        void serialize(uint8_t* response)
        {
            response[0] = OI;
            response[1] = OL;
            for (uint8_t i = 0; i < 8; i++) {
                response[2 + i] = active_time[i];
            }
            for (uint8_t i = 0; i < 8; i++) {
                response[10 + i] = sleep_time[i];
            }
        }

        uint64_t getActive_time()
        {
            return (((uint64_t)active_time[7]) << 56 | ((uint64_t)active_time[6]) << 48 | ((uint64_t)active_time[5]) << 40 | ((uint64_t)active_time[4]) << 32 | ((uint64_t)active_time[3]) << 24 | ((uint64_t)active_time[2]) << 16 | ((uint64_t)active_time[1]) << 8 | active_time[0]);
        }

        void setActive_time(uint64_t val)
        {
            active_time[7] = (uint8_t)(val >> 56);
            active_time[6] = (uint8_t)(val >> 48);
            active_time[5] = (uint8_t)(val >> 40);
            active_time[4] = (uint8_t)(val >> 32);
            active_time[3] = (uint8_t)(val >> 24);
            active_time[2] = (uint8_t)(val >> 16);
            active_time[1] = (uint8_t)(val >> 8);
            active_time[0] = (uint8_t)val;
        }

        uint64_t getSleep_time()
        {
            return (((uint64_t)sleep_time[7]) << 56 | ((uint64_t)sleep_time[6]) << 48 | ((uint64_t)sleep_time[5]) << 40 | ((uint64_t)sleep_time[4]) << 32 | ((uint64_t)sleep_time[3]) << 24 | ((uint64_t)sleep_time[2]) << 16 | ((uint64_t)sleep_time[1]) << 8 | sleep_time[0]);
        }

        void setSleep_time(uint64_t val)
        {
            sleep_time[7] = (uint8_t)(val >> 56);
            sleep_time[6] = (uint8_t)(val >> 48);
            sleep_time[5] = (uint8_t)(val >> 40);
            sleep_time[4] = (uint8_t)(val >> 32);
            sleep_time[3] = (uint8_t)(val >> 24);
            sleep_time[2] = (uint8_t)(val >> 16);
            sleep_time[1] = (uint8_t)(val >> 8);
            sleep_time[0] = (uint8_t)val;
        }

        uint8_t OI;
        uint8_t OL;
        uint8_t active_time[8];
        uint8_t sleep_time[8];

    } response_t;
}

} // namespace commands
