#include <adc.hpp>
#include <avr/io.h>
#include <util/delay.h>

namespace AtmelAdc {

uint8_t temperature(void)
{
    // https://microchipdeveloper.com/8avr:avradc

    ADMUX = (1 << REFS1) | (1 << REFS0) | (0 << ADLAR) | (1 << MUX3)
        | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);

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

}
