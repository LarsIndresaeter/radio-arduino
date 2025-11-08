# command Nrf24l01Init

- [command.hxx](../../../interface/libs/commands/include/cmd/nrf24l01_init/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/nrf24l01_init/payload.hxx)

## class RaduinoCommandNrf24l01Init

### constructors

- RaduinoCommandNrf24l01Init(std::vector<uint8_t> txAddr, std::vector<uint8_t> rxAddr, uint8_t rfChannel, uint8_t gateway)
- RaduinoCommandNrf24l01Init(std::string txAddr, std::string rxAddr, uint8_t rfChannel, uint8_t gateway)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::NRF24L01_INIT::response_t responseStruct()

## struct command_t

### methods in command struct

- void setRfchannel(uint8_t value)
- uint8_t getRfchannel()
- void setGateway(uint8_t value)
- uint8_t getGateway()


### variables in command struct

- uint8_t[5] txAddr
- uint8_t[5] rxAddr
- uint8_t rfChannel
- uint8_t gateway

## struct response_t
- empty response payload
