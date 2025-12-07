# command GetLsm303d

- [command.hxx](../../../interface/libs/commands/include/cmd/get_lsm303d/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/get_lsm303d/payload.hxx)

## class RaduinoCommandGetLsm303d

### constructors

- RaduinoCommandGetLsm303d()

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::GET_LSM303D::response_t responseStruct()

## struct command_t

- empty command payload

## struct response_t

### methods in response struct

- void setAccelerometerx[2](uint16_t value)
- uint16_t getAccelerometerx()
- void setAccelerometery[2](uint16_t value)
- uint16_t getAccelerometery()
- void setAccelerometerz[2](uint16_t value)
- uint16_t getAccelerometerz()

### variables in response struct

- uint8_t[2] accelerometerX
- uint8_t[2] accelerometerY
- uint8_t[2] accelerometerZ

