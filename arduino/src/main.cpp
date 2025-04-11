#include <gpio.hpp>
#include <protocol.hpp>
#include <radio_uart.hpp>
#include <stdio.h>
#include <uart.hpp>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include <adc.hpp>
#include <aes.hpp>
#include <arduinoCryptoHandler.hpp>
#include <eeprom.hpp>
#include <gpio.hpp>
#include <i2c.hpp>
#include <nrf24l01.hpp>
#include <payloads.hpp>
#include <pwm.hpp>
#include <random.hpp>
#include <sha1.hpp>
#include <spi.hpp>

#include <Framebuffer.hpp>
#include <avr/sleep.h>
#include <ds18b20.h>
#include <onewire.h>
#include <romsearch.h>
#include <stdio.h>
#include <version.h>
#include <ws2812b.hpp>

aes m_aes;
random m_random;
uint8_t protocolVersionLastReceivedMessage
    = static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::UNDEFINED);

#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
bool rx_mode_gateway = false;
#else
bool rx_mode_gateway = true;
#endif

uint8_t node_address = 0;
uint8_t rx_tx_addr[5] = { 0xF0, 0xF0, 0xF0, 0xF0, node_address };
uint8_t rf_channel = 121;

uint8_t rf_link_wakeup_command[32] 
        = {'w', 'a', 'k', 'e', 'u', 'p', ' ', 0x55, 0x55, 0x55, 0x55, 
            0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
            0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, node_address};

uint8_t rf_link_discover_package[32]
        = { 'd', 'i', 's', 'c', 'o', 'v', 'e', 'r', ' ', 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
            0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, node_address };

uint32_t keep_alive_interval_ms = 100; // time in idle loop before entering sleep

#ifdef USE_NRF24L01_INTTERRUPT
ISR(PCINT0_vect)
{
    uint8_t rx_buf[NRF24L01_PACKET_SIZE] = { 0 };

    NRF24L01_write_register(
        NRF24L01_REGISTER_STATUS, 0x70); // clear RX_DR, TX_DS and MAX_TR

    uint8_t status = NRF24L01_read_register(NRF24L01_REGISTER_STATUS);

    if ((status & 0x0E) != 0x0E) { // rx_fifo not empty
        uint8_t read_length
            = NRF24L01_rx(&rx_buf[0]);

        for (uint8_t i = 0; i < read_length; i++) {
            NRF24L01::rb_put(rx_buf[i]);
        }
        NRF24L01_write_register(
            NRF24L01_REGISTER_STATUS,
            0x70); // clear RX_DR, TX_DS and MAX_TR
    }
}
#endif

void sendMessage(protocol m_protocol, comBusInterface* comBus, uint8_t* payload)
{
    uint8_t packet[COMMANDS::MAX_PACKAGE_LENGTH];
    uint8_t length = payload[1] + 2;

    if (protocolVersionLastReceivedMessage
            == static_cast<uint8_t>(
                PROTOCOL::HEADER::VERSION::ENCRYPTED_BINARY_AND_TEXT)
        || ((rx_mode_gateway == false)
            && protocolVersionLastReceivedMessage
                == static_cast<uint8_t>(
                    PROTOCOL::HEADER::VERSION::RADIO_ENCRYPTED_BINARY_AND_TEXT))

    ) {
        m_protocol.createEncryptedPacket(
            length, payload, &packet[0], protocolVersionLastReceivedMessage);

#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
        NRF24L01_tx(
            &packet[0],
            PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length
                + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD);
#else
        comBus->writeBuffer(
            &packet[0],
            PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length
                + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD);
#endif
    }
    else if (
        protocolVersionLastReceivedMessage
            == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::BINARY_AND_TEXT)
        || ((rx_mode_gateway == false)
            && protocolVersionLastReceivedMessage
                == static_cast<uint8_t>(
                    PROTOCOL::HEADER::VERSION::RADIO_BINARY_AND_TEXT))) {
        m_protocol.createPacket(
            length, payload, &packet[0], protocolVersionLastReceivedMessage);

#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
        NRF24L01_tx(
            &packet[0],
            PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length);
#else
        comBus->writeBuffer(
            &packet[0],
            PROTOCOL::HEADER::LENGTH + PROTOCOL::CHECKSUM::LENGTH + length);
#endif
    }
}

void commandDs18b20(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::DS18B20::command_t command(commandPayload);
    COMMANDS::DS18B20::response_t response;

    int16_t temp;
    ds18b20convert(&PORTB, &DDRB, &PINB, (1 << 0), NULL);

    _delay_ms(1000);

    response.status = ds18b20read(&PORTB, &DDRB, &PINB, (1 << 0), NULL, &temp);

    response.temperature[0] = temp >> 8;
    response.temperature[1] = temp;

    response.serialize(responsePayload);
}

void commandBlink(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::BLINK::command_t command(commandPayload);
    COMMANDS::BLINK::response_t response;

    gpio m_gpio;
    m_gpio.blink();

    response.serialize(responsePayload);
}

void commandRandom(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::RANDOM::response_t response;

    m_random.addEntropy(AtmelAdc::getRandomByte());
    m_random.addEntropy(AtmelAdc::getRandomByte());
    m_random.mix();
    m_random.addEntropy(AtmelAdc::getRandomByte());
    m_random.addEntropy(AtmelAdc::getRandomByte());
    m_random.mix();

    for (uint8_t i = 0; i < 16; i++) {
        response.data[i] = m_random.getRandomByte();
    }

    response.serialize(responsePayload);
}

void commandSha1(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SHA1::command_t command(commandPayload);
    COMMANDS::SHA1::response_t response;
    SHA1Context sha;

    SHA1Reset(&sha);
    SHA1Input(&sha, command.data, command.OL);
    SHA1Result(&sha, response.data);

    response.serialize(responsePayload);
}

void commandHotp(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::HOTP::command_t command(commandPayload);
    COMMANDS::HOTP::response_t response;
    SHA1Context sha;

    static uint16_t cnt = 0;
    uint8_t HOTP_message[16];

    HOTP_message[0] = cnt >> 8;
    HOTP_message[1] = cnt++;
    HOTP_message[2] = 's';
    HOTP_message[3] = 'e';
    HOTP_message[4] = 'c';
    HOTP_message[5] = 'r';
    HOTP_message[6] = 'e';
    HOTP_message[7] = 't';

    SHA1Reset(&sha);
    SHA1Input(&sha, &HOTP_message[0], 8);
    SHA1Result(&sha, response.data);

    response.serialize(responsePayload);
}

void commandAes(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::AES::command_t command(commandPayload);
    COMMANDS::AES::response_t response;

    eeprom m_eeprom;

    uint8_t aes_key[16] = {};
    for (uint8_t i = 0; i < 16; i++) {
        aes_key[i] = m_eeprom.read(offsetof(eeprom_data_t, EK_KEY) + i);
    }

    uint8_t aes_iv[16] = {};

    // copy data to response buffer
    for (int i = 0; i < 16; i++) {
        response.data[i] = command.data[i];
    }

    if (command.type == 'c') {
        m_aes.Crypt(response.data, &aes_key[0], &aes_iv[0]);
    }

    if (command.type == 'd') {
        m_aes.Decrypt(response.data, &aes_key[0], &aes_iv[0]);
    }

    response.type = command.type;

    response.serialize(responsePayload);
}

void powerSaveSleepMs(uint8_t delay_ms)
{
    if (delay_ms > 16) {
        delay_ms = 16;
    }

    cli();

    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;
    OCR2A = 16 * delay_ms;
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

    uint32_t i = 0;
    if (delay > 0) {
        powerSaveSleepMs(1);
        i++;
    }
    while (i < delay) {
        if ((delay - i) > 16) {
            powerSaveSleepMs(16);
            i += 16;
        }
        else {
            powerSaveSleepMs(delay - i);
            i = delay;
        }
    }

#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
    NRF24L01_power_up();
#endif
}

void commandSleep(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SLEEP::command_t command(commandPayload);
    COMMANDS::SLEEP::response_t response;

    uint32_t delay = (uint32_t)(command.delay_0) << 24;
    delay += (uint32_t)(command.delay_1) << 16;
    delay += (uint32_t)(command.delay_2) << 8;
    delay += command.delay_3;

    powerDownRadioAndSleep(delay);

    response.status = 1;

    response.serialize(responsePayload);
}

void commandPwm(uint8_t* commandPayload, uint8_t* responsePayload)
{
    pwm m_pwm;
    COMMANDS::PWM::command_t command(commandPayload);
    COMMANDS::PWM::response_t response;

    response.port = command.port;
    response.pin = command.pin;
    response.value = command.value;

    m_pwm.write(command.port, command.pin, command.value);

    response.serialize(responsePayload);
}

void commandGpio(uint8_t* commandPayload, uint8_t* responsePayload)
{
    gpio m_gpio;
    COMMANDS::GPIO::command_t command(commandPayload);
    COMMANDS::GPIO::response_t response;

    response.portB = m_gpio.readPortB();
    response.portC = m_gpio.readPortC();
    response.portD = m_gpio.readPortD();

    response.serialize(responsePayload);
}

void commandSsd1306(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SSD1306::command_t command(commandPayload);
    COMMANDS::SSD1306::response_t response;

    Framebuffer fb;

    for (uint8_t x = 0; x < COMMANDS::SSD1306::STRING_LENGTH; x++) {
        fb.drawChar(x, command.line, command.data[x]);
    }

    fb.show();

    // Invert uses direct hardware commands
    // So no need to send the framebuffer again
    // => no need to fb.show();
    fb.invert(0);

    response.serialize(responsePayload);
}

uint16_t rising_time = 0;
uint16_t falling_time = 0;
uint16_t pulse_width = 0;

ISR(TIMER1_CAPT_vect)
{
    if (TCCR1B & (1 << ICES1)) {
        TCNT1 = 0;
        // rising_time = ICR1;
        rising_time = TCNT1;
        falling_time = 0;
        pulse_width = 0;
        TCCR1B &= ~(1 << ICES1); // input capture on falling edge
    }
    else {
        TCCR1B |= (1 << ICES1);
        falling_time = TCNT1;
        pulse_width = falling_time - rising_time;

        // TIMSK1 &= ~(1 << ICIE1); // input capture interrupt disable
        // cli();
    }
}

ISR(TIMER2_COMPA_vect)
{
    TIMSK2 = 0; // disable timer interrupt
}

void timerStart()
{
    TCNT1 = 0;
    ICR1 = 0;
    rising_time = 0;
    falling_time = 0;
    pulse_width = 0;

    // DDRB &= (0 << PB0); // set ICP1/PB0 as input
    // PORTB &= (0<<PB0); // disable pull-up resistor

    TCCR1B |= (1 << ICES1); // input capture set for rising edge
    TCCR1B |= (1 << CS10);  // no prescaler
    TIMSK1 |= (1 << ICIE1); // input capture interrupt enable
    // sei();
}

void timerStop()
{
    // TIMSK1 ^= (1 << ICIE1); // input capture interrupt disable
    TIMSK1 &= ~(1 << ICIE1); // input capture interrupt disable
    // cli();
}

void commandTimer(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::TIMER::command_t command(commandPayload);
    COMMANDS::TIMER::response_t response;

    _delay_ms(10);
    timerStart();
    _delay_ms(25);
    timerStop();

    pulse_width = pulse_width >> 4; // divide by 16 to get micro seconds

    response.pulse_width_high = pulse_width >> 8;
    response.pulse_width_low = pulse_width;

    response.serialize(responsePayload);
}

uint16_t readVcc1()
{
    uint8_t oldADMUX = ADMUX;

    // V_ref=AVCC with external capacitor at AREF pin
    // Input Channel Selections = 1.1V (VBG)
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);

    ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS2) | _BV(ADPS1)
        | _BV(ADPS0); //  enable ADC, start conversion, clk/128

    while (((ADCSRA & (1 << ADSC)) != 0))
        ; // Wait for it to complete

    ADMUX = oldADMUX;

    uint32_t vcc = (1100 * 1023L) / ADC;

    return vcc;
}

void commandVcc(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::VCC::command_t command(commandPayload);
    COMMANDS::VCC::response_t response;

    uint32_t vcc = 0;

    // discard first readings
    for (uint8_t i = 0; i < 32; i++) {
        readVcc1();
    }
                      
    // average of measurements
    for (uint8_t i = 0; i < 32; i++) {
        vcc += readVcc1();
    }
    vcc = vcc >> 5;
    
    response.vcc_h = vcc >> 8;
    response.vcc_l = vcc;

    response.serialize(responsePayload);
}

void commandDebug(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::DEBUG::command_t command(commandPayload);
    COMMANDS::DEBUG::response_t response;

    for (int i = 0; i < COMMANDS::DEBUG::RESPONSE_LENGTH; i++) {
        response.data[i] = i;
    }

    response.serialize(responsePayload);
}

void commandEepromRead(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::EEPROM_READ::command_t command(commandPayload);
    COMMANDS::EEPROM_READ::response_t response;
    eeprom m_eeprom;

    response.addressHigh = command.addressHigh;
    response.addressLow = command.addressLow;
    response.data
        = m_eeprom.read(command.addressHigh * 256 + command.addressLow);

    response.serialize(responsePayload);
}

void commandWs2812b(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::WS2812B::command_t command(commandPayload);
    COMMANDS::WS2812B::response_t response;

    ws2812b m_ws2812b;

    rgb_color colors[COMMANDS::WS2812B::LEDS];

    for (uint16_t i = 0; i < COMMANDS::WS2812B::LEDS; i++) {
        colors[i].red = command.data[i].red;
        colors[i].green = command.data[i].green;
        colors[i].blue = command.data[i].blue;
    }

    m_ws2812b.led_strip_write(colors, COMMANDS::WS2812B::LEDS);

    response.serialize(responsePayload);
}

void commandEepromWrite(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::EEPROM_WRITE::command_t command(commandPayload);
    COMMANDS::EEPROM_WRITE::response_t response;
    eeprom m_eeprom;

    m_eeprom.write(
        command.addressHigh * 256 + command.addressLow, command.data);

    response.addressHigh = command.addressHigh;
    response.addressLow = command.addressLow;
    response.data
        = m_eeprom.read(command.addressHigh * 256 + command.addressLow);

    response.serialize(responsePayload);
}

void commandI2cWrite(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::I2C_WRITE::command_t command(commandPayload);
    COMMANDS::I2C_WRITE::response_t response;

    I2C_Init();
    I2C_Start(command.device);       // write address
    I2C_Write(command.registerLow);  // first word address
    I2C_Write(command.registerHigh); // second word address
    for (int i = 0;
         (i < command.length) && (i < COMMANDS::I2C_READ::MAX_DATA_LENGTH);
         i++) {
        response.status = I2C_Write(command.data[i]);
        if (0 != response.status) {
            break;
        }
    }
    I2C_Stop();

    response.serialize(responsePayload);
}

void commandSetKey(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SET_KEY::command_t command(commandPayload);
    COMMANDS::SET_KEY::response_t response;
    eeprom m_eeprom;

    uint16_t address = 0;

    if (command.key_id == COMMANDS::SET_KEY::TK) {
        address = offsetof(eeprom_data, TK_KEY);
    }
    else if (command.key_id == COMMANDS::SET_KEY::HK) {
        address = offsetof(eeprom_data_t, HMAC_KEY);
    }
    else if (command.key_id == COMMANDS::SET_KEY::HOTP_KEY) {
        address = offsetof(eeprom_data_t, HOTP_KEY);
    }
    else if (command.key_id == COMMANDS::SET_KEY::EK) {
        address = offsetof(eeprom_data_t, EK_KEY);
    }

    if (command.key_id != COMMANDS::SET_KEY::UNKNOWN_KEY_ID) {
        for (uint8_t i = 0; i < 16; i++) {
            m_eeprom.write(address + i, command.key_value[i]);
        }
        response.status = 1;
    }

    response.status = 0;

    response.serialize(responsePayload);
}

void commandSetDeviceInfo(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SET_DEVICE_INFO::command_t command(commandPayload);
    COMMANDS::SET_DEVICE_INFO::response_t response;
    eeprom m_eeprom;

    for (uint8_t i = 0; i < 16; i++) {
        m_eeprom.write(offsetof(eeprom_data_t, NAME) + i, command.name[i]);
    }
    response.status = 1;

    response.serialize(responsePayload);
}

void commandGetDeviceInfo(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::GET_DEVICE_INFO::command_t command(commandPayload);
    COMMANDS::GET_DEVICE_INFO::response_t response;
    eeprom m_eeprom;

    for (uint8_t i = 0; i < 16; i++) {
        response.name[i] = m_eeprom.read(offsetof(eeprom_data_t, NAME) + i);
    }

    for (int i = 0; i < 32; i++) {
        response.version[i] = 0;
    }

    for (int i = 0; i < 32 && ARDUINO_VERSION[i] != 0; i++) {
        response.version[i] = ARDUINO_VERSION[i];
    }

    response.status = 1;

    response.serialize(responsePayload);
}

void commandI2cRead(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::I2C_READ::command_t command(commandPayload);
    COMMANDS::I2C_READ::response_t response;

    response.device = command.device;
    response.registerHigh = command.registerHigh;
    response.registerLow = command.registerLow;
    response.length = command.length;

    I2C_Init();
    I2C_Start(command.device);                        // read address
    response.status = I2C_Write(command.registerLow); // first word address
    // if(0 != response.status)
    //{
    response.status = I2C_Write(command.registerHigh); // second word address
                                                       //}
    // if(0 == response.status)
    //{
    response.status = I2C_Repeated_Start(command.device + 1);
    // if(0 == response.status)
    //{
    for (int i = 0;
         (i < command.length) && (i < COMMANDS::I2C_READ::MAX_DATA_LENGTH);
         i++) {
        response.data[i] = I2C_Read_Ack();
    }
    //}
    //}
    I2C_Read_Nack(); /* Read flush data with nack */
    I2C_Stop();

    response.serialize(responsePayload);
}

void commandSpiRead(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SPI_READ::command_t command(commandPayload);
    COMMANDS::SPI_READ::response_t response;

    response.reg = command.reg;
    response.length = command.length;

    SPI_init();

    SPI_ChipSelectLow();

    SPI_masterTransmitByte(command.reg);

    for (uint8_t i = 0;
         i < command.length && i < COMMANDS::SPI_READ::MAX_DATA_LENGTH;
         i++) {
        response.data[i] = SPI_masterReceive();
    }

    SPI_ChipSelectHigh();

    response.serialize(responsePayload);
}

void commandSpiWrite(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SPI_WRITE::command_t command(commandPayload);
    COMMANDS::SPI_WRITE::response_t response;

    SPI_init();

    SPI_ChipSelectLow();

    SPI_masterTransmitByte(0x20 | command.reg);

    for (uint8_t i = 0;
         i < command.length && i < COMMANDS::SPI_WRITE::MAX_DATA_LENGTH;
         i++) {
        SPI_masterTransmitByte(command.data[i]);
    }

    SPI_ChipSelectHigh();

    response.status = 1;

    response.serialize(responsePayload);
}

void commandRadioUart(
    uint8_t* commandPayload, uint8_t* responsePayload, comBusInterface* comBus)
{
    COMMANDS::RADIO_UART::command_t command(commandPayload);
    COMMANDS::RADIO_UART::response_t response;

    if (command.mode == 'p') // promiscous mode
    {
        NRF24L01_write_register(NRF24L01_REGISTER_CONFIG, 0x03);
        // illegal address, undocumented 2 byte address
        NRF24L01_write_register(NRF24L01_REGISTER_SETUP_AW, 0x00);

        uint8_t rf_channel = 125;
        uint8_t addr[5] = { 0, 0, 0, 0, 0x55 };

        NRF24L01_init(&addr[0], &addr[0], rf_channel, false);
    }
    else if (command.mode == 's') // send data read from uart over radio
    {
        radioUart uartRadio;

        uint8_t package[32] = { 0 };
        uint8_t len = 0;
        uint8_t c;

        while (true) {
            while (comBus->has_data()) {
                c = comBus->getChar();
                package[len++] = c;

                if (c == ' ' || len >= 32) {
                    uartRadio.writeBuffer(&package[0], len);
                    len = 0;
                }
            }
        }
    }
    else if (command.mode == 'r') // receive data from radio and write to uart
    {
        radioUart uartRadio;

        while (true) {
            if (uartRadio.has_data()) {
                comBus->putChar(uartRadio.getChar());
            }
        }
    }

    response.status = command.mode;

    response.serialize(responsePayload);
}

void commandNrf24l01Init(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::NRF24L01_INIT::command_t command(commandPayload);
    COMMANDS::NRF24L01_INIT::response_t response;

    NRF24L01_init(
        &command.rx_addr[0],
        &command.tx_addr[0],
        command.rf_channel,
        command.gateway == 1);

    response.status = 1;

    response.serialize(responsePayload);
}

void commandNrf24l01Read(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::NRF24L01_READ::command_t command(commandPayload);
    COMMANDS::NRF24L01_READ::response_t response;

    SPI_init();

    response.length
        = NRF24L01_rx(response.data);

    response.serialize(responsePayload);
}

void commandNrf24l01Write(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::NRF24L01_WRITE::command_t command(commandPayload);
    COMMANDS::NRF24L01_WRITE::response_t response;

    SPI_init();

    NRF24L01_tx(&command.data[0], command.length);
    response.length = NRF24L01_rx(&response.data[0]);

    if (rx_mode_gateway) {
        uint8_t status = NRF24L01_read_register(NRF24L01_REGISTER_STATUS);

        if (status & 0x20) {
            response.status = COMMANDS::NRF24L01_WRITE::STATUS_OK;
        }
        else {
            response.status = COMMANDS::NRF24L01_WRITE::STATUS_NACK;
        }
    }
    else {
        response.status = COMMANDS::NRF24L01_WRITE::STATUS_OK;
    }

    response.serialize(responsePayload);
}

void configureIna219(uint8_t address)
{
    // resisters
    // CONFIGURATION = 0x00,
    // SHUNT_VOLTAGE = 0x01,
    // BUS_VOLTAGE = 0x02,
    // POWER = 0x03,
    // CURRENT = 0x04,
    // CALIBRATION = 0x05,

#define range_40_mV 0x00
#define range_80_mV 0x01
#define range_160_mv 0x10
#define range_320_mV 0x11 // 3.2A range with 0.1 ohm resistor

#define samples_1_9bit 0x0011  // 84 uS conversion time
#define samples_1_12bit 0x0011 // 532 uS conversion time
#define samples_1 0x1000
#define samples_2 0x1001
#define samples_4 0x1010
#define samples_8 0x1011
#define samples_16 0x1100
#define samples_32 0x1101
#define samples_64 0x1110
#define samples_128 0x1111

#define mode_power_down 0x000
#define mode_shunt_voltage_triggered 0x001
#define mode_bus_voltage_triggered 0x010
#define mode_shunt_and_bus_triggered 0x011
#define mode_adc_off 0x100
#define mode_shunt_voltage_continuous 0x101
#define mode_bus_voltage_continuous 0x110
#define mode_shunt_and_bus_continuous 0x111

    // set configuration register
    uint16_t configurationValue = range_40_mV << 11;
    configurationValue |= samples_2 << 3;
    configurationValue |= mode_shunt_and_bus_triggered << 0;
    I2C_Init();
    I2C_Start(address);
    I2C_Write(0x00); // register address
    I2C_Write(configurationValue >> 8);
    I2C_Write(configurationValue);
    I2C_Stop();

    // set calibration value
    uint16_t calibrationValue = 2048;
    I2C_Init();
    I2C_Start(address);
    I2C_Write(0x05); // register address
    I2C_Write(calibrationValue >> 8);
    I2C_Write(calibrationValue);
    I2C_Stop();
}

void commandIna219(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::INA219::command_t command(commandPayload);
    COMMANDS::INA219::response_t response;
    response.status = 0;

    static bool ina219NotConfigured = true;
    uint8_t address = 0x80;

    if (ina219NotConfigured) {
        configureIna219(address);
        ina219NotConfigured = false;
    }

    // set register pointer to voltage register
    I2C_Init();
    I2C_Start(address);
    I2C_Write(2); // voltage
    I2C_Stop();

    I2C_Init();
    I2C_Start(address + 1);
    response.voltage[0] = I2C_Read_Ack();
    response.voltage[1] = I2C_Read_Nack();
    I2C_Stop();

    // set register pointer to current register
    I2C_Init();
    I2C_Start(address);
    I2C_Write(4); // current
    I2C_Stop();

    I2C_Init();
    I2C_Start(address + 1);
    response.current[0] = I2C_Read_Ack();
    response.current[1] = I2C_Read_Nack();
    I2C_Stop();

    response.serialize(responsePayload);
}

void commandWakeup(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::WAKEUP::command_t command(commandPayload);
    COMMANDS::WAKEUP::response_t response;
    response.status = 0;

    // only gateway should execute this command
#ifndef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
    uint8_t read_discover_package[32] = {0};

    NRF24L01_set_rx_as_master(false);

    //NRF24L01_write_ack_payload(&rf_link_wakeup_command[0], 32); // put command in ack buffer
                                                        
    for(uint16_t i = 0; i < 1000; i++)
    {
        uint8_t length = NRF24L01_read_rx_payload(&read_discover_package[0]);

        if(length == 32)
        {
            NRF24L01_set_rx_as_master(true);
            NRF24L01_write_tx_payload(&rf_link_wakeup_command[0], 32);
            response.status = 1;
                                                                
            for(uint8_t j=0; j<32; j++)
            {
                if(read_discover_package[j] != rf_link_discover_package[j])
                {
                    response.status = 0;
                }
                i=10000;
                break;
            }
        }

        _delay_ms(10);
    }
    _delay_ms(10); // give rf node some time to be ready for new commands
#endif

    response.serialize(responsePayload);
}

void commandSetNodeAddress(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SET_NODE_ADDRESS::command_t command(commandPayload);
    COMMANDS::SET_NODE_ADDRESS::response_t response;
    response.status = 0;

    rx_tx_addr[NRF24L01_ADDR_SIZE - 1] = command.node_address;

    // update wakeup command and discover package
    rf_link_wakeup_command[31] = command.node_address;
    rf_link_discover_package[31] = command.node_address;

    NRF24L01_init(&rx_tx_addr[0], &rx_tx_addr[0], rf_channel, rx_mode_gateway);

    response.status = 1;

    response.serialize(responsePayload);
}

void commandKeepAlive(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::KEEP_ALIVE::command_t command(commandPayload);
    COMMANDS::KEEP_ALIVE::response_t response;
    response.status = 0;

    keep_alive_interval_ms = 100 + command.time*100; // only used by radio node

    response.status = 1;

    response.serialize(responsePayload);
}

void parseCommand(
    protocol& m_protocol, comBusInterface* comBus, uint8_t* commandPayload)
{
    uint8_t responsePayload[COMMANDS::MAX_PAYLOAD_LENGTH] = {};

    uint8_t cmd_id = commandPayload[0];
    responsePayload[0] = static_cast<uint8_t>(COMMANDS::OI::UNDEFINED);

    switch (static_cast<COMMANDS::OI>(cmd_id)) {
    case COMMANDS::OI::BLINK:
        commandBlink(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::RANDOM:
        commandRandom(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SHA1:
        commandSha1(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SSD1306:
        commandSsd1306(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::HOTP:
        commandHotp(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::EEPROM_WRITE:
        commandEepromWrite(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::EEPROM_READ:
        commandEepromRead(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::WS2812B:
        commandWs2812b(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::AES:
        commandAes(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::PWM:
        commandPwm(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SLEEP:
        commandSleep(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GPIO:
        commandGpio(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::DEBUG:
        commandDebug(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::VCC:
        commandVcc(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::RADIO_UART:
        commandRadioUart(commandPayload, responsePayload, comBus);
        break;
    case COMMANDS::OI::TIMER:
        commandTimer(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::I2C_READ:
        commandI2cRead(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SPI_READ:
        commandSpiRead(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::INA219:
        commandIna219(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::DS18B20:
        commandDs18b20(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::I2C_WRITE:
        commandI2cWrite(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SPI_WRITE:
        commandSpiWrite(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::NRF24L01_INIT:
        commandNrf24l01Init(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::NRF24L01_READ:
        commandNrf24l01Read(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::NRF24L01_WRITE:
        commandNrf24l01Write(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SET_KEY:
        commandSetKey(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SET_DEVICE_INFO:
        commandSetDeviceInfo(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GET_DEVICE_INFO:
        commandGetDeviceInfo(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::WAKEUP:
        commandWakeup(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SET_NODE_ADDRESS:
        commandSetNodeAddress(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::KEEP_ALIVE:
        commandKeepAlive(commandPayload, responsePayload);
        break;
    default:
        break;
    }

    if (responsePayload[0] != static_cast<uint8_t>(COMMANDS::OI::UNDEFINED)) {
        sendMessage(m_protocol, comBus, responsePayload);
    }
}

void rxNodeSleepAndPollForWakeup()
{
    // periodically poll rx gateway for wakeup command

    uint8_t read_wakeup_command[32] = { 0 };

    uint8_t wakeup_received = 0;

    while (wakeup_received == 0) {
        powerDownRadioAndSleep(5000);
        NRF24L01_set_rx_as_master(true); // set gateway and flush pipes

        // send command to rf gateway
        NRF24L01_tx(&rf_link_discover_package[0], 32);

        // ack not working, set in receiver mode as a temprary hack
        NRF24L01_set_rx_as_master(false); // wait for command from gateway
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

    NRF24L01_set_rx_as_master(false);
}

void parseInput(protocol m_protocol, comBusInterface* comBus)
{
    uint8_t c = ' ';
    uint8_t payload[COMMANDS::MAX_PAYLOAD_LENGTH] = {};
    uint8_t packet[COMMANDS::MAX_PACKAGE_LENGTH];
    uint8_t length = 0;
    uint8_t cnt = 0;
#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
    uint32_t idle_loop_cnt_ms = 0;
#endif

    while (1) {
        cnt++;
        if (comBus->has_data()) {
            c = comBus->getChar();

            protocolVersionLastReceivedMessage
                = static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::UNDEFINED);

            if (c == PROTOCOL::HEADER::SYNC_PATTERN_BYTE_0) {
                length = m_protocol.searchForMessage(
                    (uint8_t*)payload, &protocolVersionLastReceivedMessage);

                if (length > 0) { // found payload
                    m_random.addEntropy(cnt);

                    if (protocolVersionLastReceivedMessage
                            == static_cast<uint8_t>(PROTOCOL::HEADER::VERSION::
                                                        RADIO_BINARY_AND_TEXT)
                        && rx_mode_gateway) {
                        uint8_t data_size = 0;
                        m_protocol.createPacket(
                            length,
                            payload,
                            &packet[0],
                            protocolVersionLastReceivedMessage);

                        data_size = PROTOCOL::HEADER::LENGTH
                            + PROTOCOL::CHECKSUM::LENGTH + length;

                        // send command to rx_node and wait for response
                        NRF24L01_tx(&packet[0], data_size);
                    }
                    else if (
                        protocolVersionLastReceivedMessage
                            == static_cast<uint8_t>(
                                PROTOCOL::HEADER::VERSION::
                                    RADIO_ENCRYPTED_BINARY_AND_TEXT)
                        && rx_mode_gateway) {
                        uint8_t data_size = 0;
                        m_protocol.createEncryptedPacket(
                            length,
                            payload,
                            &packet[0],
                            protocolVersionLastReceivedMessage);

                        data_size = PROTOCOL::HEADER::LENGTH
                            + PROTOCOL::CHECKSUM::LENGTH + length
                            + PROTOCOL::ENCRYPTED::CRYPTO_OVERHEAD;

                        // send command to rx_node and wait for response
                        NRF24L01_tx(&packet[0], data_size);
                    }
                    else {
                        parseCommand(m_protocol, comBus, payload);
#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
                        idle_loop_cnt_ms = 0;
#endif
                    }
                }
                c = 0;
            }
        }
        else {
            // idle process
#ifdef USE_NRF24L01_INTTERRUPT
            radioUart uartRadio;
            if (uartRadio.has_data()) {
                comBus->putChar(uartRadio.getChar());
            }

#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
            set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            sleep_enable();
            sleep_bod_disable();
            sei();
            sleep_cpu();
            sleep_disable();
            cli();
#endif
#endif

            
#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
            _delay_ms(1);
            idle_loop_cnt_ms++;
            if (idle_loop_cnt_ms > keep_alive_interval_ms) {
                // node has been idle for too long so go to sleep and wait for wakeup command
                rxNodeSleepAndPollForWakeup();
                idle_loop_cnt_ms = 0;
            }
#endif

#ifndef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
            // rx gateway reads response in idle loop
            uint8_t ack_packet[COMMANDS::MAX_PACKAGE_LENGTH];
            uint8_t response_length
                = NRF24L01_rx(&ack_packet[0]);

            // ignore messages from rx node if it is a wakeup ack packet
            uint8_t is_wakeup_ack = 0;
            if (response_length == 32) {
                is_wakeup_ack = 1;
                for (uint8_t j = 0; j < 32; j++) {
                    if (ack_packet[j] != rf_link_discover_package[j]) {
                        is_wakeup_ack = 0;
                    }
                }
            }

            if(is_wakeup_ack == 0)
            {
                comBus->writeBuffer(&ack_packet[0], response_length);
            }
#endif
        }
    }
}

int main()
{
#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
    radioUart uartRadio;
    NRF24L01_init(&rx_tx_addr[0], &rx_tx_addr[0], rf_channel, rx_mode_gateway);
    comBusInterface* u = &uartRadio;
#else
    uart uartSerialPort;
    NRF24L01_init(&rx_tx_addr[0], &rx_tx_addr[0], rf_channel, rx_mode_gateway);
    comBusInterface* u = &uartSerialPort;
#endif
    ArduinoCryptoHandler c(m_aes);
    protocol p(u, &c);

#ifdef USE_NRF24L01_INTTERRUPT
    PCICR |= _BV(PCIE0);
    PCMSK0 |= _BV(PCINT0);
#endif

    parseInput(p, u);

    return 0;
}

