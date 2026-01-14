# command GetLastDeviceIdSeen

- [command.hxx](../../../interface/libs/commands/include/cmd/get_last_device_id_seen/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/get_last_device_id_seen/payload.hxx)

## class RaduinoCommandGetLastDeviceIdSeen

### constructors

- RaduinoCommandGetLastDeviceIdSeen()

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::GET_LAST_DEVICE_ID_SEEN::response_t responseStruct()

## struct command_t

- empty command payload

## struct response_t

### methods in response struct

- void setId[4](uint32_t value)
- uint32_t getId()

### variables in response struct

- uint8_t[4] id

