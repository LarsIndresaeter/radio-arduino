#include <util/delay.h>
#include <avr/io.h>
#include <gpio.hpp>

namespace GPIO
{
    void blink()
    {
        PORTB = PINB ^ 0x20;
        _delay_ms(DELAY_MS); 
        PORTB = PINB ^ 0x20;
        _delay_ms(DELAY_MS); 
    }

    uint8_t readPortB()
    {
        return PINB;
    }

    uint8_t readPortC()
    {
        return PINC;
    }

    uint8_t readPortD()
    {
        return PIND;
    }
}

