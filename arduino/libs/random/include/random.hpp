#pragma once

#include <sha1.hpp>
#include <adc.hpp>
#include <stdint.h>

constexpr uint8_t randomPoolSize = 16;

class Random {
public:
    Random();
    uint8_t getRandomByte();
    void mix();
    void addEntropy(uint8_t e);
    void addEntropyAndMix();

private:
    SHA1Context sha;
    uint8_t state[randomPoolSize];
    uint8_t readPos = 0;
    uint8_t writePos = 0;
};

