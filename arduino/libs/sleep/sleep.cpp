#include <sleep.hpp>

uint8_t node_address = 0;

uint8_t rf_link_discover_package[32]
        = { 'd', 'i', 's', 'c', 'o', 'v', 'e', 'r', ' ', 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, node_address, 1 };

uint8_t attention_flag = 0;

void powerSaveSleepMs(uint8_t delay_ms)
{
    cli();

    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;
    OCR2A = delay_ms<<4;
    TCCR2A |= (1 << WGM21);
    TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // clk/1024=16kHz
    TIMSK2 |= (1 << OCIE2A);

    // sleep until timer wake up the chip
    set_sleep_mode(SLEEP_MODE_EXT_STANDBY); // keep external oscillator enabled
    sleep_enable();
    sleep_bod_disable();
    sei();
    sleep_cpu();
    sleep_disable();
}



void powerDownRadioAndSleep(uint16_t delay)
{
#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
    NRF24L01_power_down();
#endif

    uint16_t i = 0;

    while (i < delay) {
        if ((delay - i) > 16) {
            powerSaveSleepMs(16);
            i += 16;
        }
        else {
            powerSaveSleepMs(delay - i);
            i = delay;
        }
        if(1 == attention_flag)
        {
            break; // wake up and send discover package 
        }
    }

#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
    NRF24L01_power_up();
#endif
}
