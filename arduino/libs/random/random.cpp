#include <random.hpp>

namespace RANDOM {

constexpr uint8_t randomPoolSize = 16;
SHA1Context sha;
uint8_t state[randomPoolSize];
uint8_t readPos = 0;
uint8_t writePos = 0;

void mix()
{
    SHA1Reset(&sha);
    SHA1Input(&sha, state, randomPoolSize);
    SHA1Result(&sha, state);
    readPos = state[3] % randomPoolSize;
    writePos = state[9] % randomPoolSize;
}

void addEntropy(uint8_t e)
{
    writePos = (writePos + 1) % randomPoolSize;
    state[writePos] = e;
}

uint8_t getRandomByte()
{
    readPos = (readPos + 1) % randomPoolSize;
    return state[readPos];
}

void addEntropyAndMix()
{
    addEntropy(AtmelAdc::getRandomByte());
    addEntropy(AtmelAdc::getRandomByte());
    mix();
    addEntropy(AtmelAdc::getRandomByte());
    addEntropy(AtmelAdc::getRandomByte());
    mix();
}

} // namespace
