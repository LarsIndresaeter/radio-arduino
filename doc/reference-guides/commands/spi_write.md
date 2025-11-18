# command SpiWrite

- [command.hxx](../../../interface/libs/commands/include/cmd/spi_write/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/spi_write/payload.hxx)

## class RaduinoCommandSpiWrite

### constructors

- RaduinoCommandSpiWrite(uint8_t reg, uint8_t length, std::vector<uint8_t> data)
- RaduinoCommandSpiWrite(uint8_t reg, uint8_t length, std::string data)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SPI_WRITE::response_t responseStruct()

## struct command_t

### methods in command struct

- void setReg(uint8_t value)
- uint8_t getReg()
- void setLength(uint8_t value)
- uint8_t getLength()

### variables in command struct

- uint8_t reg
- uint8_t length
- uint8_t[32] data

## struct response_t

- empty response payload

