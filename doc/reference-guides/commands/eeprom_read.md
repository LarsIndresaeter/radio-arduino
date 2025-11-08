# command EepromRead

- [command.hxx](../../../interface/libs/commands/include/cmd/eeprom_read/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/eeprom_read/payload.hxx)

## class RaduinoCommandEepromRead

### constructors

- RaduinoCommandEepromRead(uint16_t address)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::EEPROM_READ::response_t responseStruct()

## struct command_t

### methods in command struct

- void setAddress(uint16_t value)
- uint32_t getAddress[2]()


### variables in command struct

- uint8_t[2] address

## struct response_t

### methods in response struct

- void setAddress[2](uint16_t value)
- uint16_t getAddress()
- void setData(uint8_t value)
- uint8_t getData()

### variables in response struct

- uint8_t[2] address
- uint8_t data

