#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandWakeup : public RaduinoCommandBase {
public:
    RaduinoCommandWakeup(uint8_t checkAttentionFlag)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::WAKEUP),
              COMMANDS::WAKEUP::COMMAND_LENGTH)
    {
        COMMANDS::WAKEUP::command_t command;

        m_payload.at(offsetof(COMMANDS::WAKEUP::command_t, checkAttentionFlag)) = checkAttentionFlag;
    };

    void printResponse(std::ostream& out, COMMANDS::WAKEUP::response_t response) const
    {
        out << "WAKEUP                 : ";
        out << " discovered=" << static_cast<uint32_t>(response.getDiscovered());
    }

    std::string getCommandName() { return "wakeup"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":\"" + getCommandName() + "\", ");
        json.append(getJsonCommonFields());
        json.append("\"payload\":{");
        json.append("\"discovered\":");
        json.append(std::to_string(responseStruct().getDiscovered()));
        json.append("}}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::WAKEUP::RESPONSE_LENGTH + 4)) {
            COMMANDS::WAKEUP::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "WAKEUP: insufficient data" << std::endl;
        }
    };

    COMMANDS::WAKEUP::response_t responseStruct()
    {
        COMMANDS::WAKEUP::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

