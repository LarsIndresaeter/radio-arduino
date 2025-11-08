# command Wakeup

- [command.hxx](../../../interface/libs/commands/include/cmd/wakeup/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/wakeup/payload.hxx)

## class RaduinoCommandWakeup

### constructors

- RaduinoCommandWakeup(uint8_t checkAttentionFlag)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::WAKEUP::response_t responseStruct()

## struct command_t

### methods in command struct

- void setCheckattentionflag(uint8_t value)
- uint8_t getCheckattentionflag()


### variables in command struct

- uint8_t checkAttentionFlag

## struct response_t

### methods in response struct

- void setDiscovered(uint8_t value)
- uint8_t getDiscovered()

### variables in response struct

- uint8_t discovered

