# command GetActiveTimeCounter

commandId = 47

- [command.hxx](../../../interface/libs/commands/include/cmd/get_active_time_counter/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/get_active_time_counter/payload.hxx)

## class RaduinoCommandGetActiveTimeCounter

### constructors

- RaduinoCommandGetActiveTimeCounter(std::vector<uint8_t> timestamp)
- RaduinoCommandGetActiveTimeCounter(std::string timestamp)

### member methods in response struct

- std::string getActive_time()
- std::string getSleep_time()
- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::GET_ACTIVE_TIME_COUNTER::response_t responseStruct()

## struct command_t

### methods in command struct

### variables in command struct

- uint8_t[8] timestamp

## struct response_t

### methods in response struct

### variables in response struct

- uint8_t[8] active_time
- uint8_t[8] sleep_time

