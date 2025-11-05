#include <cmd/payloads.hxx>
#include <command-handlers-nrf24l01.hpp>
#include <nrf24l01.hpp>
#include <spi.hpp>

extern bool rx_mode_gateway;

namespace NRF24L01 {

void commandNrf24l01Init(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::NRF24L01_INIT::command_t command(commandPayload);
    COMMANDS::NRF24L01_INIT::response_t response;

    NRF24L01_set_rf_channel(command.rfChannel);

    NRF24L01_init(&command.rxAddr[0], &command.txAddr[0], command.gateway == 1);

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

} // namespace NRF24L01
