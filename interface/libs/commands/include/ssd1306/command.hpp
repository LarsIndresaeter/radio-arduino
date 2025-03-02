#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandSsd1306 : public UartCommandBase {
public:
    UartCommandSsd1306(uint8_t line, std::vector<uint8_t> data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::SSD1306),
            COMMANDS::SSD1306::COMMAND_LENGTH)
    {
        m_payload.at(offsetof(COMMANDS::SSD1306::command_t, line)) = line;

        for (int i = 0; i < data.size() && i < COMMANDS::SSD1306::STRING_LENGTH; i++) {
            m_payload.at(offsetof(COMMANDS::SSD1306::command_t, data) + i)
                = data.at(i);
        }
    };

    void print(std::ostream& out) const override
    {
        uint8_t c;
        out << "SSD1306          : ";
        UartCommandBase::print(out);
    };

    COMMANDS::SSD1306::response_t responseStruct()
    {
        return {(uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH]};
    };
};

