# command SetRadioChannel

- [command.hxx](../../../interface/libs/commands/include/cmd/set_radio_channel/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/set_radio_channel/payload.hxx)

## class RaduinoCommandSetRadioChannel

### constructors

- RaduinoCommandSetRadioChannel(uint8_t channel)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SET_RADIO_CHANNEL::response_t responseStruct()

## struct command_t

### methods in command struct

- void setChannel(uint8_t value)
- uint8_t getChannel()

### variables in command struct

- uint8_t channel

## struct response_t

- empty response payload

