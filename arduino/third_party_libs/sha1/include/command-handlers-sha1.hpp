#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace SHA1 {

void commandSha1(uint8_t* commandPayload, uint8_t* responsePayload);
void commandHotp(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace SHA1
