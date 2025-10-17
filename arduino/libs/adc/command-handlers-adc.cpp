#include <cmd/payloads.hxx>
#include <command-handlers-adc.hpp>
#include <adc.hpp>

namespace COMMAND_HANDLERS{

void commandVcc(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::VCC::command_t command(commandPayload);
    COMMANDS::VCC::response_t response;

    response.setVcc(AtmelAdc::getAverageVcc());

    response.serialize(responsePayload);
}

} // namespace COMMAND_HANDLERS

