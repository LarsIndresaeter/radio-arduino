# command RequireTransportEncryption

- [command.hxx](../../../interface/libs/commands/include/cmd/require_transport_encryption/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/require_transport_encryption/payload.hxx)

## class RaduinoCommandRequireTransportEncryption

### constructors

- RaduinoCommandRequireTransportEncryption(uint8_t value)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::REQUIRE_TRANSPORT_ENCRYPTION::response_t responseStruct()

## struct command_t

### methods in command struct

- void setValue(uint8_t value)
- uint8_t getValue()

### variables in command struct

- uint8_t value

## struct response_t

- empty response payload

