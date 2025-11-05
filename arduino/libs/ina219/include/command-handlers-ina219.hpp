#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace INA219 {

void commandIna219(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace INA219
