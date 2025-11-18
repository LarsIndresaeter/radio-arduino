# command SetNodeAddress

- [command.hxx](../../../interface/libs/commands/include/cmd/set_node_address/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/set_node_address/payload.hxx)

## class RaduinoCommandSetNodeAddress

### constructors

- RaduinoCommandSetNodeAddress(uint8_t nodeAddress)

### member methods in response struct

- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SET_NODE_ADDRESS::response_t responseStruct()

## struct command_t

### methods in command struct

- void setNodeaddress(uint8_t value)
- uint8_t getNodeaddress()

### variables in command struct

- uint8_t nodeAddress

## struct response_t

- empty response payload

