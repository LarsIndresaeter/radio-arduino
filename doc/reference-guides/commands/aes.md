# command Aes

- [command.hxx](../../../interface/libs/commands/include/cmd/aes/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/aes/payload.hxx)

## class RaduinoCommandAes

### constructors

- RaduinoCommandAes(uint8_t type, std::vector<uint8_t> data)
- RaduinoCommandAes(uint8_t type, std::string data)

### member methods in response struct

- std::string getData()
- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::AES::response_t responseStruct()

## struct command_t

### methods in command struct

- void setType(uint8_t value)
- uint8_t getType()

### variables in command struct

- uint8_t type
- uint8_t[16] data

## struct response_t

### methods in response struct

- void setType(uint8_t value)
- uint8_t getType()

### variables in response struct

- uint8_t type
- uint8_t[16] data

