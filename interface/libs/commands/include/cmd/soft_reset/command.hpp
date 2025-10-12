#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandSoftReset : public RaduinoCommandBase {
public:
    RaduinoCommandSoftReset()
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SOFT_RESET),
              COMMANDS::SOFT_RESET::COMMAND_LENGTH)
    {
        COMMANDS::SOFT_RESET::command_t command;
    };

    void printResponse(std::ostream& out, COMMANDS::SOFT_RESET::response_t response) const
    {
        out << "SOFT_RESET             : ";
    }

    std::string getCommandName() { return "soft_reset"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"soft_reset\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::SOFT_RESET::RESPONSE_LENGTH + 4)) {
            COMMANDS::SOFT_RESET::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SOFT_RESET: insufficient data" << std::endl;
        }
    };

    COMMANDS::SOFT_RESET::response_t responseStruct()
    {
        COMMANDS::SOFT_RESET::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

