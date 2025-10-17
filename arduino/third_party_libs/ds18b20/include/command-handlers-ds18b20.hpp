#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace COMMAND_HANDLERS{

void commandDs18b20(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace COMMAND_HANDLERS

