#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

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

        for (int i = 0; i < sizeof(command.data); i++) {
            if (i >= data.size()) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::SSD1306::command_t, data[0]) + i)
                = data.at(i);
        }

    };

    void printResponse(std::ostream& out, COMMANDS::SSD1306::response_t response) const
    {
        out << "SSD1306                : ";
    }


    std::string getCommandName() { return "ssd1306";}

    std::string getJson() {
        std::string json;
        json.append("{");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append("\"name\":");
        json.append("\"ssd1306\", ");
        json.append(", ");
        json.append("}");
        return(json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::SSD1306::RESPONSE_LENGTH + 4)) {
            COMMANDS::SSD1306::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SSD1306: insufficient data" << std::endl;
        }
    };

    COMMANDS::SSD1306::response_t responseStruct()
    {
        COMMANDS::SSD1306::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

