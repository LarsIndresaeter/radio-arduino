# command SetUniqueId

- [command.hxx](../../../interface/libs/commands/include/cmd/set_unique_id/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/set_unique_id/payload.hxx)

## class RaduinoCommandSetUniqueId

### constructors

- RaduinoCommandSetUniqueId(uint32_t id)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SET_UNIQUE_ID::response_t responseStruct()

## struct command_t

### methods in command struct

- void setId(uint32_t value)
- uint32_t getId()

### variables in command struct

- uint8_t[4] id

## struct response_t

- empty response payload

