# command Vcc

- [command.hxx](../../../interface/libs/commands/include/cmd/vcc/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/vcc/payload.hxx)

## class RaduinoCommandVcc

### constructors

- RaduinoCommandVcc()

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::VCC::response_t responseStruct()

## struct command_t

- empty command payload

## struct response_t

### methods in response struct

- void setVcc[2](uint16_t value)
- uint16_t getVcc()

### variables in response struct

- uint8_t[2] vcc

