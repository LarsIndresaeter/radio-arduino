# eeprom data store

The arduino [eeprom library](../../../arduino/libs/eeprom/eeprom.cpp) has a data store that has two data sections. The active section is the one with valid crc and the highest `dataVersion` value.

```cpp
typedef struct eeprom_data {
    uint8_t deviceName[16];
    uint8_t dataEncryptionKey[16];
    uint8_t transportEncryptionKey[16];
    uint32_t dataVersion;
    uint8_t requireEncryption;
    uint8_t isRadioNode;
    uint16_t restarts;
    uint32_t uniqueId;
    uint8_t padding[96];
    uint32_t crc;
} eeprom_data_t;
```

