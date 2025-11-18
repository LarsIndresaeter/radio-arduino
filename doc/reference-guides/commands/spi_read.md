# command SpiRead

- [command.hxx](../../../interface/libs/commands/include/cmd/spi_read/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/spi_read/payload.hxx)

## class RaduinoCommandSpiRead

### constructors

- RaduinoCommandSpiRead(uint8_t reg, uint8_t length)

### member methods in response struct

- std::string getData()
- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SPI_READ::response_t responseStruct()

## struct command_t

### methods in command struct

- void setReg(uint8_t value)
- uint8_t getReg()
- void setLength(uint8_t value)
- uint8_t getLength()

### variables in command struct

- uint8_t reg
- uint8_t length

## struct response_t

### methods in response struct

- void setReg(uint8_t value)
- uint8_t getReg()
- void setLength(uint8_t value)
- uint8_t getLength()

### variables in response struct

- uint8_t reg
- uint8_t length
- uint8_t[32] data

