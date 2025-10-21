#pragma once

#include <avr/interrupt.h>
#include <stdint.h>

namespace QUADENCODER {
void initialize();
uint8_t clearChangedFlag();
uint8_t pollChangedFlag();
uint16_t getCountPositivePulses();
uint16_t getCountNegativePulses();
uint8_t getSwitchCount();
uint8_t getSwitchPosition();
}
