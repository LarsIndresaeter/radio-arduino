#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace COMMAND_HANDLERS{

void commandWakeup(uint8_t* commandPayload, uint8_t* responsePayload);
void commandSetNodeAddress(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace COMMAND_HANDLERS

