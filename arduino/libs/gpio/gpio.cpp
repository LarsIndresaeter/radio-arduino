#include <util/delay.h>
#include <avr/io.h>
#include <gpio.hpp>

void Gpio::wait() { _delay_ms(DELAY_MS); }

Gpio::Gpio() {}

void Gpio::blink()
{
    PORTB = PINB ^ 0x20;
    wait();
    PORTB = PINB ^ 0x20;
    wait();
}

uint8_t Gpio::readPortB()
{
    return PINB;
}

uint8_t Gpio::readPortC()
{
    return PINC;
}

uint8_t Gpio::readPortD()
{
    return PIND;
}
