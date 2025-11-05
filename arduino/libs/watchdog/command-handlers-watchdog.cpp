#include <cmd/payloads.hxx>
#include <command-handlers-watchdog.hpp>
#include <watchdog.hpp>

namespace WATCHDOG {

void commandSoftReset(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SOFT_RESET::command_t command(commandPayload);
    COMMANDS::SOFT_RESET::response_t response;

    WATCHDOG::softReset();
    response.serialize(responsePayload);
}

} // namespace WATCHDOG
