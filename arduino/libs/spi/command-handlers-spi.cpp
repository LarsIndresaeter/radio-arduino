#include <cmd/payloads.hxx>
#include <command-handlers-spi.hpp>
#include <spi.hpp>

namespace LIBSPI {

void commandSpiRead(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SPI_READ::command_t command(commandPayload);
    COMMANDS::SPI_READ::response_t response;

    response.setReg(command.getReg());
    response.setLength(command.getLength());

    SPI_init();

    SPI_ChipSelectLow();

    SPI_masterTransmitByte(command.reg);

    for (uint8_t i = 0; i < command.length && i < sizeof(response.data); i++) {
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

    for (uint8_t i = 0; i < command.length && i < sizeof(command.data); i++) {
        SPI_masterTransmitByte(command.data[i]);
    }

    SPI_ChipSelectHigh();

    response.serialize(responsePayload);
}

} // namespace LIBSPI
