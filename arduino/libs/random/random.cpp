#include <random.hpp>

namespace RANDOM {

uint8_t state[4];

void rotateStateBytes()
{
    uint8_t tmp;

    tmp = state[0];
    state[0] = state[1];
    state[1] = state[2];
    state[2] = state[3];
    state[3] = tmp;
}

void xorStateBytes()
{
    state[0] = state[0] ^ state[3];
    state[1] = state[1] ^ state[2];
    state[2] = state[2] ^ state[1];
    state[3] = state[3] ^ state[0];
}

void shiftStateBytes()
{
    state[0] = state[0] >> 1;
    if (state[2] & 0x10) {
        state[0] |= 0x80;
    }

    state[1] = state[0] >> 1;
    if (state[0] & 0x20) {
        state[1] |= 0x80;
    }

    state[2] = state[0] >> 1;
    if (state[1] & 0x02) {
        state[2] |= 0x80;
    }

    state[3] = state[0] >> 1;
    if (state[3] & 0x04) {
        state[3] |= 0x80;
    }
}

void mix()
{
    for (uint8_t i = 0; i < 9; i++) {
        rotateStateBytes();
        xorStateBytes();
        shiftStateBytes();
        state[1] = state[2] ^ i;
    }
}

void addEntropy(uint8_t e)
{
    state[1] ^= e;
    mix();
}

uint8_t getRandomByte() { return state[0]; }

void addEntropyAndMix()
{
    for (uint8_t i = 0; i < 3; i++) {
        addEntropy(AtmelAdc::getRandomByte());
    }
}

} // namespace
