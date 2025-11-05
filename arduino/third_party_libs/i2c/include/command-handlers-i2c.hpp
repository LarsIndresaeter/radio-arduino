#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace LIBI2C {

void commandI2cWrite(uint8_t* commandPayload, uint8_t* responsePayload);
void commandI2cRead(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace LIBI2C
