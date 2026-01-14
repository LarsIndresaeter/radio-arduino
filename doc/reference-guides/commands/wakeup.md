# command Wakeup

- [command.hxx](../../../interface/libs/commands/include/cmd/wakeup/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/wakeup/payload.hxx)

## class RaduinoCommandWakeup

### constructors

- RaduinoCommandWakeup(uint8_t checkAttentionFlag, uint32_t id)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::WAKEUP::response_t responseStruct()

## struct command_t

### methods in command struct

- void setCheckattentionflag(uint8_t value)
- uint8_t getCheckattentionflag()
- void setId(uint32_t value)
- uint32_t getId()

### variables in command struct

- uint8_t checkAttentionFlag
- uint8_t[4] id

## struct response_t

### methods in response struct

- void setDiscovered(uint8_t value)
- uint8_t getDiscovered()

### variables in response struct

- uint8_t discovered

