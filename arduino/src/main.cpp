#include <arduinoCryptoHandler.hpp>
#include <eeprom.hpp>
#include <parser.hpp>
#include <radioUart.hpp>
#include <uart.hpp>

#include <command-handlers.hpp>
#include <command-handlers-eeprom.hpp>
#include <command-handlers-pwm.hpp>
#include <command-handlers-sleep.hpp>
#include <command-handlers-ds18b20.hpp>
#include <command-handlers-ina219.hpp>
#include <command-handlers-gpio.hpp>
#include <command-handlers-nrf24l01.hpp>
#include <command-handlers-i2c.hpp>
#include <command-handlers-aes.hpp>
#include <command-handlers-sha1.hpp>
#include <command-handlers-ssd1306.hpp>
#include <command-handlers-ws2812b.hpp>
#include <command-handlers-spi.hpp>
#include <command-handlers-adc.hpp>
#include <command-handlers-timer.hpp>
#include <command-handlers-watchdog.hpp>
#include <command-handlers-quadencoder.hpp>
#include <command-handlers-random.hpp>
#include <command-handlers-radio-link.hpp>
#include <command-handlers-parser.hpp>
#include <command-handlers-radio-uart.hpp>

bool rx_mode_gateway = true; // default role, update eeprom to switch to node

void commandSwitch(uint8_t* commandPayload, uint8_t* responsePayload, ComBusInterface* comBus)
{
    uint8_t cmd_id = commandPayload[0];

    switch (static_cast<COMMANDS::OI>(cmd_id)) {
    case COMMANDS::OI::BLINK:
        COMMAND_HANDLERS::commandBlink(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::RANDOM:
        COMMAND_HANDLERS::commandRandom(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SHA1:
        COMMAND_HANDLERS::commandSha1(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SSD1306:
        COMMAND_HANDLERS::commandSsd1306(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::HOTP:
        COMMAND_HANDLERS::commandHotp(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::EEPROM_WRITE:
        COMMAND_HANDLERS::commandEepromWrite(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::EEPROM_READ:
        COMMAND_HANDLERS::commandEepromRead(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::WS2812B:
        COMMAND_HANDLERS::commandWs2812b(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::AES:
        COMMAND_HANDLERS::commandAes(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::PWM:
        COMMAND_HANDLERS::commandPwm(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SLEEP:
        COMMAND_HANDLERS::commandSleep(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GPIO:
        COMMAND_HANDLERS::commandGpio(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::DEBUG:
        COMMAND_HANDLERS::commandDebug(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::PING:
        COMMAND_HANDLERS::commandPing(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::VCC:
        COMMAND_HANDLERS::commandVcc(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::RADIO_UART:
        COMMAND_HANDLERS::commandRadioUart(commandPayload, responsePayload, comBus);
        break;
    case COMMANDS::OI::TIMER:
        COMMAND_HANDLERS::commandTimer(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::I2C_READ:
        COMMAND_HANDLERS::commandI2cRead(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SPI_READ:
        COMMAND_HANDLERS::commandSpiRead(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::INA219:
        COMMAND_HANDLERS::commandIna219(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::DS18B20:
        COMMAND_HANDLERS::commandDs18b20(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::I2C_WRITE:
        COMMAND_HANDLERS::commandI2cWrite(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SPI_WRITE:
        COMMAND_HANDLERS::commandSpiWrite(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::NRF24L01_INIT:
        COMMAND_HANDLERS::commandNrf24l01Init(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::NRF24L01_READ:
        COMMAND_HANDLERS::commandNrf24l01Read(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::NRF24L01_WRITE:
        COMMAND_HANDLERS::commandNrf24l01Write(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SET_KEY:
        COMMAND_HANDLERS::commandSetKey(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SET_DEVICE_NAME:
        COMMAND_HANDLERS::commandSetDeviceName(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GET_DEVICE_NAME:
        COMMAND_HANDLERS::commandGetDeviceName(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GET_VERSION:
        COMMAND_HANDLERS::commandGetVersion(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GET_STATISTICS:
        COMMAND_HANDLERS::commandGetStatistics(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::WAKEUP:
        COMMAND_HANDLERS::commandWakeup(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::QUADRATURE_ENCODER:
        COMMAND_HANDLERS::commandQuadratureEncoder(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SET_NODE_ADDRESS:
        COMMAND_HANDLERS::commandSetNodeAddress(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::KEEP_ALIVE:
        COMMAND_HANDLERS::commandKeepAlive(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::REQUIRE_TRANSPORT_ENCRYPTION:
        COMMAND_HANDLERS::commandRequireTransportEncryption(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::UNENCRYPTED_SESSION:
        COMMAND_HANDLERS::commandUnencryptedSession(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SET_RADIO_ROLE:
        COMMAND_HANDLERS::commandSetRadioRole(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SOFT_RESET:
        COMMAND_HANDLERS::commandSoftReset(commandPayload, responsePayload);
        break;
    default:
        break;
    }
}

int main()
{
    RADIOLINK::setNodeAddress(0);
    uint8_t transport_key[16] = { 0 };
    EEPROM_DATA_STORE::readFromActive(offsetof(eeprom_data_t, TK_KEY), &transport_key[0], 16);
    ArduinoCryptoHandler cryptoHandler(&transport_key[0]);

    if (false) { // set to true when you need to force update isRadioNode flag in eeprom 
        EEPROM_DATA_STORE::setIsRadioNode('g');
    };

    if ('n' == EEPROM_DATA_STORE::getIsRadioNode()) {
        rx_mode_gateway = false; // override default role
    }

    if ('g' == EEPROM_DATA_STORE::getIsRadioNode()) {
        rx_mode_gateway = true; // override default role
    }

    EEPROM_DATA_STORE::incrementRestarts();

    if (rx_mode_gateway) {

        Uart uart;

        Protocol protocol((ComBusInterface*)&uart, &cryptoHandler);
        PARSER::setRequireTransportEncryption(EEPROM_DATA_STORE::getRequireTransportEncryption());
        PARSER::parseInput(protocol, (ComBusInterface*)&uart);
    }
    else {

        RadioUart uart;

        Protocol protocol((ComBusInterface*)&uart, &cryptoHandler);
        PARSER::setRequireTransportEncryption(EEPROM_DATA_STORE::getRequireTransportEncryption());
        PARSER::parseInput(protocol, (ComBusInterface*)&uart);
    }

    return 0;
}

