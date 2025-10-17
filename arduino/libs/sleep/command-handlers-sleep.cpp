#include <cmd/payloads.hxx>
#include <command-handlers-sleep.hpp>
#include <sleep.hpp>

namespace COMMAND_HANDLERS{

void commandSleep(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SLEEP::command_t command(commandPayload);
    COMMANDS::SLEEP::response_t response;

    SLEEP::powerDownRadioAndSleep(command.getDelay());

    response.serialize(responsePayload);
}

} // namespace COMMAND_HANDLERS

