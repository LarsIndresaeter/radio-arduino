# command Gpio

- [command.hxx](../../../interface/libs/commands/include/cmd/gpio/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/gpio/payload.hxx)

## class RaduinoCommandGpio

### constructors

- RaduinoCommandGpio()

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::GPIO::response_t responseStruct()

## struct command_t

- empty command payload

## struct response_t

### methods in response struct

- void setPortb(uint8_t value)
- uint8_t getPortb()
- void setPortc(uint8_t value)
- uint8_t getPortc()
- void setPortd(uint8_t value)
- uint8_t getPortd()

### variables in response struct

- uint8_t portB
- uint8_t portC
- uint8_t portD

