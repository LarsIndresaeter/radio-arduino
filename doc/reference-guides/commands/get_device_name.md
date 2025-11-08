# command GetDeviceName

- [command.hxx](../../../interface/libs/commands/include/cmd/get_device_name/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/get_device_name/payload.hxx)

## class RaduinoCommandGetDeviceName

### constructors

- RaduinoCommandGetDeviceName()

### member methods in response struct

- std::string getNamestring()
- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::GET_DEVICE_NAME::response_t responseStruct()

## struct command_t

- empty command payload

## struct response_t

### methods in response struct


### variables in response struct

- uint8_t[16] nameString

