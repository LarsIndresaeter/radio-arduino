#pragma once

#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

namespace TIMER {
    void timerStart();
    void timerStop();
    uint16_t getPulseWidthMicroSeconds();
}
