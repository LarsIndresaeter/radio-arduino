#pragma once

#include <avr/io.h>
#include <stddef.h>
#include <util/delay.h>

namespace PWM {

void commandPwm(uint8_t* commandPayload, uint8_t* responsePayload);

} // namespace PWM
