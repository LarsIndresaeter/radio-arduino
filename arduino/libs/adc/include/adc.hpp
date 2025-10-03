#pragma once

#include <stdint.h>

namespace AtmelAdc {
uint16_t readVcc1();
uint8_t temperature(void);
uint8_t getRandomByte();
uint16_t getAverageVcc();
}

