#include <avr/io.h>
#include <eeprom.hpp>
#include <stdint.h>

#define EEPROM_SIZE 1024

// https://gist.github.com/ricardas/4367016

namespace EEPROM {
void write(uint16_t address, uint8_t data)
{
    while (EECR & (1 << EEPE))
        ;
    EEAR = address;
    EEDR = data;
    EECR |= 1 << EEMPE;
    EECR |= 1 << EEPE;
}

void writeMultiple(uint16_t address, uint8_t* buffer, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++) {
        EEPROM::write(address + i, buffer[i]);
    }
}

uint8_t read(uint16_t address)
{
    while (EECR & (1 << EEPE))
        ;
    EEAR = address;
    EECR |= (1 << EERE);
    return EEDR;
}

void readMultiple(uint16_t address, uint8_t* buffer, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++) {
        buffer[i] = EEPROM::read(address + i);
    }
}

namespace DATA_STORE {
#define ACTIVE_PARTITION_A 1
#define ACTIVE_PARTITION_B 2
#define CRC32_POLY 0x04C11DB7 /* AUTODIN II, Ethernet, & FDDI */
    static uint8_t active = 0;

    uint16_t offsetActiveStruct()
    {
        if (ACTIVE_PARTITION_B == active) {
            return offsetof(full_eeprom_t, B);
        }

        return 0;
    }

    uint16_t offsetSpareStruct()
    {
        if (ACTIVE_PARTITION_B == active) {
            return 0;
        }

        return offsetof(full_eeprom_t, B);
    }

    void increamentSpareVersion()
    {
        uint32_t version;
        EEPROM::readMultiple(offsetSpareStruct() + offsetof(eeprom_data_t, version), (uint8_t*)&version, sizeof(uint32_t));
        version = version + 1;
        EEPROM::writeMultiple(offsetSpareStruct() + offsetof(eeprom_data_t, version), (uint8_t*)&version, sizeof(uint32_t));
    }

    void findActivePartition()
    {
        uint32_t versionA;
        uint32_t versionB;
        EEPROM::readMultiple(offsetof(full_eeprom_t, A) + offsetof(eeprom_data_t, version), (uint8_t*)&versionA, sizeof(uint32_t));
        EEPROM::readMultiple(offsetof(full_eeprom_t, B) + offsetof(eeprom_data_t, version), (uint8_t*)&versionB, sizeof(uint32_t));

        if ((versionA > versionB) && EEPROM::DATA_STORE::validCrcA()) {
            active = ACTIVE_PARTITION_A;
        }
        else {
            active = ACTIVE_PARTITION_B;
        }
    }

    void clearData()
    {
        for (uint8_t i = 0; i < sizeof(full_eeprom_t); i++) {
            EEPROM::write(i, 0xff);
        }
    }

    void copyActiveToSpare()
    {
        findActivePartition();

        for (uint8_t i = 0; i < sizeof(eeprom_data_t); i++) {
            EEPROM::write(offsetSpareStruct() + i, EEPROM::read(offsetActiveStruct() + i));
        }
    }

    void CRC32_calculate(uint8_t* buf, uint16_t length, uint32_t* pCrc)
    {
        uint8_t* p;
        uint32_t tmp;

        tmp = *pCrc;

        for (p = &buf[0]; length > 0; ++p, --length) {
            uint32_t crcTableVal;
            uint8_t crcTableIndex = (uint8_t)(tmp >> 24) ^ *p;
            uint8_t j;
            for (crcTableVal = (uint32_t)crcTableIndex << 24, j = 8; j > 0;
                 --j) {
                crcTableVal = (crcTableVal & 0x80000000)
                    ? ((crcTableVal << 1) ^ CRC32_POLY)
                    : (crcTableVal << 1);
            }
            tmp = (tmp << 8) ^ crcTableVal;
        }
        *pCrc = tmp;
    }

    uint32_t calculateCrcSpare()
    {
        eeprom_data_t data;
        EEPROM::readMultiple(offsetSpareStruct(), (uint8_t*)&data, sizeof(eeprom_data_t));

        uint32_t crc = 0;
        CRC32_calculate((uint8_t*)&data, sizeof(eeprom_data_t) - 4, &crc);

        return crc;
    }

    bool validCrcA()
    {
        eeprom_data_t A;
        EEPROM::readMultiple(offsetof(full_eeprom_t, A), (uint8_t*)&A, sizeof(eeprom_data_t));

        // calculate crc
        uint32_t crc = 0;
        CRC32_calculate((uint8_t*)&A, sizeof(eeprom_data_t) - 4, &crc);

        return crc == A.crc;
    }

    bool validCrcB()
    {
        eeprom_data_t B;
        EEPROM::readMultiple(offsetof(full_eeprom_t, B), (uint8_t*)&active, sizeof(eeprom_data_t));

        // calculate crc
        uint32_t crc = 0;
        CRC32_calculate((uint8_t*)&B, sizeof(eeprom_data_t) - 4, &crc);

        return crc == B.crc;
    }

    void calculateCrcAndSetSpareAsActive()
    {
        increamentSpareVersion();
        uint32_t crc = calculateCrcSpare();

        EEPROM::writeMultiple(offsetSpareStruct() + offsetof(eeprom_data_t, crc), (uint8_t*)&crc, sizeof(uint32_t));
        findActivePartition();
    }

    void getDeviceName(uint8_t* buffer)
    {
        for (uint8_t i = 0; i < 16; i++) {
            buffer[i] = EEPROM::read(offsetActiveStruct() + offsetof(eeprom_data_t, NAME) + i);
        }
    }

    void setDeviceName(uint8_t* buffer)
    {
        copyActiveToSpare();
        for (uint8_t i = 0; i < 16; i++) {
            EEPROM::write(offsetSpareStruct() + offsetof(eeprom_data_t, NAME) + i, buffer[i]);
        }
        calculateCrcAndSetSpareAsActive();
    }
} // namespace
} // namespace
