#include <cmd/payloads.hxx>
#include <command-handlers-parser.hpp>
#include <eeprom.hpp>
#include <parser.hpp>

namespace COMMAND_HANDLERS {

void commandSetKey(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::SET_KEY::command_t command(commandPayload);
    COMMANDS::SET_KEY::response_t response;

    if (PARSER::lastReceivedCommandWasEncrypted()) {
        if (command.keyId == 'E') {
            EEPROM_DATA_STORE::writeToSpareAndSetAsActive(offsetof(eeprom_data_t, dataEncryptionKey), &command.keyValue[0], 16);
        }
        else if (command.keyId == 'T') {
            EEPROM_DATA_STORE::writeToSpareAndSetAsActive(offsetof(eeprom_data_t, transportEncryptionKey), &command.keyValue[0], 16);
        }
    }

    response.serialize(responsePayload);
}

void commandKeepAlive(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::KEEP_ALIVE::command_t command(commandPayload);
    COMMANDS::KEEP_ALIVE::response_t response;

    PARSER::setKeepAliveInterval(command.time);

    response.serialize(responsePayload);
}

void commandRequireTransportEncryption(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::REQUIRE_TRANSPORT_ENCRYPTION::command_t command(commandPayload);
    COMMANDS::REQUIRE_TRANSPORT_ENCRYPTION::response_t response;

    if (PARSER::lastReceivedCommandWasEncrypted()) {
        PARSER::setRequireTransportEncryption(command.value);
        EEPROM_DATA_STORE::setRequireTransportEncryption(command.value);
    }

    response.serialize(responsePayload);
}

void commandUnlockSession(uint8_t* commandPayload, uint8_t* responsePayload)
{
    COMMANDS::UNLOCK_SESSION::command_t command(commandPayload);
    COMMANDS::UNLOCK_SESSION::response_t response;

    if (PARSER::lastReceivedCommandWasEncrypted()) {
        PARSER::setRequireTransportEncryption(0);
    }

    response.serialize(responsePayload);
}

} // namespace COMMAND_HANDLERS
