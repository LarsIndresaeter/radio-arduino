#include <cmd/payloads.hxx>
#include <command-handlers-timer.hpp>
#include <timer.hpp>

namespace COMMAND_HANDLERS {

void commandTimer(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::TIMER::command_t command(commandPayload);
    COMMANDS::TIMER::response_t response;

    response.setPulsewidth(TIMER::getPulseWidthMicroSeconds());

    response.serialize(responsePayload);
}

} // namespace COMMAND_HANDLERS
