#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace LSM303D {

void commandGetLsm303d(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace LSM303D
