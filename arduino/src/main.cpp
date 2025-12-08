#include <arduinoCryptoHandler.hpp>
#include <eeprom.hpp>
#include <parser.hpp>
#include <radioUart.hpp>
#include <uart.hpp>

#include <command-handlers-adc.hpp>
#include <command-handlers-aes.hpp>
#include <command-handlers-ds18b20.hpp>
#include <command-handlers-eeprom.hpp>
#include <command-handlers-gpio.hpp>
#include <command-handlers-i2c.hpp>
#include <command-handlers-ina219.hpp>
#include <command-handlers-nrf24l01.hpp>
#include <command-handlers-parser.hpp>
#include <command-handlers-pwm.hpp>
#include <command-handlers-quadencoder.hpp>
#include <command-handlers-radio-link.hpp>
#include <command-handlers-radio-uart.hpp>
#include <command-handlers-random.hpp>
#include <command-handlers-sha1.hpp>
#include <command-handlers-sleep.hpp>
#include <command-handlers-spi.hpp>
#include <command-handlers-ssd1306.hpp>
#include <command-handlers-timer.hpp>
#include <command-handlers-watchdog.hpp>
#include <command-handlers-ws2812b.hpp>
#include <command-handlers-lsm303d.hpp>
#include <command-handlers.hpp>

bool rx_mode_gateway = true; // default role, update eeprom to switch to node

void commandSwitch(uint8_t* commandPayload, uint8_t* responsePayload, ComBusInterface* comBus)
{
    uint8_t cmd_id = commandPayload[0];

    switch (static_cast<COMMANDS::OI>(cmd_id)) {
    case COMMANDS::OI::BLINK:
        GPIO::commandBlink(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::RANDOM:
        RANDOM::commandRandom(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SHA1:
        SHA1::commandSha1(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SSD1306:
        LIBSSD1306::commandSsd1306(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::HOTP:
        SHA1::commandHotp(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::EEPROM_WRITE:
        EEPROM::commandEepromWrite(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::EEPROM_READ:
        EEPROM::commandEepromRead(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::WS2812B:
        LIBWS2812B::commandWs2812b(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GET_LSM303D:
        LSM303D::commandGetLsm303d(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::AES:
        AES::commandAes(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::PWM:
        PWM::commandPwm(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SLEEP:
        SLEEP::commandSleep(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GPIO:
        GPIO::commandGpio(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::DEBUG:
        COMMAND_HANDLERS::commandDebug(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::PING:
        COMMAND_HANDLERS::commandPing(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::VCC:
        AtmelAdc::commandVcc(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::RADIO_UART:
        RADIOUART::commandRadioUart(commandPayload, responsePayload, comBus);
        break;
    case COMMANDS::OI::TIMER:
        TIMER::commandTimer(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::I2C_READ:
        LIBI2C::commandI2cRead(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SPI_READ:
        LIBSPI::commandSpiRead(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::INA219:
        INA219::commandIna219(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::DS18B20:
        DS18B20::commandDs18b20(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::I2C_WRITE:
        LIBI2C::commandI2cWrite(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SPI_WRITE:
        LIBSPI::commandSpiWrite(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::NRF24L01_INIT:
        NRF24L01::commandNrf24l01Init(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::NRF24L01_READ:
        NRF24L01::commandNrf24l01Read(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::NRF24L01_WRITE:
        NRF24L01::commandNrf24l01Write(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SET_KEY:
        PARSER::commandSetKey(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SET_DEVICE_NAME:
        EEPROM::commandSetDeviceName(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GET_DEVICE_NAME:
        EEPROM::commandGetDeviceName(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GET_VERSION:
        COMMAND_HANDLERS::commandGetVersion(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GET_STATISTICS:
        COMMAND_HANDLERS::commandGetStatistics(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::WAKEUP:
        RADIOLINK::commandWakeup(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::QUADRATURE_ENCODER:
        QUADENCODER::commandQuadratureEncoder(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SET_NODE_ADDRESS:
        RADIOLINK::commandSetNodeAddress(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::KEEP_ALIVE:
        PARSER::commandKeepAlive(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::REQUIRE_TRANSPORT_ENCRYPTION:
        PARSER::commandRequireTransportEncryption(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::UNLOCK_SESSION:
        PARSER::commandUnlockSession(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SET_RADIO_ROLE:
        EEPROM::commandSetRadioRole(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SOFT_RESET:
        WATCHDOG::commandSoftReset(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SET_UNIQUE_ID:
        EEPROM::commandSetUniqueId(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GET_UNIQUE_ID:
        EEPROM::commandGetUniqueId(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::GET_ATTACHED_DEVICES_CSV_STRING:
        EEPROM::commandGetAttachedDevicesCsvString(commandPayload, responsePayload);
        break;
    case COMMANDS::OI::SET_ATTACHED_DEVICES_CSV_STRING:
        EEPROM::commandSetAttachedDevicesCsvString(commandPayload, responsePayload);
        break;
    default:
        break;
    }
}

int main()
{
    RADIOLINK::setNodeAddress(0);
    uint8_t transport_key[16] = { 0 };
    EEPROM_DATA_STORE::readFromActive(offsetof(eeprom_data_t, transportEncryptionKey), &transport_key[0], 16);
    ArduinoCryptoHandler cryptoHandler(&transport_key[0]);

    rx_mode_gateway = EEPROM_DATA_STORE::readRxModeGatewayFromEeprom();

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
