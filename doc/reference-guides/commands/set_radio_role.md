# command SetRadioRole

- [command.hxx](../../../interface/libs/commands/include/cmd/set_radio_role/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/set_radio_role/payload.hxx)

## class RaduinoCommandSetRadioRole

### constructors

- RaduinoCommandSetRadioRole(uint8_t isRadioNode)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SET_RADIO_ROLE::response_t responseStruct()

## struct command_t

### methods in command struct

- void setIsradionode(uint8_t value)
- uint8_t getIsradionode()


### variables in command struct

- uint8_t isRadioNode

## struct response_t
- empty response payload
