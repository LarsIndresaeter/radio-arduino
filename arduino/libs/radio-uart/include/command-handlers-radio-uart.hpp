#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace COMMAND_HANDLERS {

void commandRadioUart(
    uint8_t* commandPayload, uint8_t* responsePayload, ComBusInterface* comBus);

} // namespace COMMAND_HANDLERS

