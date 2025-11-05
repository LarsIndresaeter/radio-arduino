#include <adc.hpp>
#include <cmd/payloads.hxx>
#include <command-handlers-adc.hpp>

namespace AtmelAdc {

void commandVcc(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::VCC::command_t command(commandPayload);
    COMMANDS::VCC::response_t response;

    response.setVcc(AtmelAdc::getAverageVcc());

    response.serialize(responsePayload);
}

} // namespace AtmelAdc
