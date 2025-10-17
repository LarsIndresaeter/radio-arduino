#include <cmd/payloads.hxx>
#include <command-handlers-ina219.hpp>
#include <ina219.hpp>

namespace COMMAND_HANDLERS{

void commandIna219(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::INA219::command_t command(commandPayload);
    COMMANDS::INA219::response_t response;

    uint16_t voltage, current;

    readIna219(&voltage, &current);

    response.setVoltage(voltage);
    response.setCurrent(current);

    response.serialize(responsePayload);
}

} // namespace COMMAND_HANDLERS

