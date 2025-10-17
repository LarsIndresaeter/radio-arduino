#include <cmd/payloads.hxx>
#include <command-handlers-ws2812b.hpp>
#include <ws2812b.hpp>

namespace COMMAND_HANDLERS{

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

} // namespace COMMAND_HANDLERS

