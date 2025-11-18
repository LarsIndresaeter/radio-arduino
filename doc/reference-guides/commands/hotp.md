# command Hotp

- [command.hxx](../../../interface/libs/commands/include/cmd/hotp/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/hotp/payload.hxx)

## class RaduinoCommandHotp

### constructors

- RaduinoCommandHotp()

### member methods in response struct

- std::string getData()
- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::HOTP::response_t responseStruct()

## struct command_t

- empty command payload

## struct response_t

### methods in response struct

### variables in response struct

- uint8_t[20] data

