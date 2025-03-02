#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

typedef struct eeprom_data {
    uint8_t TK_KEY[16];
    uint8_t HMAC_KEY[16];
    uint8_t HOTP_KEY[16];
    uint8_t EK_KEY[16];
    uint8_t NAME[16];
} eeprom_data_t;

class eeprom {
public:
    eeprom();
    void write(uint16_t address, uint8_t data);
    uint8_t read(uint16_t address);
};
