#include <timer.hpp>

uint16_t rising_time = 0;
uint16_t falling_time = 0;
uint16_t pulse_width = 0;

ISR(TIMER1_CAPT_vect)
{
    if (TCCR1B & (1 << ICES1)) {
        TCNT1 = 0;
        rising_time = TCNT1;
        falling_time = 0;
        pulse_width = 0;
        TCCR1B &= ~(1 << ICES1); // input capture on falling edge
    }
    else {
        TCCR1B |= (1 << ICES1);
        falling_time = TCNT1;
        pulse_width = falling_time - rising_time;
    }
}

ISR(TIMER2_COMPA_vect)
{
    TIMSK2 = 0; // disable timer interrupt
}

namespace TIMER {
    void timerStart()
    {
        TCNT1 = 0;
        ICR1 = 0;
        rising_time = 0;
        falling_time = 0;
        pulse_width = 0;

        TCCR1B |= (1 << ICES1); // input capture set for rising edge
        TCCR1B |= (1 << CS10); // no prescaler
        TIMSK1 |= (1 << ICIE1); // input capture interrupt enable
    }

    void timerStop()
    {
        TIMSK1 &= ~(1 << ICIE1); // input capture interrupt disable
    }

    uint16_t getPulseWidthMicroSeconds()
    {
        return pulse_width >> 4;
    }
} // namespace
