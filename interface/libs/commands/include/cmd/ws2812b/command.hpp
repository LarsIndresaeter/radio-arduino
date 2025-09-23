#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/uartCommandBase.hpp>

class UartCommandWs2812b : public UartCommandBase {
public:
    UartCommandWs2812b(std::vector<uint8_t> red, std::vector<uint8_t> green, std::vector<uint8_t> blue)
        : UartCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::WS2812B),
              COMMANDS::WS2812B::COMMAND_LENGTH)
    {
        COMMANDS::WS2812B::command_t command;

        for (int i = 0; i < sizeof(command.red); i++) {
            if (i >= red.size()) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::WS2812B::command_t, red[0]) + i)
                = red.at(i);
        }

        for (int i = 0; i < sizeof(command.green); i++) {
            if (i >= green.size()) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::WS2812B::command_t, green[0]) + i)
                = green.at(i);
        }

        for (int i = 0; i < sizeof(command.blue); i++) {
            if (i >= blue.size()) {
                break;
            }
            m_payload.at(
                offsetof(COMMANDS::WS2812B::command_t, blue[0]) + i)
                = blue.at(i);
        }

    };

    void printResponse(std::ostream& out, COMMANDS::WS2812B::response_t response) const
    {
        out << "WS2812B                : ";
    }


    std::string getCommandName() { return "ws2812b";}

    std::string getJson() {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"ws2812b\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("}");
        return(json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::WS2812B::RESPONSE_LENGTH + 4)) {
            COMMANDS::WS2812B::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "WS2812B: insufficient data" << std::endl;
        }
    };

    COMMANDS::WS2812B::response_t responseStruct()
    {
        COMMANDS::WS2812B::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

