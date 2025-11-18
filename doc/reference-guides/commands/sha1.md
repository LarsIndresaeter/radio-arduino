# command Sha1

- [command.hxx](../../../interface/libs/commands/include/cmd/sha1/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/sha1/payload.hxx)

## class RaduinoCommandSha1

### constructors

- RaduinoCommandSha1(std::vector<uint8_t> data)
- RaduinoCommandSha1(std::string data)

### member methods in response struct

- std::string getData()
- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SHA1::response_t responseStruct()

## struct command_t

### methods in command struct

### variables in command struct

- uint8_t[20] data

## struct response_t

### methods in response struct

### variables in response struct

- uint8_t[20] data

