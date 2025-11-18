# command GetVersion

- [command.hxx](../../../interface/libs/commands/include/cmd/get_version/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/get_version/payload.hxx)

## class RaduinoCommandGetVersion

### constructors

- RaduinoCommandGetVersion()

### member methods in response struct

- std::string getVersionstring()
- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::GET_VERSION::response_t responseStruct()

## struct command_t

- empty command payload

## struct response_t

### methods in response struct

### variables in response struct

- uint8_t[32] versionString

