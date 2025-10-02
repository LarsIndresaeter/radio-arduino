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
#include <cmd/payloads.hpp>
#include <eeprom.hpp>
#include <i2c.hpp>
#include <ina219.hpp>
#include <nrf24l01.hpp>
#include <parser.hpp>
#include <pwm.hpp>
#include <random.hpp>
#include <sha1.hpp>
#include <sleep.hpp>
#include <spi.hpp>
#include <quadencoder.hpp>
#include <gpio.hpp>

#include <Framebuffer.hpp>
#include <avr/sleep.h>
#include <ds18b20.h>
#include <onewire.h>
#include <romsearch.h>
#include <stdio.h>
#include <version.h>
#include <ws2812b.hpp>

Aes aes;

void commandDs18b20(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::DS18B20::command_t command(commandPayload);
    COMMANDS::DS18B20::response_t response;

    int16_t temp;
    ds18b20convert(&PORTB, &DDRB, &PINB, (1 << 0), NULL);

    _delay_ms(1000);

    ds18b20read(&PORTB, &DDRB, &PINB, (1 << 0), NULL, &temp);

    response.setTemperature(temp);

    response.serialize(responsePayload);
}

void commandBlink(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::BLINK::command_t command(commandPayload);
    COMMANDS::BLINK::response_t response;

    GPIO::blink();

    response.serialize(responsePayload);
}

void commandRandom(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::RANDOM::response_t response;

    random.addEntropyAndMix();

    for (uint8_t i = 0; i < sizeof(response.data); i++) {
        response.data[i] = random.getRandomByte();
    }

    response.serialize(responsePayload);
}

void commandSha1(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SHA1::command_t command(commandPayload);
    COMMANDS::SHA1::response_t response;

    SHA1Context sha;

    SHA1Reset(&sha);
    SHA1Input(&sha, command.data, sizeof(command.data));
    SHA1Result(&sha, response.data);

    response.serialize(responsePayload);
}

void commandHotp(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::HOTP::command_t command(commandPayload);
    COMMANDS::HOTP::response_t response;

    static uint16_t cnt = 0;
    uint8_t HOTP_message[20] = { ' ', ' ', 's', 'e', 'c', 'r', 'e', 't' };

    HOTP_message[0] = cnt >> 8;
    HOTP_message[1] = cnt++;

    SHA1Context sha;

    SHA1Reset(&sha);
    SHA1Input(&sha, &HOTP_message[0], 8);
    SHA1Result(&sha, response.data);

    response.serialize(responsePayload);
}

void commandAes(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::AES::command_t command(commandPayload);
    COMMANDS::AES::response_t response;

    Eeprom eeprom;

    uint8_t aes_key[16] = {};
    for (uint8_t i = 0; i < 16; i++) {
        aes_key[i] = eeprom.read(offsetof(eeprom_data_t, EK_KEY) + i);
    }

    uint8_t aes_iv[16] = { 0 };

    // copy data to response buffer
    for (uint8_t i = 0; i < sizeof(response.data); i++) {
        response.data[i] = command.data[i];
    }

    if (command.type == 'c') {
        aes.Crypt(response.data, &aes_key[0], &aes_iv[0]);
    }

    if (command.type == 'd') {
        aes.Decrypt(response.data, &aes_key[0], &aes_iv[0]);
    }

    response.type = command.type;

    response.serialize(responsePayload);
}

void commandSleep(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SLEEP::command_t command(commandPayload);
    COMMANDS::SLEEP::response_t response;

    powerDownRadioAndSleep(command.getDelay());

    response.serialize(responsePayload);
}

void commandPwm(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::PWM::command_t command(commandPayload);
    COMMANDS::PWM::response_t response;

    response.setPort(command.getPort());
    response.setPin(command.getPin());
    response.setValue(command.getValue());

    PWM::write(command.port, command.pin, command.value);

    response.serialize(responsePayload);
}

void commandGpio(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::GPIO::command_t command(commandPayload);
    COMMANDS::GPIO::response_t response;

    response.setPortb(GPIO::readPortB());
    response.setPortc(GPIO::readPortC());
    response.setPortd(GPIO::readPortD());

    response.serialize(responsePayload);
}

void commandSsd1306(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SSD1306::command_t command(commandPayload);
    COMMANDS::SSD1306::response_t response;

    Framebuffer fb;

    for (uint8_t x = 0; x < sizeof(command.data); x++) {
        fb.drawChar(x, command.line, command.data[x]);
    }

    fb.show();
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
        rising_time = TCNT1;
        falling_time = 0;
        pulse_width = 0;
        TCCR1B &= ~(1 << ICES1); // input capture on falling edge
    }
    else {
        TCCR1B |= (1 << ICES1);
        falling_time = TCNT1;
        pulse_width = falling_time - rising_time;
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

    TCCR1B |= (1 << ICES1); // input capture set for rising edge
    TCCR1B |= (1 << CS10); // no prescaler
    TIMSK1 |= (1 << ICIE1); // input capture interrupt enable
}

void timerStop()
{
    TIMSK1 &= ~(1 << ICIE1); // input capture interrupt disable
}

void commandTimer(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::TIMER::command_t command(commandPayload);
    COMMANDS::TIMER::response_t response;

    _delay_ms(10);
    timerStart();
    _delay_ms(25);
    timerStop();

    response.setPulsewidth(pulse_width >> 4); // divide by 16 to get micro seconds

    response.serialize(responsePayload);
}

void commandVcc(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::VCC::command_t command(commandPayload);
    COMMANDS::VCC::response_t response;

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

    response.setVcc(vcc);

    response.serialize(responsePayload);
}

void commandDebug(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::DEBUG::command_t command(commandPayload);
    COMMANDS::DEBUG::response_t response;

    for (uint8_t i = 0; i < sizeof(response.data); i++) {
        response.data[i] = i;
    }

    response.serialize(responsePayload);
}

void commandPing(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::PING::response_t response;
    response.serialize(responsePayload);
}

void commandEepromRead(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::EEPROM_READ::command_t command(commandPayload);
    COMMANDS::EEPROM_READ::response_t response;
    Eeprom eeprom;

    response.setAddress(command.getAddress());
    response.setData(eeprom.read(command.getAddress()));

    response.serialize(responsePayload);
}

void commandWs2812b(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::WS2812B::command_t command(commandPayload);
    COMMANDS::WS2812B::response_t response;

    Ws2812b ws2812b;

    rgb_color colors[sizeof(command.red)];

    for (uint16_t i = 0; i < sizeof(command.red); i++) {
        colors[i].red = command.red[i];
        colors[i].green = command.green[i];
        colors[i].blue = command.blue[i];
    }

    ws2812b.led_strip_write(colors, sizeof(command.red));

    response.serialize(responsePayload);
}

void commandEepromWrite(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::EEPROM_WRITE::command_t command(commandPayload);
    COMMANDS::EEPROM_WRITE::response_t response;
    Eeprom eeprom;

    eeprom.write(command.getAddress(), command.data);

    response.setAddress(command.getAddress());
    response.setData(eeprom.read(command.getAddress()));

    response.serialize(responsePayload);
}

void commandI2cWrite(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::I2C_WRITE::command_t command(commandPayload);
    COMMANDS::I2C_WRITE::response_t response;

    I2C_Init();
    I2C_Start(command.device); // write address
    I2C_Write(command.registerAddress[0]); // first word address
    I2C_Write(command.registerAddress[1]); // second word address
    for (uint8_t i = 0;
         (i < command.length) && (i < sizeof(command.data));
         i++) {
        if (0 != I2C_Write(command.data[i])) {
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
    Eeprom eeprom;

    uint16_t address = 0;

    if (command.keyId == 'T') {
        address = offsetof(eeprom_data, TK_KEY);
    }
    else if (command.keyId == 'H') {
        address = offsetof(eeprom_data_t, HMAC_KEY);
    }
    else if (command.keyId == 'O') {
        address = offsetof(eeprom_data_t, HOTP_KEY);
    }
    else if (command.keyId == 'E') {
        address = offsetof(eeprom_data_t, EK_KEY);
    }

    if (command.keyId != 'U') {
        for (uint8_t i = 0; i < sizeof(command.keyValue); i++) {
            eeprom.write(address + i, command.keyValue[i]);
        }
    }

    response.serialize(responsePayload);
}

void commandSetDeviceInfo(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SET_DEVICE_NAME::command_t command(commandPayload);
    COMMANDS::SET_DEVICE_NAME::response_t response;
    Eeprom eeprom;

    for (uint8_t i = 0; i < sizeof(command.name); i++) {
        eeprom.write(offsetof(eeprom_data_t, NAME) + i, command.name[i]);
    }

    response.serialize(responsePayload);
}

void commandGetDeviceName(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::GET_DEVICE_NAME::command_t command(commandPayload);
    COMMANDS::GET_DEVICE_NAME::response_t response;
    Eeprom eeprom;

    for (uint8_t i = 0; i < sizeof(response.nameString); i++) {
        response.nameString[i] = eeprom.read(offsetof(eeprom_data_t, NAME) + i);
    }

    response.serialize(responsePayload);
}

void commandGetVersion(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::GET_VERSION::command_t command(commandPayload);
    COMMANDS::GET_VERSION::response_t response;

    for (uint8_t i = 0; i < sizeof(response.versionString); i++) {
        response.versionString[i] = 0;
    }

    for (uint8_t i = 0; i < sizeof(response.versionString) && ARDUINO_VERSION[i] != 0; i++) {
        response.versionString[i] = ARDUINO_VERSION[i];
    }
}

void commandGetStatistics(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::GET_STATISTICS::command_t command(commandPayload);
    COMMANDS::GET_STATISTICS::response_t response;

    response.setUart_rx(uart_rx);
    response.setUart_tx(uart_tx);
    response.setRf_rx(rf_rx);
    response.setRf_tx(rf_tx);
    response.setCommandsparsed(commandsParsed);

    response.serialize(responsePayload);
}

void commandI2cRead(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::I2C_READ::command_t command(commandPayload);
    COMMANDS::I2C_READ::response_t response;

    response.setDevice(command.getDevice());
    response.setRegisteraddress(command.getRegisteraddress());
    response.setLength(command.getLength());

    I2C_Init();
    I2C_Start(command.device); // read address
    I2C_Write(command.registerAddress[0]); // first word address
    I2C_Write(command.registerAddress[1]); // second word address

    I2C_Repeated_Start(command.device + 1);

    for (uint8_t i = 0;
         (i < command.length) && (i < sizeof(response.data));
         i++) {
        response.data[i] = I2C_Read_Ack();
    }

    I2C_Read_Nack(); /* Read flush data with nack */
    I2C_Stop();

    response.serialize(responsePayload);
}

void commandSpiRead(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SPI_READ::command_t command(commandPayload);
    COMMANDS::SPI_READ::response_t response;

    response.setReg(command.getReg());
    response.setLength(command.getLength());

    SPI_init();

    SPI_ChipSelectLow();

    SPI_masterTransmitByte(command.reg);

    for (uint8_t i = 0;
         i < command.length && i < sizeof(response.data);
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
         i < command.length && i < sizeof(command.data);
         i++) {
        SPI_masterTransmitByte(command.data[i]);
    }

    SPI_ChipSelectHigh();

    response.serialize(responsePayload);
}

void commandRadioUart(
    uint8_t* commandPayload, uint8_t* responsePayload, ComBusInterface* comBus)
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
        RadioUart uartRadio;

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
        RadioUart uartRadio;

        while (true) {
            if (uartRadio.has_data()) {
                comBus->putChar(uartRadio.getChar());
            }
        }
    }

    response.serialize(responsePayload);
}

void commandNrf24l01Init(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::NRF24L01_INIT::command_t command(commandPayload);
    COMMANDS::NRF24L01_INIT::response_t response;

    NRF24L01_init(
        &command.rxAddr[0],
        &command.txAddr[0],
        command.rfChannel,
        command.gateway == 1);

    response.serialize(responsePayload);
}

void commandNrf24l01Read(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::NRF24L01_READ::command_t command(commandPayload);
    COMMANDS::NRF24L01_READ::response_t response;

    SPI_init();

    response.setLength(NRF24L01_rx(response.data));

    response.serialize(responsePayload);
}

void commandNrf24l01Write(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::NRF24L01_WRITE::command_t command(commandPayload);
    COMMANDS::NRF24L01_WRITE::response_t response;

    SPI_init();

    NRF24L01_tx(&command.data[0], command.length);
    response.setLength(NRF24L01_rx(&response.data[0]));

    if (rx_mode_gateway) {
        NRF24L01_read_register(NRF24L01_REGISTER_STATUS);
    }

    response.serialize(responsePayload);
}

void commandIna219(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::INA219::command_t command(commandPayload);
    COMMANDS::INA219::response_t response;

    uint16_t voltage, current;

    readIna219(&voltage, &current);

    response.setVoltage(voltage);
    response.setCurrent(current);

    response.serialize(responsePayload);
}

void commandWakeup(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::WAKEUP::command_t command(commandPayload);
    COMMANDS::WAKEUP::response_t response;
    response.attention = 0;

    // only gateway should execute this command
#ifndef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
    uint8_t read_discover_package[32] = { 0 };

    for (uint16_t i = 0; i < 1000; i++) {
        uint8_t length = NRF24L01_read_rx_payload(&read_discover_package[0]);

        if (length == 32) {
            response.attention = read_discover_package[31];

            if ((0 != command.checkAttentionFlag) && (0 == read_discover_package[31])) {
                // received discover package but about wakeup since data available flag was not set
                break;
            }
            else {
                NRF24L01_tx(&rf_link_wakeup_command[0], 32);

                for (uint8_t j = 0; j < 31; j++) {
                    if (read_discover_package[j] != rf_link_discover_package[j]) {
                        i = 10000;
                        break;
                    }
                }
            }
        }

        _delay_ms(10);
    }
    _delay_ms(10); // give rf node some time to be ready for new commands
#endif

    response.serialize(responsePayload);
}

void commandQuadratureEncoder(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::QUADRATURE_ENCODER::command_t command(commandPayload);
    COMMANDS::QUADRATURE_ENCODER::response_t response;

    QUADENCODER::initialize();

    attention_flag = QUADENCODER::isChanged();

    response.setCountpositive(QUADENCODER::getCountPositivePulses());
    response.setCountnegative(QUADENCODER::getCountNegativePulses());
    response.setSwitchcount(QUADENCODER::getSwitchCount());
    response.setSwitchposition(QUADENCODER::getSwitchPosition());

    response.serialize(responsePayload);
}

void commandSetNodeAddress(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SET_NODE_ADDRESS::command_t command(commandPayload);
    COMMANDS::SET_NODE_ADDRESS::response_t response;

    rx_tx_addr[NRF24L01_ADDR_SIZE - 1] = command.nodeAddress;

    // update wakeup command and discover package
    rf_link_wakeup_command[31] = command.nodeAddress;
    rf_link_discover_package[30] = command.nodeAddress;

    NRF24L01_init(&rx_tx_addr[0], &rx_tx_addr[0], rf_channel, rx_mode_gateway);

    response.serialize(responsePayload);
}

void commandKeepAlive(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::KEEP_ALIVE::command_t command(commandPayload);
    COMMANDS::KEEP_ALIVE::response_t response;

    setKeepAliveInterval(command.time);

    response.serialize(responsePayload);
}

void commandSwitch(uint8_t* commandPayload, uint8_t* responsePayload, ComBusInterface* comBus)
{
    uint8_t cmd_id = commandPayload[0];

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
    case COMMANDS::OI::PING:
        commandPing(commandPayload, responsePayload);
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
    case COMMANDS::OI::SET_DEVICE_NAME:
        commandSetDeviceInfo(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GET_DEVICE_NAME:
        commandGetDeviceName(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GET_VERSION:
        commandGetVersion(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GET_STATISTICS:
        commandGetStatistics(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::WAKEUP:
        commandWakeup(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::QUADRATURE_ENCODER:
        commandQuadratureEncoder(commandPayload, responsePayload);
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
}

int main()
{
#ifdef REPLACE_UART_WITH_RADIO_COMMUNICATION_AKA_RX_NODE
    RadioUart uart;
#else
    Uart uart;
#endif

    NRF24L01_init(&rx_tx_addr[0], &rx_tx_addr[0], rf_channel, rx_mode_gateway);
    ArduinoCryptoHandler cryptoHandler(aes);
    Protocol protocol((ComBusInterface*)&uart, &cryptoHandler);

    parseInput(protocol, (ComBusInterface*)&uart);

    return 0;
}

