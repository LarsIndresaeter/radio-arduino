#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandKeepAlive : public RaduinoCommandBase {
public:
    RaduinoCommandKeepAlive(uint8_t time)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::KEEP_ALIVE),
              COMMANDS::KEEP_ALIVE::COMMAND_LENGTH)
    {
        COMMANDS::KEEP_ALIVE::command_t command;

        m_payload.at(offsetof(COMMANDS::KEEP_ALIVE::command_t, time)) = time;
    };

    void printResponse(std::ostream& out, COMMANDS::KEEP_ALIVE::response_t response) const
    {
        out << "KEEP_ALIVE             : ";
    }

    std::string getCommandName() { return "keep_alive"; }

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
        if (m_response.size() >= (COMMANDS::KEEP_ALIVE::RESPONSE_LENGTH + 4)) {
            COMMANDS::KEEP_ALIVE::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "KEEP_ALIVE: insufficient data" << std::endl;
        }
    };

    COMMANDS::KEEP_ALIVE::response_t responseStruct()
    {
        COMMANDS::KEEP_ALIVE::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

