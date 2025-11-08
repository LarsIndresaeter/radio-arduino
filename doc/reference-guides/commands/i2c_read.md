# command I2cRead

- [command.hxx](../../../interface/libs/commands/include/cmd/i2c_read/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/i2c_read/payload.hxx)

## class RaduinoCommandI2cRead

### constructors

- RaduinoCommandI2cRead(uint8_t device, uint16_t registerAddress, uint8_t length)

### member methods in response struct

- std::string getData()
- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::I2C_READ::response_t responseStruct()

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

## struct response_t

### methods in response struct

- void setDevice(uint8_t value)
- uint8_t getDevice()
- void setRegisteraddress[2](uint16_t value)
- uint16_t getRegisteraddress()
- void setLength(uint8_t value)
- uint8_t getLength()

### variables in response struct

- uint8_t device
- uint8_t[2] registerAddress
- uint8_t length
- uint8_t[16] data

