#include <cmd/payloads.hxx>
#include <command-handlers-quadencoder.hpp>
#include <quadencoder.hpp>

namespace COMMAND_HANDLERS {

void commandQuadratureEncoder(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::QUADRATURE_ENCODER::command_t command(commandPayload);
    COMMANDS::QUADRATURE_ENCODER::response_t response;

    QUADENCODER::initialize();

    response.setCountpositive(QUADENCODER::getCountPositivePulses());
    response.setCountnegative(QUADENCODER::getCountNegativePulses());
    response.setSwitchcount(QUADENCODER::getSwitchCount());
    response.setSwitchposition(QUADENCODER::getSwitchPosition());

    response.serialize(responsePayload);
}

} // namespace COMMAND_HANDLERS
