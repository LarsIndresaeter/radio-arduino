# command SetKey

- [command.hxx](../../../interface/libs/commands/include/cmd/set_key/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/set_key/payload.hxx)

## class RaduinoCommandSetKey

### constructors

- RaduinoCommandSetKey(uint8_t keyId, std::vector<uint8_t> keyValue)
- RaduinoCommandSetKey(uint8_t keyId, std::string keyValue)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SET_KEY::response_t responseStruct()

## struct command_t

### methods in command struct

- void setKeyid(uint8_t value)
- uint8_t getKeyid()


### variables in command struct

- uint8_t keyId
- uint8_t[16] keyValue

## struct response_t
- empty response payload
