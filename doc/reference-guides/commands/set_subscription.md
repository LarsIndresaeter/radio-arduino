# command SetSubscription

commandId = 49

- [command.hxx](../../../interface/libs/commands/include/cmd/set_subscription/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/set_subscription/payload.hxx)

## class RaduinoCommandSetSubscription

### constructors

- RaduinoCommandSetSubscription(uint8_t id, uint16_t interval)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SET_SUBSCRIPTION::response_t responseStruct()

## struct command_t

### methods in command struct

- void setId(uint8_t value)
- uint8_t getId()
- void setInterval(uint16_t value)
- uint32_t getInterval[2]()

### variables in command struct

- uint8_t id
- uint8_t[2] interval

## struct response_t

- empty response payload

