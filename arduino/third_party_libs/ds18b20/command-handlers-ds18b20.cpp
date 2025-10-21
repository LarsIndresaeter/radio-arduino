#include <cmd/payloads.hxx>
#include <command-handlers-ds18b20.hpp>
#include <ds18b20.h>

namespace COMMAND_HANDLERS {

void commandDs18b20(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::DS18B20::command_t command(commandPayload);
    COMMANDS::DS18B20::response_t response;

    int16_t temp;
    ds18b20convert(&PORTB, &DDRB, &PINB, (1 << 0), NULL);

    _delay_ms(1000);

    ds18b20read(&PORTB, &DDRB, &PINB, (1 << 0), NULL, &temp);

    response.setTemperature(temp);

    response.serialize(responsePayload);
}

} // namespace COMMAND_HANDLERS
