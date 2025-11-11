# command GetStatistics

- [command.hxx](../../../interface/libs/commands/include/cmd/get_statistics/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/get_statistics/payload.hxx)

## class RaduinoCommandGetStatistics

### constructors

- RaduinoCommandGetStatistics()

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::GET_STATISTICS::response_t responseStruct()

## struct command_t

- empty command payload

## struct response_t

### methods in response struct

- void setCommandsparsed[2](uint16_t value)
- uint16_t getCommandsparsed()
- void setUartrx[4](uint32_t value)
- uint32_t getUartrx()
- void setUarttx[4](uint32_t value)
- uint32_t getUarttx()
- void setRadiorx[4](uint32_t value)
- uint32_t getRadiorx()
- void setRadiotx[4](uint32_t value)
- uint32_t getRadiotx()
- void setRestarts[2](uint16_t value)
- uint16_t getRestarts()

### variables in response struct

- uint8_t[2] commandsParsed
- uint8_t[4] uartRx
- uint8_t[4] uartTx
- uint8_t[4] radioRx
- uint8_t[4] radioTx
- uint8_t[2] restarts

