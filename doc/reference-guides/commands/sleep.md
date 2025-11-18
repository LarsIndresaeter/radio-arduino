# command Sleep

- [command.hxx](../../../interface/libs/commands/include/cmd/sleep/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/sleep/payload.hxx)

## class RaduinoCommandSleep

### constructors

- RaduinoCommandSleep(uint32_t delay)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SLEEP::response_t responseStruct()

## struct command_t

### methods in command struct

- void setDelay(uint32_t value)
- uint32_t getDelay()

### variables in command struct

- uint8_t[4] delay

## struct response_t

- empty response payload

