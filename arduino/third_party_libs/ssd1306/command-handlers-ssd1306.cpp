#include <cmd/payloads.hxx>
#include <command-handlers-ssd1306.hpp>
#include <SSD1306.hpp>
#include <i2c.hpp>
#include <Framebuffer.hpp>

namespace COMMAND_HANDLERS{

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

} // namespace COMMAND_HANDLERS

