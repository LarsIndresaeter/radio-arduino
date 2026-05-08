#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace SLEEP {

void commandSetAdvertisementInterval(uint8_t* commandPayload, uint8_t* responsePayload);
void commandSleep(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace SLEEP
