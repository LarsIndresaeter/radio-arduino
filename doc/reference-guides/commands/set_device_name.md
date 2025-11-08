# command SetDeviceName

- [command.hxx](../../../interface/libs/commands/include/cmd/set_device_name/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/set_device_name/payload.hxx)

## class RaduinoCommandSetDeviceName

### constructors

- RaduinoCommandSetDeviceName(std::vector<uint8_t> name)
- RaduinoCommandSetDeviceName(std::string name)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SET_DEVICE_NAME::response_t responseStruct()

## struct command_t

### methods in command struct



### variables in command struct

- uint8_t[16] name

## struct response_t
- empty response payload
