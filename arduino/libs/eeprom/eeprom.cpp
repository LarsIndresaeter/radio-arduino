#include <eeprom.hpp>
#include <stdint.h>
#include <avr/io.h>

#define EEPROM_SIZE 1024

// https://gist.github.com/ricardas/4367016

Eeprom::Eeprom()
{
}

void Eeprom::write(uint16_t address, uint8_t data) {
	while(EECR & (1 << EEPE));
	EEAR = address;
	EEDR = data;
	EECR |= 1 << EEMPE;
	EECR |= 1 << EEPE;
}


uint8_t Eeprom::read(uint16_t address) {
    while (EECR & (1 << EEPE));
    EEAR = address;
    EECR |= (1 << EERE);
    return EEDR;
}
