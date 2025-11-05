#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace QUADENCODER {

void commandQuadratureEncoder(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace QUADENCODER
