#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace LIBWS2812B {

void commandWs2812b(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace LIBWS2812B
