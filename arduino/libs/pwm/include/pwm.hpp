#pragma once

#include <stdint.h>
#include <stdio.h>

namespace PWM {
uint8_t write(uint8_t port, uint8_t pin, uint8_t value);
}
