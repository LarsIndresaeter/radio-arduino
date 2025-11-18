# command RadioUart

- [command.hxx](../../../interface/libs/commands/include/cmd/radio_uart/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/radio_uart/payload.hxx)

## class RaduinoCommandRadioUart

### constructors

- RaduinoCommandRadioUart(uint8_t mode)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::RADIO_UART::response_t responseStruct()

## struct command_t

### methods in command struct

- void setMode(uint8_t value)
- uint8_t getMode()

### variables in command struct

- uint8_t mode

## struct response_t

- empty response payload

