#include <avr/io.h>
#include <pwm.hpp>
#include <stdint.h>

namespace PWM
{

uint8_t write(uint8_t port, uint8_t pin, uint8_t value)
{
    uint8_t retval = 0;

    if (port == 'd' && pin == 3) {
        if (value == 0) {
            DDRD &= (0 << PD3); // disable PD3
        }
        else {
            if (value < 4) {
                return (0);
            }

            if (value > 19) {
                return (0);
            }

            DDRD |= (1 << PD3); // Fast PWM output at OC0A pin
            TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM20);
            TCCR2B = _BV(WGM22) | _BV(CS22) | _BV(CS21) | _BV(CS20); //
            OCR2A = 156;
            OCR2B = value; // 8 - 16
        }
        retval = 1;
    }

    if (port == 'b' && pin == 2) {
        if (value == 0) {
            DDRB &= (0 << PB2); // disable PB2
        }
        else {
            // < 1ms
            if (value < 100) {
                return (0);
            }

            // > 2 ms
            if (value > 200) {
                return (0);
            }

            DDRB |= (1 << PB2);
            TCCR1A = _BV(COM1A0) | _BV(COM1B1) | _BV(WGM11) | _BV(WGM10); // PWM phase correct
            TCCR1B =  _BV(CS11) | _BV(WGM13); // prescaler 010 = clk_io/8 = 1 MHz
            OCR1A = 20*1000; // 20 ms, 50Hz
            OCR1B = value*10; // <100, 200>
        }
        retval = 1;
    }

    return (retval);
}

} // namespace
