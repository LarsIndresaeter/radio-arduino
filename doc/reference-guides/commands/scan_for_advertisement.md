# command ScanForAdvertisement

- [command.hxx](../../../interface/libs/commands/include/cmd/scan_for_advertisement/command.hxx)
- [payload.hxx](../../../interface/libs/commands/include/cmd/scan_for_advertisement/payload.hxx)

## class RaduinoCommandScanForAdvertisement

### constructors

- RaduinoCommandScanForAdvertisement(uint32_t id, uint16_t timeout)

### member methods in response struct

- std::string getData()
- std::string getJson()
- std::string printResponse()
- std::string getCommandName()
- COMMANDS::SCAN_FOR_ADVERTISEMENT::response_t responseStruct()

## struct command_t

### methods in command struct

- void setId(uint32_t value)
- uint32_t getId()
- void setTimeout(uint16_t value)
- uint32_t getTimeout[2]()

### variables in command struct

- uint8_t[4] id
- uint8_t[2] timeout

## struct response_t

### methods in response struct

- void setSequence_number[4](uint32_t value)
- uint32_t getSequence_number()
- void setFlags(uint8_t value)
- uint8_t getFlags()
- void setId[4](uint32_t value)
- uint32_t getId()

### variables in response struct

- uint8_t[4] sequence_number
- uint8_t[16] data
- uint8_t flags
- uint8_t[4] id

