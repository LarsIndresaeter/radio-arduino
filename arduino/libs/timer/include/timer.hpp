#pragma once

#include <avr/interrupt.h>
#include <stdint.h>

namespace TIMER {
    void timerStart();
    void timerStop();
    uint16_t getPulseWidthMicroSeconds();
}
