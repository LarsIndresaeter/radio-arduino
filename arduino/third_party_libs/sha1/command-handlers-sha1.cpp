#include <cmd/payloads.hxx>
#include <command-handlers-sha1.hpp>
#include <sha1.hpp>

namespace COMMAND_HANDLERS{

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

} // namespace COMMAND_HANDLERS

