#include <sleep.hpp>

#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
    constexpr bool rx_mode_gateway = false;
#else
    constexpr bool rx_mode_gateway = true;
#endif

void powerSaveSleepMs(uint8_t delay_ms)
{
    cli();

    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;
    OCR2A = delay_ms << 4;
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

void rxNodeSleepAndPollForWakeup()
{
    // periodically poll rx gateway for wakeup command

    uint8_t wakeup_received = 0;

    while (wakeup_received == 0) {
        powerDownRadioAndSleep(5000);

        wakeup_received = sendDiscoverToGateway();
    }
}


void powerDownRadioAndSleep(uint16_t delay)
{
    if(false == rx_mode_gateway)
    {
        NRF24L01_power_down();
    }

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
        if (1 == attention_flag) {
            break; // wake up and send discover package
        }
    }

    if(false == rx_mode_gateway)
    {
        NRF24L01_power_up();
    }
}
