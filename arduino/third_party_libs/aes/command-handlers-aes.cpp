#include <aes.hpp>
#include <cmd/payloads.hxx>
#include <command-handlers-aes.hpp>
#include <eeprom.hpp>

namespace COMMAND_HANDLERS {

void commandAes(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::AES::command_t command(commandPayload);
    COMMANDS::AES::response_t response;

    uint8_t aes_key[16] = {};
    EEPROM_DATA_STORE::readFromActive(offsetof(eeprom_data_t, EK_KEY), &aes_key[0], 16);

    uint8_t aes_iv[16] = { 0 };

    // copy data to response buffer
    for (uint8_t i = 0; i < sizeof(response.data); i++) {
        response.data[i] = command.data[i];
    }

    AES::Sanitize();

    if (command.type == 'c') {
        AES::Crypt(response.data, &aes_key[0], &aes_iv[0]);
    }

    if (command.type == 'd') {
        AES::Decrypt(response.data, &aes_key[0], &aes_iv[0]);
    }

    response.type = command.type;

    response.serialize(responsePayload);
}

} // namespace COMMAND_HANDLERS
