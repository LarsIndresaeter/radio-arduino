#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandEepromRead : public UartCommandBase {
public:
    UartCommandEepromRead(uint16_t address)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::EEPROM_READ),
            COMMANDS::EEPROM_READ::COMMAND_LENGTH)
    {
        COMMANDS::EEPROM_READ::command_t command;

        m_payload.at(offsetof(COMMANDS::EEPROM_READ::command_t, address) + 1) = address>>8;
        m_payload.at(offsetof(COMMANDS::EEPROM_READ::command_t, address)) = address;

    };

    void printResponse(std::ostream& out, COMMANDS::EEPROM_READ::response_t response) const
    {
        out << "EEPROM_READ   : ";

        out << "addr=" << static_cast<int>(response.getAddress())
            << ", data=" << static_cast<int>(response.getData());
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::EEPROM_READ::RESPONSE_LENGTH + 4)) {
            COMMANDS::EEPROM_READ::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "EEPROM_READ: insufficient data" << std::endl;
        }
    };

    COMMANDS::EEPROM_READ::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

