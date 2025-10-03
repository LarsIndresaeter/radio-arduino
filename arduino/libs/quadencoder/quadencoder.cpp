#include <quadencoder.hpp>

bool quadratureEncoderIsInitialised = false;

uint16_t cnt_pos = 0;
uint16_t cnt_neg = 0;
uint16_t sw_cnt = 0;
uint8_t sw_pos = 0;
uint8_t pinc_prev;
uint8_t quadencoder_updated = 0;

namespace QUADENCODER {

ISR(PCINT1_vect)
{
    // PC0 = (CLK)
    // PC1 = (DT)
    // PC2 = (SW)
    cli(); // disable interrupt

    if ((PINC & 0x03) != pinc_prev) {
        if (((PINC & 0x03) == 0x00) || ((PINC & 0x03) == 0x03)) {
            cnt_pos++;
        }
        else {
            cnt_neg++;
        }
    }

    pinc_prev = PINC & 0x03;

    if (PINC & 0x04) {
        if (sw_pos == 0) {
            sw_cnt++;
        }

        sw_pos = 1;
    }
    else {
        if (sw_pos == 1) {
            sw_cnt++;
        }

        sw_pos = 0;
    }

    quadencoder_updated = 1;

    sei();
}

uint8_t isChanged()
{
    uint8_t retval = quadencoder_updated;
    quadencoder_updated = 0;

    return retval;
}

void initialize()
{
    if (!quadratureEncoderIsInitialised) {
        quadratureEncoderIsInitialised = true;

        DDRC &= ~(1 << PC0); // set PC0 input (CLK)
        DDRC &= ~(1 << PC1); // set PC1 input (DT)
        DDRC &= ~(1 << PC2); // set PC2 input (SW)
        PCICR = 0x02; // enable PCINT1
        PCMSK1 = 0x05; // enable pin PCINT8 (PC0) and PCINT10 (PC2)
                       // seher
        PORTC |= 0x07; // enable pull-up resistor

        sei();
    }
}

uint16_t getCountPositivePulses() { return cnt_pos; }
uint16_t getCountNegativePulses() { return cnt_neg; }
uint8_t getSwitchCount() { return sw_cnt; }
uint8_t getSwitchPosition() { return sw_pos; }

} // namespace
