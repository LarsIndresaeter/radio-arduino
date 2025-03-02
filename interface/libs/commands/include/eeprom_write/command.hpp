#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandEepromWrite : public UartCommandBase {
public:
    UartCommandEepromWrite(uint16_t address, uint8_t data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::EEPROM_WRITE),
            COMMANDS::EEPROM_WRITE::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::EEPROM_WRITE::command_t, addressHigh))
            = address >> 8;
        m_payload.at(offsetof(COMMANDS::EEPROM_WRITE::command_t, addressLow))
            = address;
        m_payload.at(offsetof(COMMANDS::EEPROM_WRITE::command_t, data)) = data;
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::EEPROM_WRITE::response_t response(
            (uint8_t*)&m_response.data()[4]);
        out << "EEEPROM_WRITE : ";
        out << "addr=" << static_cast<int>(response.addressHigh*256 + response.addressLow)
            << ", data=" << static_cast<int>(response.data);
    };

    COMMANDS::EEPROM_WRITE::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

