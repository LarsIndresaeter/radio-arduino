#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct eeprom_data {
    uint8_t deviceName[16];
    uint8_t dataEncryptionKey[16];
    uint8_t transportEncryptionKey[16];
    // uint8_t keyEncryptionKey[16];
    //  uint8_t HMAC_KEY[16];
    //  uint8_t HOTP_KEY[16];
    uint32_t dataVersion;
    uint8_t requireEncryption;
    uint8_t isRadioNode; // otherwise it is a gateway
    uint16_t restarts;
    uint32_t uniqueId;
    uint8_t padding[96];
    uint32_t crc;
} eeprom_data_t;

static_assert(sizeof(eeprom_data_t) == 160, "eeprom_data_t is not expected size");

// valid data struct is the one with valid crc and the highest version number
typedef struct full_eeprom {
    eeprom_data_t A;
    eeprom_data_t B;
    uint8_t attached_devices_csv_string[64];
} full_eeprom_t;

namespace EEPROM {
void write(uint16_t address, uint8_t data);
void writeMultiple(uint16_t address, uint8_t* buffer, uint16_t length);
uint8_t read(uint16_t address);
void readMultiple(uint16_t address, uint8_t* buffer, uint16_t length);
} // namespace EEPROM

namespace EEPROM_DATA_STORE {
void readFromActive(uint16_t address, uint8_t* buffer, uint16_t length);
void writeToSpareAndSetAsActive(uint16_t address, uint8_t* buffer, uint16_t length);
uint8_t getRequireTransportEncryption();
void setRequireTransportEncryption(uint8_t flag);
void setIsRadioNode(uint8_t flag);
uint8_t getIsRadioNode();
void incrementRestarts();
uint16_t getRestarts();
bool readRxModeGatewayFromEeprom();
uint32_t getUniqueId();
void setUniqueId(uint32_t uniqueId);
} // namespace EEPROM_DATA_STORE
