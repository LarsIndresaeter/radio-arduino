#include <cmd/payloads.hxx>
#include <command-handlers.hpp>
#include <eeprom.hpp>
#include <nrf24l01.hpp>
#include <parser.hpp>
#include <uart.hpp>
#include <version.h>

namespace COMMAND_HANDLERS {

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

    response.serialize(responsePayload);
}

void commandGetStatistics(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::GET_STATISTICS::command_t command(commandPayload);
    COMMANDS::GET_STATISTICS::response_t response;

    response.setUart_rx(UART::getUartRxBytes());
    response.setUart_tx(UART::getUartTxBytes());
    response.setRf_rx(NRF24L01_getRxBytes());
    response.setRf_tx(NRF24L01_getTxBytes());
    response.setCommandsparsed(PARSER::getCommandsParsedCounter());
    response.setRestarts(EEPROM_DATA_STORE::getRestarts());

    response.serialize(responsePayload);
}

} // namespace COMMAND_HANDLERS
