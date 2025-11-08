# command QuadratureEncoder

- [command.hxx](../../../interface/libs/commands/include/cmd/quadrature_encoder/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/quadrature_encoder/payload.hxx)

## class RaduinoCommandQuadratureEncoder

### constructors

- RaduinoCommandQuadratureEncoder()

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::QUADRATURE_ENCODER::response_t responseStruct()

## struct command_t

- empty command payload

## struct response_t

### methods in response struct

- void setCountnegative[2](uint16_t value)
- uint16_t getCountnegative()
- void setCountpositive[2](uint16_t value)
- uint16_t getCountpositive()
- void setSwitchposition(uint8_t value)
- uint8_t getSwitchposition()
- void setSwitchcount[2](uint16_t value)
- uint16_t getSwitchcount()

### variables in response struct

- uint8_t[2] countnegative
- uint8_t[2] countpositive
- uint8_t switchposition
- uint8_t[2] switchcount

