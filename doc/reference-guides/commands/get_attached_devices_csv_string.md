# command GetAttachedDevicesCsvString

- [command.hxx](../../../interface/libs/commands/include/cmd/get_attached_devices_csv_string/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/get_attached_devices_csv_string/payload.hxx)

## class RaduinoCommandGetAttachedDevicesCsvString

### constructors

- RaduinoCommandGetAttachedDevicesCsvString()

### member methods in response struct

- std::string getCsvstring()
- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::GET_ATTACHED_DEVICES_CSV_STRING::response_t responseStruct()

## struct command_t

- empty command payload

## struct response_t

### methods in response struct

### variables in response struct

- uint8_t[64] csvString

