#include <cmd/payloads.hxx>
#include <command-handlers-ina219.hpp>
#include <ina219.hpp>

namespace COMMAND_HANDLERS{

void commandIna219(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::INA219::command_t command(commandPayload);
    COMMANDS::INA219::response_t response;

    response.setVoltage(readIna219Voltage());
    response.setCurrent(readIna219Current());

    response.serialize(responsePayload);
}

} // namespace COMMAND_HANDLERS

