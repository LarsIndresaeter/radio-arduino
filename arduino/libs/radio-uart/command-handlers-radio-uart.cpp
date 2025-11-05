#include <cmd/payloads.hxx>
#include <comBusInterface.hpp>
#include <command-handlers-radio-uart.hpp>
#include <nrf24l01.hpp>
#include <radioUart.hpp>

namespace RADIOUART {

void commandRadioUart(uint8_t* commandPayload, uint8_t* responsePayload, ComBusInterface* comBus)
{
    COMMANDS::RADIO_UART::command_t command(commandPayload);
    COMMANDS::RADIO_UART::response_t response;

    if (command.mode == 'p') // promiscous mode
    {
        NRF24L01_write_register(NRF24L01_REGISTER_CONFIG, 0x03);
        // illegal address, undocumented 2 byte address
        NRF24L01_write_register(NRF24L01_REGISTER_SETUP_AW, 0x00);

        uint8_t addr[5] = { 0, 0, 0, 0, 0x55 };

        NRF24L01_init(&addr[0], &addr[0], false);
    }
    else if (command.mode == 's') // send data read from uart over radio
    {
        RadioUart uartRadio;

        uint8_t package[32] = { 0 };
        uint8_t len = 0;
        uint8_t c;

        while (true) {
            while (comBus->hasData()) {
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
            if (uartRadio.hasData()) {
                comBus->putChar(uartRadio.getChar());
            }
        }
    }

    response.serialize(responsePayload);
}

} // namespace RADIOUART
