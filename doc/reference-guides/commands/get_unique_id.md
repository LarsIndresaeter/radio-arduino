# command GetUniqueId

- [command.hxx](../../../interface/libs/commands/include/cmd/get_unique_id/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/get_unique_id/payload.hxx)

## class RaduinoCommandGetUniqueId

### constructors

- RaduinoCommandGetUniqueId()

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::GET_UNIQUE_ID::response_t responseStruct()

## struct command_t

- empty command payload

## struct response_t

### methods in response struct

- void setId[4](uint32_t value)
- uint32_t getId()

### variables in response struct

- uint8_t[4] id

