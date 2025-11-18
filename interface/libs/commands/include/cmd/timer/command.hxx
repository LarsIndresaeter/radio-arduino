#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandTimer : public RaduinoCommandBase {
public:
    RaduinoCommandTimer()
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::TIMER),
              COMMANDS::TIMER::COMMAND_LENGTH)
    {
        COMMANDS::TIMER::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::TIMER::response_t response) const
    {
        out << "TIMER                  : ";
        out << " pulseWidth=" << static_cast<uint32_t>(response.getPulsewidth());
    }

    std::string getCommandName() { return "timer"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"timer\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("\"pulseWidth\":");
        json.append(std::to_string(responseStruct().getPulsewidth()));
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::TIMER::RESPONSE_LENGTH + 4)) {
            COMMANDS::TIMER::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "TIMER: insufficient data" << std::endl;
        }
    };

    COMMANDS::TIMER::response_t responseStruct()
    {
        COMMANDS::TIMER::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

