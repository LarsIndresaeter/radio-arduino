#include <random.hpp>

random::random() {}

void random::mix()
{
    SHA1Reset(&sha);
    SHA1Input(&sha, state, randomPoolSize);
    SHA1Result(&sha, state);
    readPos = state[3] % randomPoolSize;
    writePos = state[9] % randomPoolSize;
}

void random::addEntropy(uint8_t e)
{
    writePos = (writePos + 1) % randomPoolSize;
    state[writePos] = e;
}

uint8_t random::getRandomByte()
{
    readPos = (readPos + 1) % randomPoolSize;
    return state[readPos];
}

