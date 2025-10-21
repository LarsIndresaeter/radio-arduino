#pragma once

#include <adc.hpp>
#include <sha1.hpp>
#include <stdint.h>

namespace RANDOM {
uint8_t getRandomByte();
void mix();
void addEntropy(uint8_t e);
void addEntropyAndMix();
} // namespace
