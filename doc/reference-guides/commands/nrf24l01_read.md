# command Nrf24l01Read

- [command.hxx](../../../interface/libs/commands/include/cmd/nrf24l01_read/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/nrf24l01_read/payload.hxx)

## class RaduinoCommandNrf24l01Read

### constructors

- RaduinoCommandNrf24l01Read(uint8_t length, std::vector<uint8_t> data)
- RaduinoCommandNrf24l01Read(uint8_t length, std::string data)

### member methods in response struct

- std::string getData()
- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::NRF24L01_READ::response_t responseStruct()

## struct command_t

### methods in command struct

- void setLength(uint8_t value)
- uint8_t getLength()


### variables in command struct

- uint8_t length
- uint8_t[32] data

## struct response_t

### methods in response struct

- void setLength(uint8_t value)
- uint8_t getLength()

### variables in response struct

- uint8_t length
- uint8_t[32] data

