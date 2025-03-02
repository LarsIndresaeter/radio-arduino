#pragma once

#include <stdint.h>

#define DELAY_MS 500

class gpio
{
public:
    gpio();
    void blink(void);
    uint8_t readPortB();
    uint8_t readPortC();
    uint8_t readPortD();
private:
    void wait();
};

