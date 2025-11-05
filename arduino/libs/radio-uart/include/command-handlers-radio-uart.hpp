#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace RADIOUART {

void commandRadioUart(uint8_t* commandPayload, uint8_t* responsePayload, ComBusInterface* comBus);

} // namespace RADIOUART
