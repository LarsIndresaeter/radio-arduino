#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace LIBSPI {

void commandSpiRead(uint8_t* commandPayload, uint8_t* responsePayload);
void commandSpiWrite(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace LIBSPI
