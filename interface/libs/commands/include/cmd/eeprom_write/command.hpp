#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandEepromWrite : public UartCommandBase {
public:
    UartCommandEepromWrite(uint16_t address, uint8_t data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::EEPROM_WRITE),
            COMMANDS::EEPROM_WRITE::COMMAND_LENGTH)
    {
        COMMANDS::EEPROM_WRITE::command_t command;

        m_payload.at(offsetof(COMMANDS::EEPROM_WRITE::command_t, address) + 1) = address>>8;
        m_payload.at(offsetof(COMMANDS::EEPROM_WRITE::command_t, address)) = address;

        m_payload.at(offsetof(COMMANDS::EEPROM_WRITE::command_t, data)) = data;

    };

    void printResponse(std::ostream& out, COMMANDS::EEPROM_WRITE::response_t response) const
    {
        out << "EEPROM_WRITE           : ";
        out << " address=" << static_cast<int>(response.getAddress());
        out << " data=" << static_cast<int>(response.getData());
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::EEPROM_WRITE::RESPONSE_LENGTH + 4)) {
            COMMANDS::EEPROM_WRITE::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "EEPROM_WRITE: insufficient data" << std::endl;
        }
    };

    COMMANDS::EEPROM_WRITE::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

