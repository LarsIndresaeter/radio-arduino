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

    uint64_t timestamp = command.getTimestamp();

    if(sleep_time_start_measurement == 0)
    {
        sleep_time_start_measurement = timestamp;
        sleep_time = 0;
    }

    uint64_t active_time = timestamp - sleep_time_start_measurement - sleep_time;

    response.setActive_time(active_time);
    response.setSleep_time(sleep_time);

    response.serialize(responsePayload);
}

} // namespace SLEEP
