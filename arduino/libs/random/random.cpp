#include <random.hpp>

Random::Random() {}

void Random::mix()
{
    SHA1Reset(&sha);
    SHA1Input(&sha, state, randomPoolSize);
    SHA1Result(&sha, state);
    readPos = state[3] % randomPoolSize;
    writePos = state[9] % randomPoolSize;
}

void Random::addEntropy(uint8_t e)
{
    writePos = (writePos + 1) % randomPoolSize;
    state[writePos] = e;
}

uint8_t Random::getRandomByte()
{
    readPos = (readPos + 1) % randomPoolSize;
    return state[readPos];
}

void Random::addEntropyAndMix()
{
    addEntropy(AtmelAdc::getRandomByte());
    addEntropy(AtmelAdc::getRandomByte());
    mix();
    addEntropy(AtmelAdc::getRandomByte());
    addEntropy(AtmelAdc::getRandomByte());
    mix();
}
