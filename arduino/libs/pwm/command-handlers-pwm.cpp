#include <cmd/payloads.hxx>
#include <command-handlers-pwm.hpp>
#include <pwm.hpp>

namespace PWM {

void commandPwm(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::PWM::command_t command(commandPayload);
    COMMANDS::PWM::response_t response;

    response.setPort(command.getPort());
    response.setPin(command.getPin());
    response.setValue(command.getValue());

    PWM::write(command.port, command.pin, command.value);

    response.serialize(responsePayload);
}

} // namespace PWM
