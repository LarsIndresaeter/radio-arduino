#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct eeprom_data {
    uint8_t deviceName[16];
    uint8_t EK_KEY[16];
    uint8_t TK_KEY[16];
    //uint8_t HMAC_KEY[16];
    //uint8_t HOTP_KEY[16];
    uint32_t dataVersion;
    uint8_t padding[8];
    uint32_t crc;
} eeprom_data_t;

static_assert(sizeof(eeprom_data_t) % 16 == 0, "eeprom_data_t is not a multiple of 16 bytes");

// valid data struct is the one with valid crc and the highest version number
typedef struct full_eeprom {
    eeprom_data_t A;
    eeprom_data_t B;
} full_eeprom_t;

namespace EEPROM {
void write(uint16_t address, uint8_t data);
void writeMultiple(uint16_t address, uint8_t* buffer, uint16_t length);
uint8_t read(uint16_t address);
void readMultiple(uint16_t address, uint8_t* buffer, uint16_t length);

namespace DATA_STORE {
    bool validCrcA();
    bool validCrcB();
    void calculateCrcAndSetSpareAsActive();
    void copyActiveToSpare();
    uint32_t calculateCrcSpare();
    void CRC32_calculate(uint8_t* buf, uint16_t length, uint32_t* pCrc);
    void getDeviceName(uint8_t* buffer);
    void setDeviceName(uint8_t* buffer);
    void getEncryptionKey(uint8_t* buffer);
    void setEncryptionKey(uint8_t* buffer);
    void getTransportKey(uint8_t* buffer);
    void setTransportKey(uint8_t* buffer);
} // namespace
} // namespace
