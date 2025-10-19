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

} // namespace EEPROM

namespace EEPROM_DATA_STORE {
#define ACTIVE_PARTITION_A 1
#define ACTIVE_PARTITION_B 2
#define CRC32_POLY 0x04C11DB7 /* AUTODIN II, Ethernet, & FDDI */
    bool validCrcA();
    bool validCrcB();
    void calculateCrcAndSetSpareAsActive();
    void copyActiveToSpare();
    uint32_t calculateCrcSpare();
    void CRC32_calculate(uint8_t* buf, uint16_t length, uint32_t* pCrc);

    static uint8_t active = 0;

    uint16_t offsetActiveStruct()
    {
        if (ACTIVE_PARTITION_B == active) {
            return offsetof(full_eeprom_t, B);
        }

        return offsetof(full_eeprom_t, A);
    }

    uint16_t offsetSpareStruct()
    {
        if (ACTIVE_PARTITION_B == active) {
            return offsetof(full_eeprom_t, A);
        }

        return offsetof(full_eeprom_t, B);
    }

    void increamentSpareVersion()
    {
        uint32_t dataVersion;
        EEPROM::readMultiple(offsetSpareStruct() + offsetof(eeprom_data_t, dataVersion), (uint8_t*)&dataVersion, sizeof(uint32_t));
        dataVersion = dataVersion + 1;
        EEPROM::writeMultiple(offsetSpareStruct() + offsetof(eeprom_data_t, dataVersion), (uint8_t*)&dataVersion, sizeof(uint32_t));
    }

    void clearData()
    {
        for (uint8_t i = 0; i < sizeof(full_eeprom_t); i++) {
            EEPROM::write(i, 0x00);
        }

        eeprom_data_t A;
        EEPROM::readMultiple(offsetof(full_eeprom_t, A), (uint8_t*)&A, sizeof(eeprom_data_t));

        uint32_t dataVersion = 0;
        EEPROM::writeMultiple(offsetof(full_eeprom_t, A) + offsetof(eeprom_data_t, dataVersion), (uint8_t*)&dataVersion, sizeof(uint32_t));
        uint32_t crc = 0;
        CRC32_calculate((uint8_t*)&A, sizeof(eeprom_data_t) - 4, &crc);
        EEPROM::writeMultiple(offsetof(full_eeprom_t, A) + offsetof(eeprom_data_t, crc), (uint8_t*)&crc, sizeof(uint32_t));

        // copy A to B
        for (uint8_t i = 0; i < sizeof(eeprom_data_t); i++) {
            EEPROM::write(offsetof(full_eeprom_t, B) + i, EEPROM::read(offsetof(full_eeprom_t, A) + i));
        }
    }

    void findActivePartition()
    {
        uint32_t dataVersionA;
        uint32_t dataVersionB;
        EEPROM::readMultiple(offsetof(full_eeprom_t, A) + offsetof(eeprom_data_t, dataVersion), (uint8_t*)&dataVersionA, sizeof(uint32_t));
        EEPROM::readMultiple(offsetof(full_eeprom_t, B) + offsetof(eeprom_data_t, dataVersion), (uint8_t*)&dataVersionB, sizeof(uint32_t));
        bool validA = EEPROM_DATA_STORE::validCrcA();
        bool validB = EEPROM_DATA_STORE::validCrcB();

        if ((false == validA) && (false == validB)) {
            clearData();
        }

        if ((true == validA) && (true == validB)) {
            if (dataVersionA > dataVersionB) {

                active = ACTIVE_PARTITION_A;
            }
            else {
                active = ACTIVE_PARTITION_B;
            }
        }
        else if (validA) { // implies that B is invalid
            active = ACTIVE_PARTITION_A;
        }
        else {
            active = ACTIVE_PARTITION_B;
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
        EEPROM::readMultiple(offsetof(full_eeprom_t, B), (uint8_t*)&B, sizeof(eeprom_data_t));

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

    void readFromActive(uint16_t address, uint8_t* buffer, uint16_t length)
    {
        findActivePartition();
        EEPROM::readMultiple(offsetActiveStruct() + address, &buffer[0], length);
    }

    void writeToSpareAndSetAsActive(uint16_t address, uint8_t* buffer, uint16_t length)
    {
        copyActiveToSpare();
        EEPROM::writeMultiple(offsetSpareStruct() + address, &buffer[0], length);
        calculateCrcAndSetSpareAsActive();
    }

    uint8_t getRequireTransportEncryption()
    {
        findActivePartition();
        return EEPROM::read(offsetActiveStruct() + offsetof(eeprom_data_t, requireEncryption));
    }

    void setRequireTransportEncryption(uint8_t flag)
    {
        writeToSpareAndSetAsActive(offsetof(eeprom_data_t, requireEncryption), &flag, sizeof(uint8_t));
    }

    void setIsRadioNode(uint8_t flag)
    {
        writeToSpareAndSetAsActive(offsetof(eeprom_data_t, isRadioNode), &flag, sizeof(uint8_t));
    }

    uint8_t getIsRadioNode()
    {
        findActivePartition();
        return EEPROM::read(offsetActiveStruct() + offsetof(eeprom_data_t, isRadioNode));
    }

    uint16_t getRestarts()
    {
        findActivePartition();
        uint16_t restarts;
        EEPROM::readMultiple(offsetActiveStruct() + offsetof(eeprom_data, restarts), (uint8_t*)&restarts, sizeof(uint16_t));
        return restarts;
    }

    void incrementRestarts()
    {
        uint16_t restarts = getRestarts() + 1; // calls findActivePartition
        EEPROM::writeMultiple(offsetSpareStruct() + offsetof(eeprom_data_t, restarts), (uint8_t*)&restarts, sizeof(uint16_t));
        calculateCrcAndSetSpareAsActive();
    }

    bool readRxModeGatewayFromEeprom()
    {
        bool mode=true; // new unprogrammed device is gateway

        if (false) { // set to true when you need to force update isRadioNode flag in eeprom 
            setIsRadioNode('g');
        };

        if ('n' == getIsRadioNode()) {
            mode = false; // override default role
        }

        if ('g' == getIsRadioNode()) {
            mode = true; // override default role
        }

        return mode;
    }

} // namespace EEPROM_DATA_STORE


