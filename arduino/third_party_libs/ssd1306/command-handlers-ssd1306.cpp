#include <Framebuffer.hpp>
#include <SSD1306.hpp>
#include <cmd/payloads.hxx>
#include <command-handlers-ssd1306.hpp>
#include <i2c.hpp>

namespace LIBSSD1306 {

void commandSsd1306(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SSD1306::command_t command(commandPayload);
    COMMANDS::SSD1306::response_t response;

    Framebuffer fb;

    for (uint8_t x = 0; x < sizeof(command.data); x++) {
        if (command.data[x] != 0) {
            fb.drawChar(x, command.line, command.data[x]);
        }
    }

    fb.show();
    fb.invert(0);

    response.serialize(responsePayload);
}

} // namespace LIBSSD1306
