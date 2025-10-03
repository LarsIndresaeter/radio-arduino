#include <sleep.hpp>

extern bool rx_mode_gateway;

uint8_t rf_link_discover_package[32]
    = { 'd', 'i', 's', 'c', 'o', 'v', 'e', 'r', ' ', 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
          0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, node_address, 1 };

uint8_t rf_link_wakeup_command[32]
    = { 'w', 'a', 'k', 'e', 'u', 'p', ' ', 0x55, 0x55, 0x55, 0x55,
          0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
          0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, node_address };

uint8_t attention_flag = 0;

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

    uint8_t read_wakeup_command[32] = { 0 };

    uint8_t wakeup_received = 0;

    while (wakeup_received == 0) {
        powerDownRadioAndSleep(5000);

        // send command to rf gateway
        rf_link_discover_package[31] = attention_flag;
        NRF24L01_tx(&rf_link_discover_package[0], 32);

        _delay_ms(10);

        // poll gateway for wakeup command in ack packet
        uint8_t length = NRF24L01_read_rx_payload(&read_wakeup_command[0]);

        if (length == 32) {
            wakeup_received = 1;
            for (uint8_t i = 0; i < 32; i++) {
                if (read_wakeup_command[i] != rf_link_wakeup_command[i]) {
                    wakeup_received = 0;
                }
            }
        }
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
