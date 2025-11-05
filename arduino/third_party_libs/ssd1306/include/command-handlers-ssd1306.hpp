#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace LIBSSD1306 {

void commandSsd1306(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace LIBSSD1306
