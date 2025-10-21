#pragma once

#include <stdint.h>
#include <string.h>
#define IF_PROGMEM PROGMEM
#include <avr/pgmspace.h>

namespace AES {

void Crypt(uint8_t* data, uint8_t* key, uint8_t* IV);
void Decrypt(uint8_t* data, uint8_t* key, uint8_t* IV);
void Sanitize();
} // namespace
