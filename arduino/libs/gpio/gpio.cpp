#include <util/delay.h>
#include <avr/io.h>
#include <gpio.hpp>

void gpio::wait() { _delay_ms(DELAY_MS); }

gpio::gpio() {}

void gpio::blink()
{
    PORTB = PINB ^ 0x20;
    wait();
    PORTB = PINB ^ 0x20;
    wait();
}

uint8_t gpio::readPortB()
{
    return PINB;
}

uint8_t gpio::readPortC()
{
    return PINC;
}

uint8_t gpio::readPortD()
{
    return PIND;
}
