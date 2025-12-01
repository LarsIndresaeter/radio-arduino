#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandBlink : public RaduinoCommandBase {
public:
    RaduinoCommandBlink()
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::BLINK),
              COMMANDS::BLINK::COMMAND_LENGTH)
    {
        COMMANDS::BLINK::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::BLINK::response_t response) const
    {
        out << "BLINK                  : ";
    }

    std::string getCommandName() { return "blink"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":\"" + getCommandName() + "\", ");
        json.append(getJsonCommonFields());
        json.append("\"payload\":{");
        json.append("}}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::BLINK::RESPONSE_LENGTH + 4)) {
            COMMANDS::BLINK::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "BLINK: insufficient data" << std::endl;
        }
    };

    COMMANDS::BLINK::response_t responseStruct()
    {
        COMMANDS::BLINK::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

