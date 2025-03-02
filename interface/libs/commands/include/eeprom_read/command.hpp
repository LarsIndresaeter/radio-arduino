#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandEepromRead : public UartCommandBase {
public:
    UartCommandEepromRead(uint16_t address)

        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::EEPROM_READ),
            COMMANDS::EEPROM_READ::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::EEPROM_READ::command_t, addressHigh))= address >> 8;
        m_payload.at(offsetof(COMMANDS::EEPROM_READ::command_t, addressLow))= address;
    };

    void print(std::ostream& out) const override
    {
        COMMANDS::EEPROM_READ::response_t response(
            (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]);

        out << "EEPROM_READ   : ";
        out << "addr=" << static_cast<int>(response.addressHigh + response.addressLow)
            << ", data=" << static_cast<int>(response.data);
    };

    COMMANDS::EEPROM_READ::response_t responseStruct()

    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

