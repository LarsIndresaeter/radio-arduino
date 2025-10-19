#pragma once

#include <i2c.hpp>
#include <stdint.h>

uint16_t readIna219Voltage(void);
uint16_t readIna219Current(void);
