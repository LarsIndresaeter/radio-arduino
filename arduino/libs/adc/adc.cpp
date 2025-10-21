#include <adc.hpp>
#include <avr/io.h>
#include <util/delay.h>

namespace AtmelAdc {

uint16_t readVcc1()
{
    uint8_t oldADMUX = ADMUX;

    // V_ref=AVCC with external capacitor at AREF pin
    // Input Channel Selections = 1.1V (VBG)
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0); //  enable ADC, start conversion, clk/128

    while (((ADCSRA & (1 << ADSC)) != 0))
        ; // Wait for it to complete

    ADMUX = oldADMUX;

    uint32_t vcc = (1100 * 1023L) / ADC;

    return vcc;
}

uint8_t temperature(void)
{
    // https://microchipdeveloper.com/8avr:avradc

    ADMUX = (1 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (1 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);

    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADEN);

    /* Perform Dummy Conversion to complete ADC init */
    ADCSRA |= (1 << ADSC);

    /* wait for conversion to complete */
    while ((ADCSRA & (1 << ADSC)) != 0) {
    }

    /* start a new conversion on channel 8 */
    ADCSRA |= (1 << ADSC);

    /* wait for conversion to complete */
    while ((ADCSRA & (1 << ADSC)) != 0) {
    }

    return (ADC);
}

uint8_t getRandomByte()
{
    uint8_t retval = 0;

    for (uint8_t i = 0; i < 8; i++) {
        retval = retval << 1;
        retval |= temperature() & 0x01; // get LSB of internal temperature
    }

    return (retval);
}

uint16_t getAverageVcc()
{
    uint32_t vcc = 0;

    // discard first readings
    for (uint8_t i = 0; i < 32; i++) {
        AtmelAdc::readVcc1();
    }

    // average of measurements
    for (uint8_t i = 0; i < 32; i++) {
        vcc += AtmelAdc::readVcc1();
    }
    vcc = vcc >> 5;

    return (vcc);
}

}
