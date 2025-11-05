#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace DS18B20 {

void commandDs18b20(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace DS18B20
