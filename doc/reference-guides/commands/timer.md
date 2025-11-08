# command Timer

- [command.hxx](../../../interface/libs/commands/include/cmd/timer/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/timer/payload.hxx)

## class RaduinoCommandTimer

### constructors

- RaduinoCommandTimer()

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::TIMER::response_t responseStruct()

## struct command_t

- empty command payload

## struct response_t

### methods in response struct

- void setPulsewidth[2](uint16_t value)
- uint16_t getPulsewidth()

### variables in response struct

- uint8_t[2] pulseWidth

