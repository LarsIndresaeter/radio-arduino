# command Debug

- [command.hxx](../../../interface/libs/commands/include/cmd/debug/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/debug/payload.hxx)

## class RaduinoCommandDebug

### constructors

- RaduinoCommandDebug()

### member methods in response struct

- std::string getData()
- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::DEBUG::response_t responseStruct()

## struct command_t

- empty command payload

## struct response_t

### methods in response struct

### variables in response struct

- uint8_t[32] data

