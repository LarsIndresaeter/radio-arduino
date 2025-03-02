#pragma once

#include <stdint.h>
#include <stdio.h>

class pwm {
public:
    pwm();
    uint8_t write(uint8_t port, uint8_t pin, uint8_t value);
};
