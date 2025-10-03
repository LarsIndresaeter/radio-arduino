#include <eeprom.hpp>
#include <stdint.h>
#include <avr/io.h>

#define EEPROM_SIZE 1024

// https://gist.github.com/ricardas/4367016

namespace EEPROM
{
    void write(uint16_t address, uint8_t data) {
        while(EECR & (1 << EEPE));
        EEAR = address;
        EEDR = data;
        EECR |= 1 << EEMPE;
        EECR |= 1 << EEPE;
    }

    uint8_t read(uint16_t address) {
        while (EECR & (1 << EEPE));
        EEAR = address;
        EECR |= (1 << EERE);
        return EEDR;
    }

   //namespace DATA_STORE {
        //uint16_t offset_active = 0;
        //uint16_t offset_spare = 0;

        //uint16_t offsetActiveStruct()
        //{
            //return 0;
        //}

        //uint16_t offsetSpareStruct()
        //{
            //return 0;
        //}

        //void copyActiveToSpare()
        //{// TODO
        //}

        //void calculateCrcAndSetSpareAsActive()
        //{// TODO
        //}
    //} // namespace
} // namespace
