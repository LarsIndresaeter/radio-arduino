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
- void setUart_rx[4](uint32_t value)
- uint32_t getUart_rx()
- void setUart_tx[4](uint32_t value)
- uint32_t getUart_tx()
- void setRf_rx[4](uint32_t value)
- uint32_t getRf_rx()
- void setRf_tx[4](uint32_t value)
- uint32_t getRf_tx()
- void setRestarts[2](uint16_t value)
- uint16_t getRestarts()

### variables in response struct

- uint8_t[2] commandsParsed
- uint8_t[4] uart_rx
- uint8_t[4] uart_tx
- uint8_t[4] rf_rx
- uint8_t[4] rf_tx
- uint8_t[2] restarts

