# command KeepAlive

- [command.hxx](../../../interface/libs/commands/include/cmd/keep_alive/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/keep_alive/payload.hxx)

## class RaduinoCommandKeepAlive

### constructors

- RaduinoCommandKeepAlive(uint8_t time)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::KEEP_ALIVE::response_t responseStruct()

## struct command_t

### methods in command struct

- void setTime(uint8_t value)
- uint8_t getTime()

### variables in command struct

- uint8_t time

## struct response_t

- empty response payload

