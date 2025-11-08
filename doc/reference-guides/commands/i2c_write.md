# command I2cWrite

- [command.hxx](../../../interface/libs/commands/include/cmd/i2c_write/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/i2c_write/payload.hxx)

## class RaduinoCommandI2cWrite

### constructors

- RaduinoCommandI2cWrite(uint8_t device, uint16_t registerAddress, uint8_t length, std::vector<uint8_t> data)
- RaduinoCommandI2cWrite(uint8_t device, uint16_t registerAddress, uint8_t length, std::string data)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::I2C_WRITE::response_t responseStruct()

## struct command_t

### methods in command struct

- void setDevice(uint8_t value)
- uint8_t getDevice()
- void setRegisteraddress(uint16_t value)
- uint32_t getRegisteraddress[2]()
- void setLength(uint8_t value)
- uint8_t getLength()


### variables in command struct

- uint8_t device
- uint8_t[2] registerAddress
- uint8_t length
- uint8_t[16] data

## struct response_t
- empty response payload
