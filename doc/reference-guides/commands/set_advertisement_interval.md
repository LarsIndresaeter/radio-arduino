# command SetAdvertisementInterval

commandId = 50

- [command.hxx](../../../interface/libs/commands/include/cmd/set_advertisement_interval/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/set_advertisement_interval/payload.hxx)

## class RaduinoCommandSetAdvertisementInterval

### constructors

- RaduinoCommandSetAdvertisementInterval(uint16_t interval)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SET_ADVERTISEMENT_INTERVAL::response_t responseStruct()

## struct command_t

### methods in command struct

- void setInterval(uint16_t value)
- uint32_t getInterval[2]()

### variables in command struct

- uint8_t[2] interval

## struct response_t

- empty response payload

