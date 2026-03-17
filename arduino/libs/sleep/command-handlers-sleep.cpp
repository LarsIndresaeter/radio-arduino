#include <cmd/payloads.hxx>
#include <command-handlers-sleep.hpp>
#include <sleep.hpp>

namespace SLEEP {
    
uint64_t sleep_time_start_measurement = 0;
extern uint64_t sleep_time;

void commandSleep(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SLEEP::command_t command(commandPayload);
    COMMANDS::SLEEP::response_t response;

    SLEEP::powerDownRadioAndSleep(command.getDelay());

    response.serialize(responsePayload);
}

void commandGetActiveTimeCounter(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::GET_ACTIVE_TIME_COUNTER::command_t command(commandPayload);
    COMMANDS::GET_ACTIVE_TIME_COUNTER::response_t response;

    uint64_t timestamp = (uint64_t)(command.timestamp[0]);
    timestamp += (uint64_t)(command.timestamp[1]) << 8;
    timestamp += (uint64_t)(command.timestamp[2]) << 16;
    timestamp += (uint64_t)(command.timestamp[3]) << 24;
    timestamp += (uint64_t)(command.timestamp[4]) << 32;
    timestamp += (uint64_t)(command.timestamp[5]) << 40;
    timestamp += (uint64_t)(command.timestamp[6]) << 48;
    timestamp += (uint64_t)(command.timestamp[7]) << 56;

    if(sleep_time_start_measurement == 0)
    {
        // reset sleep_time measurement and record the time
        sleep_time_start_measurement = timestamp;
        sleep_time = 0;
    }

    uint64_t active_time = timestamp - sleep_time_start_measurement - sleep_time;

    response.active_time[7] = active_time;
    response.active_time[6] = active_time >> 8;
    response.active_time[5] = active_time >> 16;
    response.active_time[4] = active_time >> 24;
    response.active_time[3] = active_time >> 32;
    response.active_time[2] = active_time >> 40;
    response.active_time[1] = active_time >> 48;
    response.active_time[0] = active_time >> 56;

    response.sleep_time[7] = sleep_time;
    response.sleep_time[6] = sleep_time >> 8;
    response.sleep_time[5] = sleep_time >> 16;
    response.sleep_time[4] = sleep_time >> 24;
    response.sleep_time[3] = sleep_time >> 32;
    response.sleep_time[2] = sleep_time >> 40;
    response.sleep_time[1] = sleep_time >> 48;
    response.sleep_time[0] = sleep_time >> 56;

    response.serialize(responsePayload);
}

} // namespace SLEEP
