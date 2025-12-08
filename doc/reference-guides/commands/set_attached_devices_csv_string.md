# command SetAttachedDevicesCsvString

- [command.hxx](../../../interface/libs/commands/include/cmd/set_attached_devices_csv_string/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/set_attached_devices_csv_string/payload.hxx)

## class RaduinoCommandSetAttachedDevicesCsvString

### constructors

- RaduinoCommandSetAttachedDevicesCsvString(std::vector<uint8_t> csvString)
- RaduinoCommandSetAttachedDevicesCsvString(std::string csvString)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SET_ATTACHED_DEVICES_CSV_STRING::response_t responseStruct()

## struct command_t

### methods in command struct

### variables in command struct

- uint8_t[64] csvString

## struct response_t

- empty response payload

