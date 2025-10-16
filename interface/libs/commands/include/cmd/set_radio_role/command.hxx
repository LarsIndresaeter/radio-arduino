#pragma once
// This file is generated with the script: `interface/libs/commands/generate.py`

#include <common/raduinoCommandBase.hpp>

class RaduinoCommandSetRadioRole : public RaduinoCommandBase {
public:
    RaduinoCommandSetRadioRole(uint8_t isRadioNode)
        : RaduinoCommandBase(
              static_cast<uint8_t>(COMMANDS::OI::SET_RADIO_ROLE),
              COMMANDS::SET_RADIO_ROLE::COMMAND_LENGTH)
    {
        COMMANDS::SET_RADIO_ROLE::command_t command;

        m_payload.at(offsetof(COMMANDS::SET_RADIO_ROLE::command_t, isRadioNode)) = isRadioNode;
    };

    void printResponse(std::ostream& out, COMMANDS::SET_RADIO_ROLE::response_t response) const
    {
        out << "SET_RADIO_ROLE         : ";
    }

    std::string getCommandName() { return "set_radio_role"; }

    std::string getJson()
    {
        std::string json;
        json.append("{");
        json.append("\"name\":");
        json.append("\"set_radio_role\", ");
        json.append("\"timestamp\":");
        json.append(std::to_string(getTimeStamp()));
        json.append(", ");
        json.append("}");
        return (json);
    };

    void print(std::ostream& out, std::vector<uint8_t> responsePayload) const override
    {
        if (m_response.size() >= (COMMANDS::SET_RADIO_ROLE::RESPONSE_LENGTH + 4)) {
            COMMANDS::SET_RADIO_ROLE::response_t response(
                (uint8_t*)&responsePayload.data()[0]);
            printResponse(out, response);
        }
        else {
            std::cout << "SET_RADIO_ROLE: insufficient data" << std::endl;
        }
    };

    COMMANDS::SET_RADIO_ROLE::response_t responseStruct()
    {
        COMMANDS::SET_RADIO_ROLE::response_t response;

        if (m_responsePayload.size() >= sizeof(response)) {
            return { (uint8_t*)&m_responsePayload[0] };
        }

        return (response);
    };
};

