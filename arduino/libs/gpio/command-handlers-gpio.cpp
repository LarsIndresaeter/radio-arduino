#include <cmd/payloads.hxx>
#include <command-handlers-gpio.hpp>
#include <gpio.hpp>

namespace GPIO {

void commandGpio(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::GPIO::command_t command(commandPayload);
    COMMANDS::GPIO::response_t response;

    response.setPortb(GPIO::readPortB());
    response.setPortc(GPIO::readPortC());
    response.setPortd(GPIO::readPortD());

    response.serialize(responsePayload);
}

void commandBlink(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::BLINK::command_t command(commandPayload);
    COMMANDS::BLINK::response_t response;

    GPIO::blink();

    response.serialize(responsePayload);
}

} // namespace GPIO
