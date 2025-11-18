# command Pwm

- [command.hxx](../../../interface/libs/commands/include/cmd/pwm/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/pwm/payload.hxx)

## class RaduinoCommandPwm

### constructors

- RaduinoCommandPwm(uint8_t port, uint8_t pin, uint8_t value)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::PWM::response_t responseStruct()

## struct command_t

### methods in command struct

- void setPort(uint8_t value)
- uint8_t getPort()
- void setPin(uint8_t value)
- uint8_t getPin()
- void setValue(uint8_t value)
- uint8_t getValue()

### variables in command struct

- uint8_t port
- uint8_t pin
- uint8_t value

## struct response_t

### methods in response struct

- void setPort(uint8_t value)
- uint8_t getPort()
- void setPin(uint8_t value)
- uint8_t getPin()
- void setValue(uint8_t value)
- uint8_t getValue()

### variables in response struct

- uint8_t port
- uint8_t pin
- uint8_t value

