#pragma once

#include <i2c.hpp>
#include <stdint.h>

void readIna219(uint16_t* voltage, uint16_t* current);

