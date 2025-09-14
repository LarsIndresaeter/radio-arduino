#pragma once

#include <common/uartCommandBase.hpp>

class UartCommandSsd1306 : public UartCommandBase {
public:
    UartCommandSsd1306(uint8_t line, std::vector<uint8_t> data)
        : UartCommandBase(
            static_cast<uint8_t>(COMMANDS::OI::SSD1306),
            COMMANDS::SSD1306::COMMAND_LENGTH)
    {
        COMMANDS::SSD1306::command_t command;

        m_payload.at(offsetof(COMMANDS::SSD1306::command_t, line)) = line;

        for (int i = 0; i < data.size(); i++) {
            if (i >= sizeof(command.data)) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::SSD1306::command_t, data[0]) + i)
                = data.at(i);
        }

    };

    void printResponse(std::ostream& out, COMMANDS::SSD1306::response_t response) const
    {
        out << "SSD1306   : ";
    }

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::SSD1306::RESPONSE_LENGTH + 4)) {
            COMMANDS::SSD1306::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        } else
        {
            std::cout << "SSD1306: insufficient data" << std::endl;
        }
    };

    COMMANDS::SSD1306::response_t responseStruct()
    {
        return { (uint8_t*)&m_response.data()[PROTOCOL::HEADER::LENGTH] };
    };
};

