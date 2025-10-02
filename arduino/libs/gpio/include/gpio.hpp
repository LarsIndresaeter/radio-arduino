#pragma once

#include <stdint.h>

#define DELAY_MS 500

namespace GPIO
{
    void blink(void);
    uint8_t readPortB();
    uint8_t readPortC();
    uint8_t readPortD();
}

